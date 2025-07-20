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

    if (!font.loadFromFile("resources/CherryBombOne-Regular.ttf")) {
        std::cerr << "Failed to load Cherry Bomb font!" << std::endl;
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

    sf::Vector2f overBase(330.f, 240.f);  
    // Game Over Buttons
    gameOverButtons.emplace_back(overBase, font, "Restart");
    gameOverButtons.emplace_back(overBase + sf::Vector2f(0.f, 60.f), font, "Menu");
    gameOverButtons.emplace_back(overBase + sf::Vector2f(0.f, 120.f), font, "Exit");  

    for (auto& btn : menuButtons)
        btn.centerHorizontally(windowSize.x);  // Center across entire window

    for (auto& btn : gameOverButtons)
        btn.centerHorizontally(windowSize.x);  
    
    sf::Vector2f instrBase(330.f, 460.f);  // Back button at bottom of board
    instructionsButtons.emplace_back(instrBase, font, "Back");
    instructionsButtons[0].centerHorizontally(windowSize.x);
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
    else if (pendingState == GameState::INSTRUCTIONS) {
        for (auto& btn : instructionsButtons) {
            btn.setHover(btn.contains(mousePos));
        }
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
                    startFade(GameState::INSTRUCTIONS); 
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
    else if (pendingState == GameState::INSTRUCTIONS) {
        for (auto& btn : instructionsButtons) {
            if (btn.handleClick(mousePos)) {
                if (btn.getText() == "Back") {
                    startFade(GameState::MENU);
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
    for (const auto& btn : gameOverButtons)
        btn.draw(window);
}

void UIManager::drawInstructionsUI(sf::RenderWindow& window) {

    sf::Vector2f size(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    sf::RectangleShape background(size);
    background.setFillColor(sf::Color(50, 50, 50)); // dark gray background
    window.draw(background);

    // Static text block
    sf::Text title("Instructions", font, 36);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::Cyan);
    title.setPosition(windowSize.x / 2.f - title.getGlobalBounds().width / 2.f, 100);
    window.draw(title);

    std::string textBlock =
        "- Use Arrow Keys to move the snake\n"
        "- Eat the red apple to grow\n"
        "- Avoid crashing into yourself or walls\n"
        "- Toggle wall enable in menu\n"
        "- Press 'P' to pause during gameplay";

    sf::Text instructions(textBlock, font, 22);
    instructions.setFillColor(sf::Color::Cyan);
    instructions.setPosition(100, 180);  // You can center or align as needed
    window.draw(instructions);

    // Draw "Back" button
    for (const auto& btn : instructionsButtons)
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
