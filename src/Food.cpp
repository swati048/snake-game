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
    int startX = wallsEnabled ? WALL_THICKNESS : 0;
    int startY = wallsEnabled ? static_cast<int>(topBarHeight + WALL_THICKNESS) : static_cast<int>(topBarHeight);
    int endX   = wallsEnabled ? windowWidth - WALL_THICKNESS : windowWidth;
    int endY   = wallsEnabled ? windowHeight - WALL_THICKNESS : windowHeight;

    int gridX = (endX - startX) / static_cast<int>(blockSize);
    int gridY = (endY - startY) / static_cast<int>(blockSize);

    sf::Vector2f newPos;
    bool onSnake;
    
    do {
        onSnake = false;
        newPos.x = startX + static_cast<float>((std::rand() % gridX) * static_cast<int>(blockSize));
        newPos.y = startY + static_cast<float>((std::rand() % gridY) * static_cast<int>(blockSize));

        for (const auto& pos : snakePositions) {
            if (pos == newPos) {
                onSnake = true;
                break;
            }
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
