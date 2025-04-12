#ifndef PONG_STATE_H
#define PONG_STATE_H

#include <cstddef> // For size_t
#include <functional> // For std::hash

// --- Constants for Discretization ---
// Adjust these values based on your game window size and desired granularity
const int GRID_X_DIVISIONS = 10; // How many horizontal sections for ball position
const int GRID_Y_DIVISIONS = 10; // How many vertical sections for ball position
const int PADDLE_Y_DIVISIONS = 10; // How many vertical sections for paddle position

// Represents a discrete state of the game for the Q-learning agent.
struct State {
    // Discretized ball position (indices into the grid)
    int ball_x_grid = 0;
    int ball_y_grid = 0;

    // Discretized ball velocity category
    // -1: Left, 1: Right (or stationary if needed)
    int ball_vx_category = 0;
    // -1: Up, 1: Down (or stationary if needed)
    int ball_vy_category = 0;

    // Discretized CPU paddle position (index into vertical divisions)
    int cpu_paddle_y_grid = 0;
    // Discretized Player paddle position (index into vertical divisions)
    int player_paddle_y_grid = 0;

    // Equality operator: Needed for using State as a key in std::unordered_map.
    bool operator==(const State& other) const {
        return ball_x_grid == other.ball_x_grid &&
               ball_y_grid == other.ball_y_grid &&
               ball_vx_category == other.ball_vx_category &&
               ball_vy_category == other.ball_vy_category &&
               cpu_paddle_y_grid == other.cpu_paddle_y_grid &&
               player_paddle_y_grid == other.player_paddle_y_grid;
    }
};

// Hash function specialization for State: Needed for std::unordered_map.
// This is a simple hash combination; more robust methods exist if collisions become an issue.
namespace std {
    template <>
    struct hash<State> {
        size_t operator()(const State& s) const noexcept {
            size_t h1 = std::hash<int>{}(s.ball_x_grid);
            size_t h2 = std::hash<int>{}(s.ball_y_grid);
            size_t h3 = std::hash<int>{}(s.ball_vx_category);
            size_t h4 = std::hash<int>{}(s.ball_vy_category);
            size_t h5 = std::hash<int>{}(s.cpu_paddle_y_grid);
            size_t h6 = std::hash<int>{}(s.player_paddle_y_grid);

            // Combine hashes (using a common technique)
            size_t seed = 0;
            seed ^= h1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h4 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h5 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h6 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
} // namespace std

#endif // PONG_STATE_H
