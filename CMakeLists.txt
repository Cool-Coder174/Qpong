# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(PongGame CXX)

# Set C++ standard (C++17 required for std::unordered_map hashing, etc.)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- SFML Configuration ---
# Set the path to your SFML installation directory if CMake doesn't find it automatically.
# Example: set(SFML_DIR "C:/path/to/SFML-2.6.1")
# Or ensure SFML is in your system's standard library paths or added to CMAKE_PREFIX_PATH.

# Find the SFML package components needed
# We need system, window, graphics for basic functionality.
# Add 'audio' if you plan to add sound effects later.
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# --- Source Files ---
# List all your C++ source files here
set(SOURCES
        main.cpp
        Game.cpp
        Paddle.cpp
        Ball.cpp
        QLearningAgent.cpp
        Menu.cpp
        State.cpp
)

# --- Header Files ---
# Although not strictly necessary for CMake, listing headers can help IDEs.
set(HEADERS
        Game.h
        Paddle.h
        Ball.h
        QLearningAgent.h
        Menu.h
        State.h
        State.h
        QLearningAgent.cpp
        QLearningAgent.h
        Paddle.cpp
        Paddle.h
        Ball.cpp
        Ball.h
        Menu.cpp
        Menu.h
        Game.cpp
        Game.h
)

# --- Executable ---
# Create the executable target from the source files
add_executable(PongGame ${SOURCES} ${HEADERS})

# --- Linking ---
# Link the SFML libraries to your executable
target_link_libraries(PongGame PRIVATE sfml-graphics sfml-window sfml-system)

# --- Optional: Include directories ---
# If your headers are in a separate 'include' directory, uncomment the line below:
# target_include_directories(PongGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Optional: Platform Specifics (macOS) ---
if(APPLE)
    # Add specific flags or settings for macOS if needed
    # For example, creating an App Bundle:
    # set(MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist) # You'd need to create Info.plist
    # set_target_properties(PongGame PROPERTIES MACOSX_BUNDLE TRUE)
endif()

# --- Installation (Optional) ---
# Define installation rules if you plan to distribute the game
# install(TARGETS PongGame DESTINATION bin)
# install(FILES ${HEADERS} DESTINATION include) # If headers are separate

# --- Output ---
# Print SFML version found (for debugging)
message(STATUS "Found SFML ${SFML_VERSION} in ${SFML_INCLUDE_DIR}")
message(STATUS "Linking against: ${SFML_LIBRARIES}")
