#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIButton {
public:
    UIButton(const sf::Vector2f& position, const sf::Font& font, const std::string& labelText);

    void draw(sf::RenderWindow& window) const;
    void setHover(bool isHovered);
    bool contains(const sf::Vector2f& point) const;
    bool handleClick(const sf::Vector2f& point);

    void setText(const std::string& newText);
    const std::string getText() const;

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::FloatRect getGlobalBounds() const;

    void setPosition(const sf::Vector2f& pos);
    void centerHorizontally(float totalWidth);

private:
    sf::Text text;
};
