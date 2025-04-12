#ifndef PONG_QLEARNINGAGENT_H
#define PONG_QLEARNINGAGENT_H

#include "State.h"
#include <unordered_map>
#include <vector>
#include <array>
#include <random>
#include <string> // For saving/loading

// Define the possible actions the agent can take.
enum class Action {
    STAY = 0,
    UP = 1,
    DOWN = 2
};

// Number of possible actions.
const int NUM_ACTIONS = 3;

// Difficulty levels for the AI.
enum class DifficultyLevel {
    EASY,
    MEDIUM, // Default or an intermediate setting
    HARD
};

class QLearningAgent {
private:
    // The Q-table mapping State to an array of Q-values for each action.
    std::unordered_map<State, std::array<double, NUM_ACTIONS>> q_table;

    // Learning parameters
    double alpha;   // Learning rate (how much new information overrides old)
    double gamma;   // Discount factor (importance of future rewards)
    double epsilon; // Exploration rate (probability of choosing a random action)

    // Random number generation for exploration
    std::mt19937 rng; // Mersenne Twister random number generator
    std::uniform_real_distribution<double> exploration_distribution; // For epsilon check
    std::uniform_int_distribution<int> action_distribution; // For choosing random action

    // Helper function to get the Q-value for a given state and action.
    // Returns 0.0 if the state or state-action pair hasn't been seen yet.
    double get_q_value(const State& state, int action_index) const;

    // Helper function to find the action with the highest Q-value for a given state.
    // Returns the index of the best action. Handles ties arbitrarily (e.g., first max).
    int get_best_action_index(const State& state) const;

public:
    // Constructor: Initializes parameters and random number generator.
    QLearningAgent();

    // Sets the AI difficulty by adjusting learning parameters.
    void set_difficulty(DifficultyLevel level);

    // Chooses an action based on the current state using the epsilon-greedy strategy.
    // Explores (random action) with probability epsilon, otherwise exploits (best known action).
    Action choose_action(const State& current_state);

    // Updates the Q-value for the state-action pair that led from old_state to new_state.
    // This is the core Q-learning update rule.
    void update_q_value(const State& old_state, Action action, double reward, const State& new_state);

    // --- Optional: Persistence ---
    // Saves the current Q-table to a file.
    bool save_q_table(const std::string& filename) const;
    // Loads a Q-table from a file.
    bool load_q_table(const std::string& filename);

    // --- Getters for parameters (optional, for debugging/display) ---
    double get_alpha() const { return alpha; }
    double get_gamma() const { return gamma; }
    double get_epsilon() const { return epsilon; }

    // Get the number of states explored (size of the Q-table)
    size_t get_explored_state_count() const { return q_table.size(); }
};

#endif // PONG_QLEARNINGAGENT_H
