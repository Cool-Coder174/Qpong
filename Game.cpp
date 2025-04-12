#include "Game.h"
#include <iostream> // For debug output
#include <cmath>    // For std::abs, std::floor
#include <string>   // For std::to_string

// --- Constants ---
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const float PADDLE_WIDTH = 15.0f;
const float PADDLE_HEIGHT = 80.0f;
const float BALL_RADIUS = 8.0f;
const float PADDLE_SPEED = 400.0f; // Pixels per second
const float BALL_INITIAL_SPEED = 300.0f; // Pixels per second
const float PADDLE_MARGIN = 20.0f; // Distance from edge

// Constructor
Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "C++ Pong AI", sf::Style::Default), // Use Default style for standard window controls
      windowSize(WINDOW_WIDTH, WINDOW_HEIGHT),
      currentState(GameState::MainMenu), // Start at the main menu
      playerScore(startingScore),
      cpuScore(startingScore),
      aiStateInitialized(false),
      currentDifficulty(DifficultyLevel::EASY) // Default difficulty
{
    window.setVerticalSyncEnabled(true); // Helps prevent screen tearing
    // Optional: Limit framerate if vsync is off or unreliable
    // window.setFramerateLimit(60);

    // --- Load Font ---
    if (!font.loadFromFile("arial.ttf")) { // Ensure arial.ttf is accessible
        if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
             std::cerr << "Error loading font! Scores and messages will not display." << std::endl;
             // Handle error appropriately
        }
    }

    // --- Initialize Game Objects ---
    playerPaddle = std::make_unique<Paddle>(
        PADDLE_MARGIN,                          // x position (left side)
        windowSize.y / 2.0f - PADDLE_HEIGHT / 2.0f, // y position (centered)
        PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED, windowSize
    );

    cpuPaddle = std::make_unique<Paddle>(
        windowSize.x - PADDLE_WIDTH - PADDLE_MARGIN, // x position (right side)
        windowSize.y / 2.0f - PADDLE_HEIGHT / 2.0f,  // y position (centered)
        PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED, windowSize // AI speed might be adjusted later
    );

    ball = std::make_unique<Ball>(
        windowSize.x / 2.0f, windowSize.y / 2.0f, // Center position
        BALL_RADIUS, BALL_INITIAL_SPEED, windowSize
    );

    // --- Setup UI Elements ---
    setupText();
    setupMenus();

    // --- AI Setup ---
    aiAgent.set_difficulty(currentDifficulty);
    // Optional: Try loading a pre-trained Q-table
    if (!aiAgent.load_q_table("pong_q_table.dat")) {
         std::cout << "No pre-trained Q-table found or error loading. Starting fresh." << std::endl;
    } else {
         std::cout << "Loaded Q-table with " << aiAgent.get_explored_state_count() << " states." << std::endl;
    }
}

// Setup text elements
void Game::setupText() {
    scoreTextPlayer.setFont(font);
    scoreTextPlayer.setCharacterSize(30);
    scoreTextPlayer.setFillColor(sf::Color::White);
    // Position left score (adjust x offset as needed)
    scoreTextPlayer.setPosition(windowSize.x * 0.25f, 20.0f);

    scoreTextCPU.setFont(font);
    scoreTextCPU.setCharacterSize(30);
    scoreTextCPU.setFillColor(sf::Color::White);
    // Position right score (adjust x offset as needed)
    scoreTextCPU.setPosition(windowSize.x * 0.75f - 50.0f, 20.0f); // Adjust x to roughly center

    messageText.setFont(font);
    messageText.setCharacterSize(40);
    messageText.setFillColor(sf::Color::White);
    // Centered - origin will be set when text is assigned
    messageText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    updateScoreDisplay(); // Set initial score text
}

