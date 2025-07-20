#include "Score.hpp"
#include <fstream>
#include <iostream>

Score::Score() : score(0), highScore(0) {
    loadHighScore();
}

void Score::increase() {
    score++;
}

void Score::reset() {
    score = 0;
}

int Score::getValue() const {
    return score;
}

void Score::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        highScore = 0;
    }
}

void Score::saveHighScore() {
    if (score > highScore) {
        highScore = score;
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
            file.close();
        }
    }
}

int Score::getHighScore() const {
    return highScore;
}
