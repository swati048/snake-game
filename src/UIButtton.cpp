#include "UIButton.hpp"

UIButton::UIButton(const sf::Vector2f& position, const sf::Font& font, const std::string& labelText) {
    text.setFont(font);
    text.setString(labelText);
    text.setCharacterSize(30);  
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Regular);
    text.setPosition(position);   
}

void UIButton::draw(sf::RenderWindow& window) const {
    window.draw(text);
}

void UIButton::setHover(bool isHovered) {
    if (isHovered) {
        text.setFillColor(sf::Color::Cyan);
        text.setStyle(sf::Text::Bold);  
    } else {
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Regular);
    }
}

bool UIButton::contains(const sf::Vector2f& point) const {
    return text.getGlobalBounds().contains(point);
}

bool UIButton::handleClick(const sf::Vector2f& point) {
    return contains(point);
}

void UIButton::setText(const std::string& newText) {
    text.setString(newText);
}

const std::string UIButton::getText() const {
    return text.getString();
}

sf::Vector2f UIButton::getPosition() const {
    return text.getPosition();
}

sf::Vector2f UIButton::getSize() const {
    sf::FloatRect bounds = text.getGlobalBounds();
    return { bounds.width, bounds.height };
}

sf::FloatRect UIButton::getGlobalBounds() const {
    return text.getGlobalBounds();
}

void UIButton::setPosition(const sf::Vector2f& pos) {
    text.setPosition(pos);
}

void UIButton::centerHorizontally(float totalWidth) {
    sf::FloatRect bounds = text.getGlobalBounds();
    sf::Vector2f pos = text.getPosition();
    text.setPosition((totalWidth - bounds.width) / 2.f, pos.y);
}