// Setup menu objects
void Game::setupMenus() {
    mainMenu = std::make_unique<Menu>(windowSize.x, windowSize.y,
                                      std::vector<std::string>{"Start Game", "Options", "Exit"},
                                      "PONG AI");

    optionsMenu = std::make_unique<Menu>(windowSize.x, windowSize.y,
                                         std::vector<std::string>{"Difficulty: Easy", "Back"}, // Text updated dynamically
                                         "Options");

    pauseMenu = std::make_unique<Menu>(windowSize.x, windowSize.y,
                                       std::vector<std::string>{"Resume", "Main Menu", "Exit"},
                                       "Paused");

    gameOverMenu = std::make_unique<Menu>(windowSize.x, windowSize.y,
                                          std::vector<std::string>{"Play Again", "Main Menu", "Exit"},
                                          "Game Over"); // Title set dynamically later
}


// Update score display strings
void Game::updateScoreDisplay() {
    scoreTextPlayer.setString(std::to_string(playerScore));
    scoreTextCPU.setString(std::to_string(cpuScore));

    // Recenter score text if needed (simple centering based on assumed max width)
    // sf::FloatRect pRect = scoreTextPlayer.getLocalBounds();
    // scoreTextPlayer.setOrigin(pRect.left + pRect.width / 2.f, pRect.top);
    // scoreTextPlayer.setPosition(windowSize.x * 0.25f, 20.f);

    // sf::FloatRect cRect = scoreTextCPU.getLocalBounds();
    // scoreTextCPU.setOrigin(cRect.left + cRect.width / 2.f, cRect.top);
    // scoreTextCPU.setPosition(windowSize.x * 0.75f, 20.f);
}

// Reset game state for a new round/match
void Game::resetGame() {
    playerScore = startingScore;
    cpuScore = startingScore;
    updateScoreDisplay();
    playerPaddle->setPosition(PADDLE_MARGIN, windowSize.y / 2.0f - PADDLE_HEIGHT / 2.0f);
    cpuPaddle->setPosition(windowSize.x - PADDLE_WIDTH - PADDLE_MARGIN, windowSize.y / 2.0f - PADDLE_HEIGHT / 2.0f);
    ball->reset();
    aiStateInitialized = false; // Reset AI state tracking
    currentState = GameState::Playing; // Go directly to playing state after reset
}

// Main game loop
void Game::run() {
    sf::Clock clock; // Clock to measure delta time
    while (window.isOpen()) {
        sf::Time dt = clock.restart(); // Time elapsed since last frame
        processEvents();
        update(dt);
        render();
    }

    // Optional: Save the learned Q-table when the game closes
    if (aiAgent.save_q_table("pong_q_table.dat")) {
         std::cout << "Q-table saved successfully on exit." << std::endl;
    } else {
         std::cerr << "Failed to save Q-table on exit." << std::endl;
    }
}

// Event handling
void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                // Handle input based on game state
                if (currentState == GameState::Playing) {
                    handlePlayerInput(event.key.code, true);
                    if (event.key.code == sf::Keyboard::Escape) {
                        currentState = GameState::Paused;
                    }
                } else if (currentState == GameState::MainMenu ||
                           currentState == GameState::OptionsMenu ||
                           currentState == GameState::Paused ||
                           currentState == GameState::GameOver)
                {
                    handleMenuInput(event.key.code);
                }
                break;
             case sf::Event::KeyReleased:
                 if (currentState == GameState::Playing) {
                     handlePlayerInput(event.key.code, false); // Handle key releases if needed (e.g., stop movement)
                 }
                 break;
            default:
                break;
        }
    }
}

// Handle player input during gameplay
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
     // Continuous movement handling will be done in the update loop based on key state
     // This function could be used for single press actions if needed later.
     // For now, we only care about the Escape key press handled in processEvents.
     if (isPressed) {
         // Example: if (key == sf::Keyboard::Space) { /* do something */ }
     } else {
         // Example: if (key == sf::Keyboard::W || key == sf::Keyboard::S) { /* stop vertical movement */ }
     }
}

