#ifndef PONG_MENU_H
#define PONG_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    sf::Font font;                     // Font for menu items
    std::vector<sf::Text> menuItems;   // Text objects for each menu option
    int selectedItemIndex;             // Index of the currently selected item
    sf::Text titleText;                // Optional title for the menu

    // Helper to setup text properties
    void setupText(sf::Text& text, const std::string& str, float yPos);

public:
    // Constructor: Loads font, sets up menu items
    Menu(float width, float height, const std::vector<std::string>& items, const std::string& title = "");

    // Draw the menu to the render window
    void draw(sf::RenderWindow& window);

    // Move selection up
    void moveUp();

    // Move selection down
    void moveDown();

    // Get the index of the currently selected item
    int getSelectedItemIndex() const;

    // Set the text of a specific item (e.g., for options menu display)
    void setItemText(int index, const std::string& text);
};

#endif // PONG_MENU_H
