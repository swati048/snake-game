#include "Game.hpp"
#include <iostream>

Game::Game(float speed, const sf::Vector2u& windowSize)
    : speed(speed), baseMoveDelay(0.2f), speedMultiplier(1.0f),
      moveDelay(baseMoveDelay), timer(0.f),
      gameOver(false), paused(false), windowSize(windowSize) {

    if (!font.loadFromFile("resources/ARIAL.TTF")) {
        std::cerr << "Failed to load font!\n";
    }
    reset(windowSize.x, windowSize.y);
}

void Game::reset(int windowWidth, int windowHeight) {
    snake.reset();
    snake.setDirection({speed, 0});
    food.spawn(windowWidth, windowHeight, snake.getBodyPositions());
    score.reset();
    gameOver = false;
    paused = false;
    speedMultiplier = 1.0f;
    moveDelay = baseMoveDelay;
    timer = 0.0f;
}

void Game::update(float deltaTime, int windowWidth, int windowHeight, bool wallsEnabled) {
    if (gameOver || paused) return;
    this->wallsEnabled = wallsEnabled; 

    timer += deltaTime;
    if (timer >= moveDelay) {
        timer = 0.f;
        snake.update(windowWidth, windowHeight, wallsEnabled);

        if (snake.checkCollisionWithFood(food.getPosition(), food.getSize())) {
            snake.shouldGrow = true;
            food.spawn(windowWidth, windowHeight, snake.getBodyPositions());
            score.increase();
            score.saveHighScore();

            if (score.getValue() % 5 == 0) {
                speedMultiplier += 0.1f;
                moveDelay = baseMoveDelay / speedMultiplier;
            }
        }

        if ((wallsEnabled && checkWallCollision()) || snake.hasCollidedWithSelf()) 
        {
            gameOver = true;
            std::cout << "Game Over! Collision Detected.\n";
        }
    }
}

void Game::draw(sf::RenderWindow& window) {
    // Draw walls first
    if (wallsEnabled) {
        setupWalls(windowSize);
        for (const auto& wall : walls) {
            window.draw(wall);
        }
    }
    food.draw(window);
    snake.draw(window);
    score.draw(window);

    if (paused) {
        sf::Text pausedText("Paused", font, 32);
        pausedText.setFillColor(sf::Color::Yellow);
        pausedText.setStyle(sf::Text::Bold);
        pausedText.setPosition(300, 250);
        window.draw(pausedText);  
    }
}

void Game::handleInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Up)
        snake.setDirection({0, -speed});
    else if (key == sf::Keyboard::Down)
        snake.setDirection({0, speed});
    else if (key == sf::Keyboard::Left)
        snake.setDirection({-speed, 0});
    else if (key == sf::Keyboard::Right)
        snake.setDirection({speed, 0});
    else if (key == sf::Keyboard::P)
        paused = !paused;
}

bool Game::checkWallCollision() {
    sf::FloatRect headBounds = snake.getHeadBounds();
    for (const auto& wall : walls) {
        if (headBounds.intersects(wall.getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

void Game::setupWalls(const sf::Vector2u& windowSize) {
    walls.clear();
    
    // Create 4 walls (top, bottom, left, right)
    sf::RectangleShape wall;
    wall.setFillColor(sf::Color(150, 75, 0)); // Wood-like color
    
    // Top wall
    wall.setSize(sf::Vector2f(windowSize.x, WALL_THICKNESS));
    wall.setPosition(0, 0);
    walls.push_back(wall);
    
    // Bottom wall
    wall.setPosition(0, windowSize.y - WALL_THICKNESS);
    walls.push_back(wall);
    
    // Left wall
    wall.setSize(sf::Vector2f(WALL_THICKNESS, windowSize.y));
    wall.setPosition(0, 0);
    walls.push_back(wall);
    
    // Right wall
    wall.setPosition(windowSize.x - WALL_THICKNESS, 0);
    walls.push_back(wall);
}

void Game::togglePause() {
    paused = !paused;
}

bool Game::isGameOver() const {
    return gameOver;
}

bool Game::isPaused() const {
    return paused;
}

int Game::getScore() const { 
    return score.getValue(); 
}

int Game::getHighScore() const { 
    return score.getHighScore(); 
}
