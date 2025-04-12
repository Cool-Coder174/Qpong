#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "QLearningAgent.h"
#include "Menu.h"
#include "State.h"
#include <memory> // For std::unique_ptr

// Define the different states the game can be in
enum class GameState {
    MainMenu,
    OptionsMenu,
    Playing,
    Paused,
    GameOver
};

class Game {
private:
    // --- Core SFML Objects ---
    sf::RenderWindow window;
    sf::Vector2u windowSize;
    sf::Font font; // Font for scores and messages
    sf::Text scoreTextPlayer;
    sf::Text scoreTextCPU;
    sf::Text messageText; // For Pause/Game Over messages
    sf::RectangleShape centerLine; // Dashed line effect will be drawn manually

    // --- Game Objects ---
    std::unique_ptr<Paddle> playerPaddle;
    std::unique_ptr<Paddle> cpuPaddle;
    std::unique_ptr<Ball> ball;

    // --- AI ---
    QLearningAgent aiAgent;
    State previousAiState; // Store the state before the AI's last action
    Action lastAiAction;   // Store the last action the AI took
    bool aiStateInitialized; // Flag to check if previousAiState is valid
    DifficultyLevel currentDifficulty; // Store the selected difficulty

    // --- Game State & Logic ---
    GameState currentState;
    int playerScore;
    int cpuScore;
    const int startingScore = 10; // Score starts at 10 and decrements
    const int scoreToWin = 0;     // Game ends when score reaches 0

    // --- Menus ---
    std::unique_ptr<Menu> mainMenu;
    std::unique_ptr<Menu> optionsMenu;
    std::unique_ptr<Menu> pauseMenu;
    std::unique_ptr<Menu> gameOverMenu;


    // --- Private Helper Methods ---
    void processEvents();       // Handle window events and user input
    void update(sf::Time dt);   // Update game logic (movement, AI, collisions)
    void render();              // Draw everything to the window

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed); // Handle key presses/releases
    void handleMenuInput(sf::Keyboard::Key key); // Handle input specific to menus

    void updatePlaying(sf::Time dt); // Update logic specific to the Playing state
    void updateAI(sf::Time dt);      // Update the AI's decision and paddle movement

    void resetGame();           // Reset scores, paddles, ball
    void setupText();           // Initialize score and message text objects
    void setupMenus();          // Initialize menu objects
    void updateScoreDisplay();  // Update the score text strings
    void drawCenterLine();      // Draw the dashed center line

    // AI State Conversion
    State getCurrentStateForAI() const; // Convert current game situation to a discrete AI State

    // AI Reward Calculation
    double calculateReward(int scoreEvent, bool cpuHitBall, bool cpuMovedUnnecessarily) const;


public:
    // Constructor
    Game();

    // Main game loop runner
    void run();

    // Destructor (optional, unique_ptr handles cleanup)
    ~Game() = default;
};

#endif // PONG_GAME_H
