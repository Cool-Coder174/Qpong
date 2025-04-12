#include "Game.h"
#include <iostream> // For potential startup messages

int main() {
    // Seed the random number generator used by std::rand() if any legacy code uses it.
    // std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // Note: We primarily use std::mt19937 which is seeded internally.

    std::cout << "Starting Pong AI Game..." << std::endl;

    try {
        Game pongGame; // Create the game instance
        pongGame.run(); // Start the main game loop
    } catch (const std::exception& e) {
        // Catch potential standard exceptions during initialization or runtime
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1; // Indicate failure
    } catch (...) {
        // Catch any other unknown exceptions
        std::cerr << "An unknown error occurred." << std::endl;
        return 1; // Indicate failure
    }

    std::cout << "Game exited normally." << std::endl;
    return 0; // Indicate successful execution
}