// Handle input for menus
void Game::handleMenuInput(sf::Keyboard::Key key) {
    Menu* currentMenu = nullptr;
    if (currentState == GameState::MainMenu) currentMenu = mainMenu.get();
    else if (currentState == GameState::OptionsMenu) currentMenu = optionsMenu.get();
    else if (currentState == GameState::Paused) currentMenu = pauseMenu.get();
    else if (currentState == GameState::GameOver) currentMenu = gameOverMenu.get();

    if (!currentMenu) return; // Should not happen

    if (key == sf::Keyboard::Up || key == sf::Keyboard::W) {
        currentMenu->moveUp();
    } else if (key == sf::Keyboard::Down || key == sf::Keyboard::S) {
        currentMenu->moveDown();
    } else if (key == sf::Keyboard::Return) { // Enter key
        int selectedIndex = currentMenu->getSelectedItemIndex();

        if (currentState == GameState::MainMenu) {
            if (selectedIndex == 0) { // Start Game
                resetGame(); // Reset scores and positions
                currentState = GameState::Playing;
            } else if (selectedIndex == 1) { // Options
                currentState = GameState::OptionsMenu;
                // Update options menu text based on current settings
                 std::string diffText = "Difficulty: ";
                 switch(currentDifficulty) {
                     case DifficultyLevel::EASY: diffText += "Easy"; break;
                     case DifficultyLevel::MEDIUM: diffText += "Medium"; break;
                     case DifficultyLevel::HARD: diffText += "Hard"; break;
                 }
                 optionsMenu->setItemText(0, diffText);

            } else if (selectedIndex == 2) { // Exit
                window.close();
            }
        } else if (currentState == GameState::OptionsMenu) {
            if (selectedIndex == 0) { // Difficulty Select
                // Cycle through difficulties
                if (currentDifficulty == DifficultyLevel::EASY) currentDifficulty = DifficultyLevel::MEDIUM;
                else if (currentDifficulty == DifficultyLevel::MEDIUM) currentDifficulty = DifficultyLevel::HARD;
                else currentDifficulty = DifficultyLevel::EASY;
                aiAgent.set_difficulty(currentDifficulty); // Apply to agent

                // Update menu text
                 std::string diffText = "Difficulty: ";
                 switch(currentDifficulty) {
                     case DifficultyLevel::EASY: diffText += "Easy"; break;
                     case DifficultyLevel::MEDIUM: diffText += "Medium"; break;
                     case DifficultyLevel::HARD: diffText += "Hard"; break;
                 }
                 optionsMenu->setItemText(0, diffText);

            } else if (selectedIndex == 1) { // Back
                currentState = GameState::MainMenu;
            }
        } else if (currentState == GameState::Paused) {
             if (selectedIndex == 0) { // Resume
                 currentState = GameState::Playing;
             } else if (selectedIndex == 1) { // Main Menu
                 currentState = GameState::MainMenu;
                 // Optional: Save Q-table here too if desired
             } else if (selectedIndex == 2) { // Exit
                 window.close();
             }
        } else if (currentState == GameState::GameOver) {
             if (selectedIndex == 0) { // Play Again
                 resetGame();
                 currentState = GameState::Playing;
             } else if (selectedIndex == 1) { // Main Menu
                 currentState = GameState::MainMenu;
             } else if (selectedIndex == 2) { // Exit
                 window.close();
             }
        }

    } else if (key == sf::Keyboard::Escape) {
        // Escape key behavior in menus
        if (currentState == GameState::OptionsMenu || currentState == GameState::Paused || currentState == GameState::GameOver) {
            currentState = GameState::MainMenu; // Go back to main menu
        }
        // In Main Menu, Escape doesn't do anything unless you want it to exit
        // else if (currentState == GameState::MainMenu) { window.close(); }
    }
}


// Update game logic based on state
void Game::update(sf::Time dt) {
    switch (currentState) {
        case GameState::Playing:
            updatePlaying(dt);
            break;
        case GameState::MainMenu:
        case GameState::OptionsMenu:
        case GameState::Paused:
        case GameState::GameOver:
            // No game logic updates needed in menu states
            break;
    }
}

