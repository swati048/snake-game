#pragma once
#include <SFML/Graphics.hpp>

class Score {
public:
    Score();

    void increase();
    void reset();
    int getValue() const;
    void loadHighScore();
    void saveHighScore();
    int getHighScore() const;

private:
    int score;
    int highScore;
};
