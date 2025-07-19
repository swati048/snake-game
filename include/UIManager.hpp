#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.hpp"
#include "UIButton.hpp"

class UIManager {
public:
    UIManager(const sf::Vector2u& windowSize);

    void loadResources();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    void startFade(GameState target);
    bool isFading() const;
    GameState getPendingState() const;

    void drawGameOverUI(sf::RenderWindow& window, int score, int highScore);
    void drawMenuUI(sf::RenderWindow& window);
    void drawGameBackground(sf::RenderWindow& window);

    void applyTextureScaling(sf::Sprite& sprite, sf::Texture& texture);

    void updateFadeEffect(float deltaTime);
    bool isWallsEnabled() const;
    
    void handleMouseHover(const sf::Vector2f& mousePos);
    void handleMouseClick(const sf::Vector2f& mousePos);

private:
    sf::RectangleShape overlay;
    float alpha;
    float fadeSpeed;
    float pulseTime;
    bool fading;
    GameState pendingState;

    std::vector<UIButton> menuButtons;
    std::vector<UIButton> gameOverButtons;

    bool wallsEnabled = false; 
    sf::Font font;
    sf::Vector2u windowSize;
    
    sf::Texture bgTexture;
    sf::Sprite gameBackground;
    sf::Texture menuBackgroundTexture; 
    sf::Sprite menuBackground;
    sf::Texture gameOverBackgroundTexture;
    sf::Sprite gameOverBackground;
};
