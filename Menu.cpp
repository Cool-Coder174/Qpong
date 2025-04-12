#include "Menu.h"
#include <iostream> // For error messages

// Helper to setup text
void Menu::setupText(sf::Text& text, const std::string& str, float yPos) {
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setString(str);
    text.setCharacterSize(30); // Adjust size as needed
    // Center text horizontally
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(menuItems[0].getPosition().x, yPos)); // Use x position of first item for alignment
}


// Constructor
Menu::Menu(float width, float height, const std::vector<std::string>& items, const std::string& title)
    : selectedItemIndex(0)
{
    // --- Font Loading ---
    // IMPORTANT: You MUST have a font file (e.g., arial.ttf) accessible.
    // Place it in the same directory as the executable or provide the full path.
    if (!font.loadFromFile("arial.ttf")) { // Try loading a common font
         // Try another common system font path (adjust for your OS if needed)
        if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) { // macOS specific path
             std::cerr << "Error loading font! Make sure 'arial.ttf' or a system Arial is available." << std::endl;
             // Handle error - maybe exit or use a default SFML font (which doesn't exist)
             // For now, we'll proceed, but text won't render.
        }
    }

    float startY = height / (items.size() + 2.0f); // Start position for items
    float stepY = 60.0f; // Vertical spacing between items

    // --- Setup Title (if provided) ---
    if (!title.empty()) {
        titleText.setFont(font);
        titleText.setFillColor(sf::Color::Cyan); // Different color for title
        titleText.setString(title);
        titleText.setCharacterSize(40); // Larger size for title
        sf::FloatRect titleRect = titleText.getLocalBounds();
        titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
        titleText.setPosition(sf::Vector2f(width / 2.0f, startY)); // Position title above items
        startY += stepY * 1.5f; // Add extra space after title
    }


    // --- Setup Menu Items ---
    menuItems.resize(items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        menuItems[i].setFont(font);
        menuItems[i].setFillColor(sf::Color::White);
        menuItems[i].setString(items[i]);
        menuItems[i].setCharacterSize(30); // Adjust size as needed

        // Center text horizontally
        sf::FloatRect textRect = menuItems[i].getLocalBounds();
        menuItems[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        menuItems[i].setPosition(sf::Vector2f(width / 2.0f, startY + i * stepY));
    }

    // Highlight the first item initially
    if (!menuItems.empty()) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red); // Highlight color
    }
}

// Draw the menu
void Menu::draw(sf::RenderWindow& window) {
    // Draw title if it exists
    if (!titleText.getString().isEmpty()) {
        window.draw(titleText);
    }
    // Draw menu items
    for (const auto& item : menuItems) {
        window.draw(item);
    }
}

// Move selection up
void Menu::moveUp() {
    if (selectedItemIndex > 0 && !menuItems.empty()) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White); // Deselect old
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);   // Select new
    }
}

// Move selection down
void Menu::moveDown() {
     if (!menuItems.empty() && selectedItemIndex < menuItems.size() - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White); // Deselect old
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);   // Select new
    }
}

// Get selected item index
int Menu::getSelectedItemIndex() const {
    return selectedItemIndex;
}

// Set item text (useful for options menu)
void Menu::setItemText(int index, const std::string& text) {
     if (index >= 0 && index < menuItems.size()) {
         menuItems[index].setString(text);
         // Recenter text after changing string
         sf::FloatRect textRect = menuItems[index].getLocalBounds();
         menuItems[index].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
         // Keep original position:
         // menuItems[index].setPosition(menuItems[index].getPosition()); // Position should already be correct
     }
}