// Update logic for the Playing state
void Game::updatePlaying(sf::Time dt) {
    float seconds = dt.asSeconds();

    // --- Player Movement ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        playerPaddle->moveUp(seconds);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        playerPaddle->moveDown(seconds);
    }

    // --- AI Update ---
    updateAI(dt); // Let the AI decide and move its paddle

    // --- Ball Movement & Wall Collision ---
    int scoreEvent = ball->update(seconds); // Ball updates its position and checks wall collisions

    // --- Scoring ---
    bool scored = false;
    if (scoreEvent == 1) { // Player scored
        cpuScore--;
        scored = true;
        std::cout << "Player scored! Score: P=" << playerScore << " C=" << cpuScore << std::endl;
    } else if (scoreEvent == -1) { // CPU scored
        playerScore--;
        scored = true;
         std::cout << "CPU scored! Score: P=" << playerScore << " C=" << cpuScore << std::endl;
    }

    if (scored) {
        updateScoreDisplay();
        ball->reset(); // Reset ball after score
        // If AI was involved in the score, update its Q-value for the previous state
        if (aiStateInitialized) {
             double reward = calculateReward(scoreEvent, false, false); // Calculate reward for the score event
             State nextState = getCurrentStateForAI(); // Get state after reset (might be less useful here)
             // It might be better to update based on the state *before* the score/reset.
             // Let's assume the reward applies to the action leading to the score.
             aiAgent.update_q_value(previousAiState, lastAiAction, reward, nextState);
             std::cout << "AI Q-update (Score): Reward=" << reward << std::endl;
        }
         aiStateInitialized = false; // Need a new 'previous' state after reset

        // --- Check Game Over ---
        if (playerScore <= scoreToWin || cpuScore <= scoreToWin) {
            currentState = GameState::GameOver;
            // Set Game Over message
            messageText.setString(playerScore <= scoreToWin ? "CPU Wins!" : "Player Wins!");
            sf::FloatRect textRect = messageText.getLocalBounds();
            messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            messageText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f - 50); // Position above menu

            // Update the Game Over menu title (optional)
            gameOverMenu = std::make_unique<Menu>(windowSize.x, windowSize.y,
                                      std::vector<std::string>{"Play Again", "Main Menu", "Exit"},
                                      playerScore <= scoreToWin ? "CPU Wins!" : "Player Wins!");
            return; // Exit updatePlaying early as game is over
        }
    }


    // --- Paddle Collision ---
    bool cpuHitBall = false;
    sf::FloatRect ballBounds = ball->getGlobalBounds();
    sf::FloatRect playerBounds = playerPaddle->getGlobalBounds();
    sf::FloatRect cpuBounds = cpuPaddle->getGlobalBounds();

    // Collision with player paddle
    if (ballBounds.intersects(playerBounds)) {
        // Check if ball is moving towards player (left) to prevent multi-hits
        if (ball->getVelocity().x < 0) {
            ball->bounceX();
            ball->increaseSpeed(); // Optional: Speed up ball on hit
            // Correct ball position slightly to prevent sticking
            ball->setPosition(playerBounds.left + playerBounds.width + ball->getRadius() + 1.0f, ball->getPosition().y);
             std::cout << "Player hit ball." << std::endl;
        }
    }

    // Collision with CPU paddle
    if (ballBounds.intersects(cpuBounds)) {
         // Check if ball is moving towards CPU (right)
         if (ball->getVelocity().x > 0) {
            ball->bounceX();
            ball->increaseSpeed();
            cpuHitBall = true; // Flag that the CPU successfully hit the ball
            // Correct ball position slightly
             ball->setPosition(cpuBounds.left - ball->getRadius() - 1.0f, ball->getPosition().y);
             std::cout << "CPU hit ball." << std::endl;
         }
    }

     // --- AI Learning Update (if no score occurred this frame) ---
     // We update the AI based on the consequences of its *last* action.
     if (!scored && aiStateInitialized) {
         State currentStateAI = getCurrentStateForAI();
         bool cpuMovedUnnecessarily = false; // Determine if the last move was unnecessary

         // Define "unnecessary": CPU moved when ball is moving away from its side
         if (lastAiAction != Action::STAY && ball->getVelocity().x < 0) {
              // Check if ball is sufficiently far away? Or just moving away is enough?
              // Let's start simple: penalize if moving when ball moves away.
              cpuMovedUnnecessarily = true;
              // std::cout << "CPU moved unnecessarily (ball moving left)." << std::endl;
         }

         double reward = calculateReward(0, cpuHitBall, cpuMovedUnnecessarily); // Calculate reward for this step
         aiAgent.update_q_value(previousAiState, lastAiAction, reward, currentStateAI);
         // Only print significant rewards for less spam
         if (reward != 0.0) {
             std::cout << "AI Q-update: Reward=" << reward << " (Hit:" << cpuHitBall << ", UnnecMove:" << cpuMovedUnnecessarily << ")" << std::endl;
         }
     }
     // Store current state and chosen action for the *next* frame's update
     // This happens *after* the potential Q-update for the *previous* action.
     // We get the state *before* the AI makes its next move decision.
     // previousAiState = getCurrentStateForAI(); // State before AI action
     // lastAiAction = aiAgent.choose_action(previousAiState); // AI chooses action for *next* frame
     // aiStateInitialized = true;
     // The AI update logic is moved to updateAI() for clarity.


    // --- Update Paddle Positions (ensure shape matches internal state) ---
    // This might be redundant if move/set functions always update the shape, but safe.
    playerPaddle->update(seconds);
    cpuPaddle->update(seconds);
}

