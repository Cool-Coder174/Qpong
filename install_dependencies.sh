#!/bin/bash

# Function to install dependencies on macOS
install_mac_dependencies() {
    echo "Installing dependencies for macOS..."

    # Check if Homebrew is installed
    if ! command -v brew &> /dev/null; then
        echo "Homebrew not found. Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi

    # Update Homebrew
    echo "Updating Homebrew..."
    brew update

    # Install SFML 2.5 (if required)
    echo "Installing SFML 2.5..."
    brew uninstall sfml --ignore-dependencies || true
    brew install sfml@2.5

    # Link SFML 2.5
    echo "Linking SFML 2.5..."
    brew link sfml@2.5 --force

    # Auto-detect SFMLConfig.cmake path
    echo "Searching for SFMLConfig.cmake..."
    SFML_CONFIG_PATH=$(find /usr/local /opt/homebrew -name "SFMLConfig.cmake" 2>/dev/null | head -n 1)

    if [ -n "$SFML_CONFIG_PATH" ]; then
        echo "SFMLConfig.cmake found at: $SFML_CONFIG_PATH"
        export SFML_DIR=$(dirname "$SFML_CONFIG_PATH")
    else
        echo "Error: SFMLConfig.cmake not found. Ensure SFML is installed correctly."
        exit 1
    fi

    echo "All dependencies for macOS have been installed."
}

# Function to install dependencies on Windows (via WSL)
install_windows_dependencies() {
    echo "Installing dependencies for Windows (via WSL)..."

    # Update package lists
    sudo apt update

    # Install build tools
    echo "Installing build tools..."
    sudo apt install -y build-essential cmake

    # Install SFML
    echo "Installing SFML..."
    sudo apt install -y libsfml-dev

    # Auto-detect SFMLConfig.cmake path
    echo "Searching for SFMLConfig.cmake..."
    SFML_CONFIG_PATH=$(find /usr /lib /usr/local -name "SFMLConfig.cmake" 2>/dev/null | head -n 1)

    if [ -n "$SFML_CONFIG_PATH" ]; then
        echo "SFMLConfig.cmake found at: $SFML_CONFIG_PATH"
        export SFML_DIR=$(dirname "$SFML_CONFIG_PATH")
    else
        echo "Error: SFMLConfig.cmake not found. Ensure SFML is installed correctly."
        exit 1
    fi

    echo "All dependencies for Windows (via WSL) have been installed."
}

# Detect the operating system
if [[ "$OSTYPE" == "darwin"* ]]; then
    install_mac_dependencies
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    install_windows_dependencies
else
    echo "Unsupported operating system. This script only supports macOS and Windows (via WSL)."
    exit 1
fi