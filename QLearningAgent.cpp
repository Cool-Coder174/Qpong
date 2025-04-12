#include "QLearningAgent.h"
#include <vector>
#include <limits> // For std::numeric_limits
#include <algorithm> // For std::max_element
#include <fstream>   // For file I/O
#include <iostream>  // For error messages
#include <sstream>   // For string stream parsing

// Constructor
QLearningAgent::QLearningAgent()
    : alpha(0.1), gamma(0.9), epsilon(0.1), // Default to Easy/Medium
      rng(std::random_device{}()), // Seed the random number generator
      exploration_distribution(0.0, 1.0),
      action_distribution(0, NUM_ACTIONS - 1)
{
    // Set default difficulty (can be changed later)
    set_difficulty(DifficultyLevel::EASY);
}

// Set difficulty parameters
void QLearningAgent::set_difficulty(DifficultyLevel level) {
    switch (level) {
        case DifficultyLevel::EASY:
            alpha = 0.1; // Slower learning
            gamma = 0.9; // Standard discount
            epsilon = 0.1; // Low exploration
            std::cout << "AI Difficulty set to EASY (alpha=" << alpha << ", gamma=" << gamma << ", epsilon=" << epsilon << ")" << std::endl;
            break;
        case DifficultyLevel::MEDIUM:
             alpha = 0.2; // Moderate learning
             gamma = 0.9; // Standard discount
             epsilon = 0.2; // Moderate exploration
             std::cout << "AI Difficulty set to MEDIUM (alpha=" << alpha << ", gamma=" << gamma << ", epsilon=" << epsilon << ")" << std::endl;
            break;
        case DifficultyLevel::HARD:
            alpha = 0.2; // Faster learning (or keep moderate)
            gamma = 0.95; // Higher value on future rewards (more aggressive)
            epsilon = 0.4; // High exploration (tries more things, potentially riskier)
             std::cout << "AI Difficulty set to HARD (alpha=" << alpha << ", gamma=" << gamma << ", epsilon=" << epsilon << ")" << std::endl;
            break;
    }
}


// Get Q-value, defaulting to 0 if state/action not seen
double QLearningAgent::get_q_value(const State& state, int action_index) const {
    auto it = q_table.find(state);
    if (it != q_table.end()) {
        // State exists, return the Q-value for the specific action
        return it->second[action_index];
    } else {
        // State hasn't been seen before, return default value (usually 0)
        return 0.0;
    }
}

// Get the index of the best action for a state
int QLearningAgent::get_best_action_index(const State& state) const {
    auto it = q_table.find(state);
    if (it != q_table.end()) {
        // State exists, find the action with the maximum Q-value
        const auto& q_values = it->second;
        // Use std::max_element to find the iterator to the max value
        auto max_it = std::max_element(q_values.begin(), q_values.end());
        // Return the index of the max element
        return static_cast<int>(std::distance(q_values.begin(), max_it));
    } else {
        // State not seen, return a default action (e.g., STAY or a random one)
        // Returning a random action here might encourage exploration in unknown states.
        return action_distribution(rng); // Return random action index
        // return static_cast<int>(Action::STAY); // Or just default to STAY
    }
}

// Choose action using epsilon-greedy strategy
Action QLearningAgent::choose_action(const State& current_state) {
    // Generate a random number for exploration check
    double random_value = exploration_distribution(rng);

    int chosen_action_index;

    if (random_value < epsilon) {
        // Explore: Choose a random action
        chosen_action_index = action_distribution(rng);
    } else {
        // Exploit: Choose the best known action for the current state
        chosen_action_index = get_best_action_index(current_state);
    }

    return static_cast<Action>(chosen_action_index);
}

// Update Q-value using the Q-learning formula
void QLearningAgent::update_q_value(const State& old_state, Action action, double reward, const State& new_state) {
    // Get the Q-value for the action taken in the old state
    int action_index = static_cast<int>(action);
    double old_q_value = get_q_value(old_state, action_index);

    // Find the maximum Q-value for the resulting new state (best possible future reward)
    double max_future_q = 0.0;
    auto it_new = q_table.find(new_state);
    if (it_new != q_table.end()) {
         max_future_q = *std::max_element(it_new->second.begin(), it_new->second.end());
    }
    // If new_state is not in q_table, max_future_q remains 0.0, which is correct.

    // Apply the Q-learning update rule:
    // Q(s, a) = Q(s, a) + alpha * [reward + gamma * max Q(s', a') - Q(s, a)]
    double new_q_value = old_q_value + alpha * (reward + gamma * max_future_q - old_q_value);

    // Update the Q-table. If old_state is not present, it will be inserted.
    q_table[old_state][action_index] = new_q_value;

    // Optional: Ensure the new state exists in the table, initializing its Q-values if not.
    // This helps if get_best_action_index relies on the state existing.
    if (q_table.find(new_state) == q_table.end()) {
        q_table[new_state] = {0.0, 0.0, 0.0}; // Initialize Q-values for the new state
    }
}

// --- Persistence ---

bool QLearningAgent::save_q_table(const std::string& filename) const {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file for saving Q-table: " << filename << std::endl;
        return false;
    }

    // Simple text format: state_components action_q_values
    for (const auto& pair : q_table) {
        const State& s = pair.first;
        const auto& q_values = pair.second;
        outfile << s.ball_x_grid << " " << s.ball_y_grid << " "
                << s.ball_vx_category << " " << s.ball_vy_category << " "
                << s.cpu_paddle_y_grid << " " << s.player_paddle_y_grid << " "
                << q_values[0] << " " << q_values[1] << " " << q_values[2] << "\n";
    }

    outfile.close();
    std::cout << "Q-table saved to " << filename << " (" << q_table.size() << " states)" << std::endl;
    return true;
}

bool QLearningAgent::load_q_table(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file for loading Q-table: " << filename << std::endl;
        return false; // Indicate failure, maybe start with empty table
    }

    q_table.clear(); // Clear existing table before loading
    std::string line;
    int lines_read = 0;
    int errors = 0;

    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        State s;
        std::array<double, NUM_ACTIONS> q_values;

        if (ss >> s.ball_x_grid >> s.ball_y_grid
               >> s.ball_vx_category >> s.ball_vy_category
               >> s.cpu_paddle_y_grid >> s.player_paddle_y_grid
               >> q_values[0] >> q_values[1] >> q_values[2])
        {
            q_table[s] = q_values;
            lines_read++;
        } else {
            std::cerr << "Warning: Could not parse line in Q-table file: " << line << std::endl;
            errors++;
        }
    }

    infile.close();
    std::cout << "Q-table loaded from " << filename << " (" << lines_read << " states loaded, " << errors << " errors)" << std::endl;
    return true;
}
