#pragma once
#include <SFML/Graphics.hpp>

class Score {
public:
    Score();

    void increase();
    void draw(sf::RenderWindow& window) const;
    void reset();
    int getValue() const;
    void loadHighScore();
    void saveHighScore();
    int getHighScore() const;

private:
    int score;
    sf::Font font;
    sf::Text text;
    int highScore;
};
