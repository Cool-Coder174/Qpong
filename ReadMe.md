# Pong AI Game

This is a C++ implementation of a Pong game with AI using Q-learning. The game allows you to play against an AI opponent with adjustable difficulty levels. The game is built using the SFML library for graphics and input handling.

---

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Installation](#installation)
   - [macOS](#macos)
   - [Windows (via WSL)](#windows-via-wsl)
3. [Running the Game](#running-the-game)
4. [How to Play](#how-to-play)
5. [Adjusting AI Difficulty](#adjusting-ai-difficulty)
6. [Troubleshooting](#troubleshooting)

---

## Prerequisites

Before installing and running the game, ensure you have the following installed on your system:

### General Requirements
- **C++ Compiler**: A compiler that supports C++17 or later (e.g., `g++`, `clang`).
- **CMake**: Version 3.10 or later.
- **SFML Library**: Version 2.5 or later.

### macOS
- **Homebrew**: A package manager for macOS.

### Windows (via WSL)
- **Windows Subsystem for Linux (WSL)**: Ensure WSL is installed and configured.
- **Ubuntu**: Recommended WSL distribution.

---

## Installation

### macOS

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Cool-Coder174/Qpong.git
   cd pong-ai-game
   ```

2. **Run the Dependency Installation Script**:
   Execute the provided `install_dependencies.sh` script to install SFML and other dependencies:
   ```bash
   ./install_dependencies.sh
   ```

3. **Build the Project**:
   Use CMake to configure and build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the Game**:
   After building, the executable will be located in the `build` directory. Run it using:
   ```bash
   ./PongGame
   ```

---

### Windows (via WSL)

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Cool-Coder174/Qpong.git
   cd pong-ai-game
   ```

2. **Run the Dependency Installation Script**:
   Execute the `install_dependencies.sh` script to install SFML and other dependencies:
   ```bash
   ./install_dependencies.sh
   ```

3. **Build the Project**:
   Use CMake to configure and build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the Game**:
   After building, the executable will be located in the `build` directory. Run it using:
   ```bash
   ./PongGame
   ```

---

## Running the Game

Once the game is built, you can run it by executing the `PongGame` binary. The game will open in a new window.

---

## How to Play

### Controls
- **Player Paddle**:
  - Move Up: `W`
  - Move Down: `S`
- **Pause/Resume**: `Escape`

### Objective
- Prevent the ball from passing your paddle.
- Score points by making the ball pass the AI paddle.
- The game ends when either the player or the AI reaches a score of 0.

---

## Adjusting AI Difficulty

You can adjust the AI difficulty in the **Options Menu**:
1. Navigate to the **Options** menu from the main menu.
2. Select the **Difficulty** option to cycle through:
   - **Easy**: Low exploration and slower learning.
   - **Medium**: Balanced difficulty.
   - **Hard**: High exploration and faster learning.

---

## Troubleshooting

### Common Issues
1. **SFML Not Found**:
   - Ensure SFML is installed in standard system paths or specify its location in the `CMakeLists.txt` file:
     ```cmake
     set(SFML_DIR "/path/to/SFML")
     ```

2. **Font Not Found**:
   - Ensure the `arial.ttf` font file is in the same directory as the executable or in a system font directory.

3. **Permission Denied**:
   - Ensure the `install_dependencies.sh` script has execute permissions:
     ```bash
     chmod +x install_dependencies.sh
     ```

4. **Game Crashes on Startup**:
   - Verify that all dependencies are installed correctly.
   - Check the console output for error messages.

---

Enjoy playing Pong AI! If you encounter any issues, feel free to open an issue on the repository.
