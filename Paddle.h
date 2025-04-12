#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle {
private:
    sf::RectangleShape shape; // The visual representation
    sf::Vector2f position;    // Current position (top-left corner)
    float speed;              // Movement speed in pixels per second
    sf::Vector2u windowBounds; // To keep the paddle within the screen

public:
    // Constructor
    Paddle(float startX, float startY, float width, float height, float paddleSpeed, const sf::Vector2u& bounds);

    // Move the paddle up or down
    void moveUp(float dt);    // dt is delta time for frame-independent movement
    void moveDown(float dt);

    // Set the paddle's vertical position directly
    void setPosition(float x, float y);

    // Get the current position
    sf::Vector2f getPosition() const;

    // Get the global bounds of the paddle shape (for collision detection)
    sf::FloatRect getGlobalBounds() const;

    // Get the shape to draw it
    const sf::RectangleShape& getShape() const;

    // Update logic (currently just ensures bounds, could add more later)
    void update(float dt);
};

#endif // PONG_PADDLE_H
