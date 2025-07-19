#include "UIManager.hpp"
#include <sstream>
#include <iostream>
#include <cmath>

namespace {
    const float PULSE_SPEED = 1.5f;
    const float PULSE_INTENSITY = 0.08f;
    const float COLOR_PULSE_SPEED = 0.7f;
    constexpr float PI = 3.14159265f;
}

UIManager::UIManager(const sf::Vector2u& windowSize)
    : alpha(0.f), fadeSpeed(300.f), fading(false),
      pendingState(GameState::MENU), windowSize(windowSize), pulseTime(0.f)
{
    overlay.setSize(sf::Vector2f(windowSize)); 
    overlay.setFillColor(sf::Color(0, 0, 0, 0));
}

void UIManager::loadResources() {
    if (!font.loadFromFile("resources/ARIAL.TTF")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    if (!bgTexture.loadFromFile("resources/background.png")) {
        std::cerr << "Failed to load background texture!" << std::endl;
    }

    if (!menuBackgroundTexture.loadFromFile("resources/menu_background.png")) {
        std::cerr << "Failed to load menu background!" << std::endl;
    }

    if (!gameOverBackgroundTexture.loadFromFile("resources/gameOver_bg.png")) {
        std::cerr << "Failed to load game over background!" << std::endl;
    }

    gameBackground.setTexture(bgTexture);
    menuBackground.setTexture(menuBackgroundTexture);
    gameOverBackground.setTexture(gameOverBackgroundTexture);

    applyTextureScaling(menuBackground, menuBackgroundTexture);
    applyTextureScaling(gameOverBackground, gameOverBackgroundTexture);
    applyTextureScaling(gameBackground, bgTexture);

    sf::Vector2f menuBase(330.f, 200.f);  // inside the board
    // Menu Buttons
    menuButtons.emplace_back(menuBase, font, "Start Game");
    menuButtons.emplace_back(menuBase + sf::Vector2f(0.f, 60.f), font, "WallsEnabled: OFF");
    menuButtons.emplace_back(menuBase + sf::Vector2f(0.f, 120.f), font, "Instructions");

    sf::Vector2f overBase(330.f, 240.f);  // centered in box
    // Game Over Buttons
    gameOverButtons.emplace_back(overBase, font, "Restart");
    gameOverButtons.emplace_back(overBase + sf::Vector2f(0.f, 60.f), font, "Menu");
    gameOverButtons.emplace_back(overBase + sf::Vector2f(0.f, 120.f), font, "Exit");  
}

void UIManager::handleMouseHover(const sf::Vector2f& mousePos) {
    if (pendingState == GameState::MENU) {
        for (auto& btn : menuButtons)
            btn.setHover(btn.contains(mousePos));
    }
    else if (pendingState == GameState::GAME_OVER) {
        for (auto& btn : gameOverButtons)
            btn.setHover(btn.contains(mousePos));
    }
}

void UIManager::handleMouseClick(const sf::Vector2f& mousePos) {
    if (pendingState == GameState::MENU) {
        for (auto& btn : menuButtons) {
            if (btn.handleClick(mousePos)) {
                const std::string& label = btn.getText();
                if (label == "Start Game") {
                    startFade(GameState::PLAYING);
                } else if (label.find("WallsEnabled") != std::string::npos) {
                    wallsEnabled = !wallsEnabled;
                    btn.setText(wallsEnabled ? "WallsEnabled: ON" : "WallsEnabled: OFF");
                } else if (label == "Instructions") {
                    // Add instruction screen state or logic if needed
                    std::cout << "Show instructions screen (not yet implemented)\n";
                }
            }
        }
    }
    else if (pendingState == GameState::GAME_OVER) {
        for (auto& btn : gameOverButtons) {
            if (btn.handleClick(mousePos)) {
                const std::string& label = btn.getText();
                if (label == "Restart") {
                    startFade(GameState::PLAYING);
                } else if (label == "Menu") {
                    startFade(GameState::MENU);
                } else if (label == "Exit") {
                    std::exit(0);
                }
            }
        }
    }
}

// MENU screen
void UIManager::drawMenuUI(sf::RenderWindow& window) {
    window.draw(menuBackground);
    for (const auto& btn : menuButtons)
        btn.draw(window);
}

// GAME OVER screen
void UIManager::drawGameOverUI(sf::RenderWindow& window, int score, int highScore) {
    window.draw(gameOverBackground);
    
    // [Existing score drawing code here...]

    for (const auto& btn : gameOverButtons)
        btn.draw(window);
}

void UIManager::drawGameBackground(sf::RenderWindow& window) {
    window.draw(gameBackground);
}

void UIManager::update(float deltaTime) {
    if (fading) updateFadeEffect(deltaTime);
}

void UIManager::updateFadeEffect(float deltaTime) {
    alpha += fadeSpeed * deltaTime;
    if (alpha >= 255.f) {
        alpha = 255.f;
        fading = false;
    }
    overlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
}

void UIManager::draw(sf::RenderWindow& window) {
    if (fading)
        window.draw(overlay);
}

bool UIManager::isWallsEnabled() const { 
    return wallsEnabled;
}

void UIManager::startFade(GameState target) {
    fading = true;
    alpha = 0.f;
    pendingState = target;
}

bool UIManager::isFading() const {
    return fading;
}

GameState UIManager::getPendingState() const {
    return pendingState;
}

void UIManager::applyTextureScaling(sf::Sprite& sprite, sf::Texture& texture) {
    sprite.setTexture(texture);
    if (texture.getSize().x == 1 && texture.getSize().y == 1) {
        sprite.setScale(windowSize.x, windowSize.y);
    } else {
        float scale = std::max(
            windowSize.x / static_cast<float>(texture.getSize().x),
            windowSize.y / static_cast<float>(texture.getSize().y)
        );
        sprite.setScale(scale, scale);
        sprite.setPosition(
            (windowSize.x - texture.getSize().x * scale) / 2,
            (windowSize.y - texture.getSize().y * scale) / 2
        );
    }
}
