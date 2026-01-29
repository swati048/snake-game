#include "Food.hpp"
#include <cstdlib> // for std::rand
#include <ctime>   // for seeding rand
#include <iostream>

sf::Texture Food::foodTexture;
bool Food::textureLoaded = false;

Food::Food() : blockSize(20.f) {
    if (!textureLoaded) {
        if (!foodTexture.loadFromFile("resources/food.png")) {
            std::cerr << "Failed to load food.png\n";
            throw std::runtime_error("Required texture missing");
        }
        textureLoaded = true;
    }
    foodSprite.setTexture(foodTexture);
    foodSprite.setScale(
        blockSize / foodTexture.getSize().x,
        blockSize / foodTexture.getSize().y
    );
}

void Food::spawn(int windowWidth, int windowHeight, const std::vector<sf::Vector2f>& snakePositions, bool wallsEnabled) {
    // Calculate valid spawn area boundaries
    int startX = wallsEnabled ? static_cast<int>(WALL_THICKNESS + blockSize) : 0;
    int startY = wallsEnabled ? static_cast<int>(topBarHeight + WALL_THICKNESS + blockSize) : static_cast<int>(topBarHeight);
    int endX   = wallsEnabled ? static_cast<int>(windowWidth - WALL_THICKNESS - blockSize) : windowWidth;
    int endY   = wallsEnabled ? static_cast<int>(windowHeight - WALL_THICKNESS - blockSize) : windowHeight;
    
    // Calculate grid dimensions
    int gridX = (endX - startX) / static_cast<int>(blockSize);
    int gridY = (endY - startY) / static_cast<int>(blockSize);

    // Calculate maximum attempts to prevent infinite loop
    // If grid is nearly full, we need a safety mechanism
    int maxAttempts = gridX * gridY;
    int attempts = 0;

    sf::Vector2f newPos;
    bool onSnake;
    
    do {
        onSnake = false;
        // Generate random grid position
        newPos.x = startX + static_cast<float>((std::rand() % gridX) * static_cast<int>(blockSize));
        newPos.y = startY + static_cast<float>((std::rand() % gridY) * static_cast<int>(blockSize));

        // Check if position collides with snake body
        for (const auto& pos : snakePositions) {
            if (pos == newPos) {
                onSnake = true;
                break;
            }
        }

        attempts++;
        
        // Safety check: if we've tried too many times, break and use last position
        if (attempts >= maxAttempts) {
            std::cerr << "Warning: Food spawn reached max attempts. Grid may be nearly full.\n";
            break;
        }

    } while (onSnake);

    foodSprite.setPosition(newPos);
}

void Food::draw(sf::RenderWindow& window) {
    window.draw(foodSprite);
}

sf::Vector2f Food::getPosition() const {
    return foodSprite.getPosition();
}

float Food::getSize() const {
    return blockSize;
}
