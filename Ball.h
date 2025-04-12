#ifndef PONG_BALL_H
#define PONG_BALL_H

#include <SFML/Graphics.hpp>
#include <random> // For random initial direction

class Ball {
private:
    sf::CircleShape shape;    // Visual representation (using CircleShape)
    sf::Vector2f position;    // Center position of the ball
    sf::Vector2f velocity;    // Current velocity (pixels per second)
    float speed;              // Magnitude of the velocity vector
    float radius;             // Radius of the ball
    sf::Vector2u windowBounds; // To detect wall collisions

    // Random number generation for initial direction
    std::mt19937 rng;
    std::uniform_int_distribution<int> angle_distribution;

public:
    // Constructor
    Ball(float startX, float startY, float ballRadius, float initialSpeed, const sf::Vector2u& bounds);

    // Reset the ball to the center with a random initial direction
    void reset();

    // Update the ball's position based on velocity and handle wall collisions
    // Returns: 0 = no score, 1 = player scored (CPU missed), -1 = CPU scored (player missed)
    int update(float dt);

    // Reverse the horizontal velocity (used on paddle collision)
    void bounceX();

    // Reverse the vertical velocity (used on wall collision)
    void bounceY();

    // Increase speed slightly (optional, e.g., after paddle hit)
    void increaseSpeed(float factor = 1.05f);

    // Set position directly (e.g., for reset)
    void setPosition(float x, float y);

    // Getters
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    sf::FloatRect getGlobalBounds() const;
    const sf::CircleShape& getShape() const;
    float getRadius() const;
    float getCurrentSpeed() const; // Get the magnitude of the current velocity vector
};

#endif // PONG_BALL_H
