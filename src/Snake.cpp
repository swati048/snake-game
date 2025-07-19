#include "Snake.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

Snake::Snake() : blockSize(20.f) {
    // Load textures
    headTexture.loadFromFile("resources/snake_head.png");
    bodyTexture.loadFromFile("resources/snake_body.png");
    // Now initialize snake body
    init();
}

void Snake::init() {

    shouldGrow = false;
    justGrew = false;
    direction = {blockSize, 0};

    const int initialLength = 3;
    sf::Vector2f startPos(blockSize * 5, blockSize * 5);

    for (int i = 0; i < initialLength; ++i) {
        sf::Sprite segment;
        segment.setTexture(i == 0 ? headTexture : bodyTexture);
        segment.setPosition(startPos.x - i * blockSize, startPos.y);
        segment.setScale(
            blockSize / static_cast<float>(segment.getTexture()->getSize().x),
            blockSize / static_cast<float>(segment.getTexture()->getSize().y)
        );
        body.push_back(segment);
    }
    std::reverse(body.begin(), body.end());
}

sf::FloatRect Snake::getHeadBounds() const {
    if (body.empty()) return sf::FloatRect();
    return body.front().getGlobalBounds();
}

void Snake::reset() {
    body.clear();
    init();  // Reinitialize the snake body
}

void Snake::update(int windowWidth, int windowHeight, bool wallsEnabled) {
    if (body.empty()) return;

    sf::Sprite newHead = body.front();
    newHead.move(direction);
    newHead.setTexture(headTexture);
    newHead.setScale(
        blockSize / static_cast<float>(headTexture.getSize().x),
        blockSize / static_cast<float>(headTexture.getSize().y)
    );
    // Get and snap position to grid
    sf::Vector2f pos = newHead.getPosition();

    // Handle wrap around
    if (!wallsEnabled) {
        if (pos.x < 0) pos.x = windowWidth - blockSize;
        else if (pos.x >= windowWidth) pos.x = 0;

        if (pos.y < 0) pos.y = windowHeight - blockSize;
        else if (pos.y >= windowHeight) pos.y = 0;
    }
    // snapping block
    pos.x = std::round(pos.x / blockSize) * blockSize;
    pos.y = std::round(pos.y / blockSize) * blockSize;
    // Apply snapped position
    newHead.setPosition(pos);
    // Continue update logic
    body.push_front(newHead);

    if (!shouldGrow) {
        body.pop_back();
    } else {
        shouldGrow = false;
        justGrew = true;
    }
    // Make sure the second segment becomes body
    if (body.size() > 1) {
        body[1].setTexture(bodyTexture);
        body[1].setScale(
            blockSize / static_cast<float>(bodyTexture.getSize().x),
            blockSize / static_cast<float>(bodyTexture.getSize().y)
        );
    }
    // future-proof
    if (justGrew) justGrew = false;
}

void Snake::draw(sf::RenderWindow& window) {
    for (auto& segment : body)
        window.draw(segment);
}

bool Snake::checkCollisionWithFood(const sf::Vector2f& foodPos, float foodSize) {
    sf::FloatRect headBounds = getHeadBounds();
    sf::FloatRect foodBounds(foodPos, {foodSize, foodSize});
    return headBounds.intersects(foodBounds);
}

void Snake::setDirection(const sf::Vector2f& newDir) {
    if (direction.x + newDir.x != 0 || direction.y + newDir.y != 0) {
        direction = newDir;
    }
}

bool Snake::hasCollidedWithSelf() const {
    if (body.size() < 4) return false;
    if (justGrew) return false;

    const sf::Vector2f headPos = body.front().getPosition();
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[i].getPosition() == headPos) {
            return true;
        }
    }
    return false;
}

std::vector<sf::Vector2f> Snake::getBodyPositions() const {
    std::vector<sf::Vector2f> positions;
    for (const auto& segment : body) {
        positions.push_back(segment.getPosition());
    }
    return positions;
}