// Update AI logic
void Game::updateAI(sf::Time dt) {
    float seconds = dt.asSeconds();

    // 1. Get the current state for the AI
    State currentStateAI = getCurrentStateForAI();

    // 2. If we have a valid previous state, calculate reward and update Q-table
    //    This update is for the action taken *last* frame, leading to the *current* state.
    if (aiStateInitialized) {
        // We need to know if the CPU hit the ball *as a result* of the last action.
        // This is tricky because collision happens *after* the move.
        // Let's simplify: Reward for hitting the ball is handled separately in updatePlaying.
        // Here, we focus on the penalty for unnecessary movement.

        bool cpuMovedUnnecessarily = false;
         // Check if the ball is moving away from the CPU *in the current state*
         // And if the *last action* was a move.
         if (lastAiAction != Action::STAY && currentStateAI.ball_vx_category < 0) { // Assuming -1 means moving left (away from CPU)
             cpuMovedUnnecessarily = true;
         }

        // Calculate reward (only penalty here, hit reward handled elsewhere)
        double reward = calculateReward(0, false, cpuMovedUnnecessarily);
        aiAgent.update_q_value(previousAiState, lastAiAction, reward, currentStateAI);

        if (reward != 0.0) {
             // std::cout << "AI Q-update (Movement Penalty): Reward=" << reward << std::endl;
        }
    }

    // 3. Choose the *next* action based on the *current* state
    Action chosenAction = aiAgent.choose_action(currentStateAI);

    // 4. Execute the chosen action
    if (chosenAction == Action::UP) {
        cpuPaddle->moveUp(seconds);
        // std::cout << "AI Action: UP" << std::endl;
    } else if (chosenAction == Action::DOWN) {
        cpuPaddle->moveDown(seconds);
        // std::cout << "AI Action: DOWN" << std::endl;
    } else {
        // Action::STAY - do nothing
        // std::cout << "AI Action: STAY" << std::endl;
    }

    // 5. Store the current state and chosen action for the *next* frame's update
    previousAiState = currentStateAI;
    lastAiAction = chosenAction;
    aiStateInitialized = true;
}


