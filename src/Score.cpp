#include "Score.hpp"
#include <fstream>
#include <iostream>

Score::Score() : score(0), highScore(0) {
    if (!font.loadFromFile("resources/ARIAL.TTF")) {
        std::cerr << "Error: Failed to load score font\n";
    }
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10);
    text.setString("Score: 0");
    loadHighScore();
}

void Score::increase() {
    score++;
    text.setString("Score: " + std::to_string(score));
}

void Score::draw(sf::RenderWindow& window) const {

    sf::FloatRect bounds = text.getGlobalBounds();
    sf::RectangleShape background({ bounds.width + 20.f, bounds.height + 20.f });
    background.setPosition(5.f, 5.f);
    background.setFillColor(sf::Color(0, 0, 0, 150)); // semi-transparent black
    window.draw(background);
    window.draw(text);
}

void Score::reset() {
    score = 0;
    text.setString("Score: 0");
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
