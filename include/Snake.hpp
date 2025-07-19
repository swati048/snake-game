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
    bool hasCollidedWithSelf() const;

    std::vector<sf::Vector2f> getBodyPositions() const;
    void reset();
    bool shouldGrow;
    sf::FloatRect getHeadBounds() const;

private:
    void init();
    std::deque<sf::Sprite> body;
    sf::Vector2f direction;
    float blockSize;
    
    bool justGrew;
    sf::Color color = sf::Color::Green;

    sf::Texture headTexture;
    sf::Texture bodyTexture;
};
