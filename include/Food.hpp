#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Food {
public:
    Food();
    void spawn(int windowWidth, int windowHeight, const std::vector<sf::Vector2f>& snakePositions, bool wallsEnabled);

    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    float getSize() const;

private:

    const float topBarHeight = 40.f;
    const float WALL_THICKNESS = 20.0f;

    float blockSize;
    static sf::Texture foodTexture;
    static bool textureLoaded;
    sf::Sprite foodSprite;

};
