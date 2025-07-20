#include <iostream>
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "Snake.hpp"
#include "Food.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "UIManager.hpp"

namespace {
    const float SNAKE_SPEED = 20.0f;
    const float MIN_FRAME_TIME = 1.0f/60.0f;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Game");
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(time(nullptr)));
    sf::Clock clock;

    GameState gameState = GameState::MENU;
    Game game(SNAKE_SPEED, window.getSize());
    game.setupWalls(window.getSize());

    UIManager uiManager(window.getSize());
    uiManager.loadResources();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                window.close();

            // Mouse move = update hover
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                uiManager.handleMouseHover(mousePos);
            }
            // Mouse click = trigger button
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                uiManager.handleMouseClick(mousePos);
            }
        
            // Key events
            if (event.type == sf::Event::KeyPressed) {
                if (gameState == GameState::MENU && event.key.code == sf::Keyboard::Enter) {
                    uiManager.startFade(GameState::PLAYING);
                }
                else if (gameState == GameState::PLAYING) {
                    game.handleInput(event.key.code);
                }
                else if (gameState == GameState::GAME_OVER) {
                    if (event.key.code == sf::Keyboard::R) {
                        uiManager.startFade(GameState::PLAYING);
                    }
                    else if (event.key.code == sf::Keyboard::M) {
                        uiManager.startFade(GameState::MENU);
                    }
                }
            }

        }

        float deltaTime = clock.restart().asSeconds();
       
        if (gameState == GameState::PLAYING && !uiManager.isFading()) {
            game.update(deltaTime, window.getSize().x, window.getSize().y, uiManager.isWallsEnabled());
            if (game.isGameOver()) {
                uiManager.startFade(GameState::GAME_OVER);
            }
        }
        uiManager.update(deltaTime);
        // Update gameState after fade completes
        if (!uiManager.isFading() && gameState != uiManager.getPendingState()) {
            gameState = uiManager.getPendingState();

            if (gameState == GameState::PLAYING) {
                game.reset(window.getSize().x, window.getSize().y);
            }
        }

        // DRAW
        window.clear();

        if (gameState == GameState::MENU) {
            uiManager.drawMenuUI(window);
        } 
        else if (gameState == GameState::PLAYING) {
            uiManager.drawGameBackground(window);
            game.draw(window);
        } 
        else if (gameState == GameState::GAME_OVER) {
            uiManager.drawGameOverUI(window, game.getScore(), game.getHighScore());
        }
        else if (gameState == GameState::INSTRUCTIONS) {
            uiManager.drawInstructionsUI(window);
        }

        uiManager.draw(window);
        window.display();
    }

    return 0;
}