// Convert game state to discrete AI state representation
State Game::getCurrentStateForAI() const {
    State current;
    sf::Vector2f ballPos = ball->getPosition();
    sf::Vector2f ballVel = ball->getVelocity();
    sf::Vector2f cpuPos = cpuPaddle->getPosition();
    sf::Vector2f playerPos = playerPaddle->getPosition();

    // Discretize ball position
    current.ball_x_grid = static_cast<int>(std::floor(ballPos.x / (windowSize.x / static_cast<float>(GRID_X_DIVISIONS))));
    current.ball_y_grid = static_cast<int>(std::floor(ballPos.y / (windowSize.y / static_cast<float>(GRID_Y_DIVISIONS))));
    // Clamp grid indices to valid range
    current.ball_x_grid = std::max(0, std::min(GRID_X_DIVISIONS - 1, current.ball_x_grid));
    current.ball_y_grid = std::max(0, std::min(GRID_Y_DIVISIONS - 1, current.ball_y_grid));


    // Discretize ball velocity
    current.ball_vx_category = (ballVel.x > 0) ? 1 : ((ballVel.x < 0) ? -1 : 0);
    current.ball_vy_category = (ballVel.y > 0) ? 1 : ((ballVel.y < 0) ? -1 : 0); // 1 for down, -1 for up

    // Discretize paddle positions (using center of paddle for simplicity)
    float cpuCenterY = cpuPos.y + PADDLE_HEIGHT / 2.0f;
    float playerCenterY = playerPos.y + PADDLE_HEIGHT / 2.0f;

    current.cpu_paddle_y_grid = static_cast<int>(std::floor(cpuCenterY / (windowSize.y / static_cast<float>(PADDLE_Y_DIVISIONS))));
    current.player_paddle_y_grid = static_cast<int>(std::floor(playerCenterY / (windowSize.y / static_cast<float>(PADDLE_Y_DIVISIONS))));
     // Clamp grid indices
    current.cpu_paddle_y_grid = std::max(0, std::min(PADDLE_Y_DIVISIONS - 1, current.cpu_paddle_y_grid));
    current.player_paddle_y_grid = std::max(0, std::min(PADDLE_Y_DIVISIONS - 1, current.player_paddle_y_grid));

    return current;
}

// Calculate reward for the AI based on events
double Game::calculateReward(int scoreEvent, bool cpuHitBall, bool cpuMovedUnnecessarily) const {
    double reward = 0.0;

    // Reward for hitting the ball (positive reinforcement)
    if (cpuHitBall) {
        reward += 10.0;
    }

    // Penalty for conceding a goal (negative reinforcement)
    if (scoreEvent == -1) { // CPU conceded (player scored)
        reward -= 20.0; // Significant penalty
    }
    // Note: No direct reward for scoring, as hitting the ball leads to that.
    // Could add a small reward for player conceding (scoreEvent == 1), but might be redundant.

    // Penalty for unnecessary movement
    if (cpuMovedUnnecessarily) {
        reward -= 5.0; // Penalty specified
    }

    // Small penalty for existing? Encourages faster wins? (Optional, can be risky)
    // reward -= 0.1;

    return reward;
}


// Draw the center line
void Game::drawCenterLine() {
    // Draw a dashed line manually
    float dashHeight = 10.0f;
    float gapHeight = 5.0f;
    float xPos = windowSize.x / 2.0f - 1.0f; // Center x, width 2 pixels
    sf::RectangleShape dash(sf::Vector2f(2.0f, dashHeight));
    dash.setFillColor(sf::Color::White);

    for (float yPos = 0; yPos < windowSize.y; yPos += dashHeight + gapHeight) {
        dash.setPosition(xPos, yPos);
        window.draw(dash);
    }
}

// Render the game screen
void Game::render() {
    window.clear(sf::Color::Black); // Clear screen with black background

    // Draw elements based on game state
    switch (currentState) {
        case GameState::Playing:
        case GameState::Paused: // Draw game elements even when paused
        case GameState::GameOver: // Draw final game state behind game over menu
            drawCenterLine();
            window.draw(playerPaddle->getShape());
            window.draw(cpuPaddle->getShape());
            window.draw(ball->getShape());
            window.draw(scoreTextPlayer);
            window.draw(scoreTextCPU);

            // Show messages only in Paused/GameOver states on top of game
            if (currentState == GameState::Paused) {
                 pauseMenu->draw(window);
            } else if (currentState == GameState::GameOver) {
                 // Draw final score/message if needed (already set up)
                 // window.draw(messageText); // Draw the "Player Wins!" message
                 gameOverMenu->draw(window); // Draw the menu over the game
            }
            break;

        case GameState::MainMenu:
            mainMenu->draw(window);
            break;

        case GameState::OptionsMenu:
            optionsMenu->draw(window);
            break;
    }


    window.display(); // Update the window with the drawn elements
}
