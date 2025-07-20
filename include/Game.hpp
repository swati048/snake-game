#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.hpp"
#include "Food.hpp"
#include "Score.hpp"
#include "GameState.hpp"

class Game {
public:
    Game(float speed, const sf::Vector2u& windowSize);

    void reset(int windowWidth, int windowHeight);
    void update(float deltaTime, int windowWidth, int windowHeight, bool wallsEnabled);
    void draw(sf::RenderWindow& window);
    void handleInput(sf::Keyboard::Key key);

    void setupWalls(const sf::Vector2u& windowSize);
    bool checkWallCollision();

    void togglePause();
    bool isGameOver() const;
    bool isPaused() const;
    int getScore() const;
    int getHighScore() const;

private:
    Snake snake;
    Food food;
    Score score;

    const float topBarHeight = 40.f;
    std::vector<sf::RectangleShape> walls;
    const float WALL_THICKNESS = 20.0f;

    sf::Font font;
    sf::Vector2u windowSize;

    float speed;
    float baseMoveDelay;
    float moveDelay;
    float speedMultiplier;
    float timer;

    bool gameOver;
    bool paused;
    bool wallsEnabled;
};
