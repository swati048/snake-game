#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

class Snake {
public:
    Snake();
    void update(int windowWidth, int windowHeight, bool wallsEnabled);

    void draw(sf::RenderWindow& window);
    void setDirection(const sf::Vector2f& newDir);
    
    bool checkCollisionWithFood(const sf::Vector2f& foodPos, float foodSize);

    std::vector<sf::Vector2f> getBodyPositions() const;
    void reset();
    bool shouldGrow;
    sf::FloatRect getHeadBounds() const;
    
    sf::Vector2f getNextHeadPosition() const;
    bool willCollideWithSelf(const sf::Vector2f& nextPos) const;

private:
    void init();
    const float topBarHeight = 40.f;
    std::deque<sf::Sprite> body;
    sf::Vector2f direction;
    float blockSize;
    
    bool justGrew;
    sf::Texture headTexture;
    sf::Texture bodyTexture;
};
