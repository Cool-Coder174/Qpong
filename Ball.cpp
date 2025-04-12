#include "Ball.h"
#include <cmath> // For std::sqrt, std::cos, std::sin
#include <cstdlib> // For std::abs
#include <ctime>   // For seeding random

// Constructor
Ball::Ball(float startX, float startY, float ballRadius, float initialSpeed, const sf::Vector2u& bounds)
    : radius(ballRadius), speed(initialSpeed), windowBounds(bounds),
      rng(std::random_device{}()), // Seed RNG
      angle_distribution(0, 3) // 4 initial directions (e.g., 45, 135, 225, 315 degrees)
{
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::White);
    // Set origin to the center of the circle for easier position management
    shape.setOrigin(radius, radius);
    reset(); // Set initial position and velocity
}

// Reset ball to center with random direction
void Ball::reset() {
    position.x = windowBounds.x / 2.0f;
    position.y = windowBounds.y / 2.0f;
    shape.setPosition(position);

    // Choose a random initial direction (towards one of the 4 corners)
    int direction_choice = angle_distribution(rng);
    float angle_rad;

    // Simple way to get 4 diagonal directions
    if (direction_choice == 0) angle_rad = M_PI / 4.0f;       // Down-Right
    else if (direction_choice == 1) angle_rad = 3.0f * M_PI / 4.0f; // Down-Left
    else if (direction_choice == 2) angle_rad = 5.0f * M_PI / 4.0f; // Up-Left
    else angle_rad = 7.0f * M_PI / 4.0f; // Up-Right

    velocity.x = std::cos(angle_rad) * speed;
    velocity.y = std::sin(angle_rad) * speed;
}

// Update ball position and handle wall collisions
int Ball::update(float dt) {
    position += velocity * dt;
    shape.setPosition(position);

    int score_event = 0; // 0: none, 1: player score, -1: cpu score

    // Collision with top wall
    if (position.y - radius < 0) {
        position.y = radius; // Correct position
        bounceY();
    }
    // Collision with bottom wall
    else if (position.y + radius > windowBounds.y) {
        position.y = windowBounds.y - radius; // Correct position
        bounceY();
    }

    // Collision with left wall (CPU scores)
    if (position.x - radius < 0) {
        score_event = -1; // CPU scored
        reset(); // Reset ball position and velocity
    }
    // Collision with right wall (Player scores)
    else if (position.x + radius > windowBounds.x) {
        score_event = 1; // Player scored
        reset(); // Reset ball position and velocity
    }

    return score_event;
}

// Reverse horizontal velocity
void Ball::bounceX() {
    velocity.x = -velocity.x;
    // Optional: Add a small random vertical component change to prevent boring loops
    // velocity.y += static_cast<float>(rand() % 20 - 10); // Small random change
}

// Reverse vertical velocity
void Ball::bounceY() {
    velocity.y = -velocity.y;
}

// Increase speed
void Ball::increaseSpeed(float factor) {
    velocity *= factor;
    // Update the internal speed magnitude if needed, though it's less critical now
    // speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

// Set position
void Ball::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    shape.setPosition(position);
}

// Getters
sf::Vector2f Ball::getPosition() const {
    return position;
}

sf::Vector2f Ball::getVelocity() const {
    return velocity;
}

sf::FloatRect Ball::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

const sf::CircleShape& Ball::getShape() const {
    return shape;
}

float Ball::getRadius() const {
    return radius;
}

float Ball::getCurrentSpeed() const {
     return std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}
