#include "Paddle.h"

// Constructor implementation
Paddle::Paddle(float startX, float startY, float width, float height, float paddleSpeed, const sf::Vector2u& bounds)
    : speed(paddleSpeed), windowBounds(bounds)
{
    position.x = startX;
    position.y = startY;

    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::White); // Set color to white
    shape.setPosition(position);
    shape.setOrigin(0, 0); // Position is top-left
}

// Move the paddle up
void Paddle::moveUp(float dt) {
    position.y -= speed * dt;
    // Prevent moving off the top edge
    if (position.y < 0) {
        position.y = 0;
    }
    shape.setPosition(position);
}

// Move the paddle down
void Paddle::moveDown(float dt) {
    position.y += speed * dt;
    // Prevent moving off the bottom edge
    float bottomEdge = position.y + shape.getSize().y;
    if (bottomEdge > windowBounds.y) {
        position.y = windowBounds.y - shape.getSize().y;
    }
    shape.setPosition(position);
}

// Set position directly
void Paddle::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
     // Apply bounds checking just in case
    if (position.y < 0) {
        position.y = 0;
    }
    float bottomEdge = position.y + shape.getSize().y;
    if (bottomEdge > windowBounds.y) {
        position.y = windowBounds.y - shape.getSize().y;
    }
    shape.setPosition(position);
}


// Get the current position
sf::Vector2f Paddle::getPosition() const {
    return position;
}

// Get the global bounds
sf::FloatRect Paddle::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

// Get the shape for drawing
const sf::RectangleShape& Paddle::getShape() const {
    return shape;
}

// Update (currently just ensures position is reflected in shape, bounds check done in move/set)
void Paddle::update(float dt) {
     // Ensure shape's position matches the internal position variable
     // This might be redundant if setPosition is always used, but good practice.
     shape.setPosition(position);
}
