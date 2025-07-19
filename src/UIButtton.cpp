#include "UIButton.hpp"

UIButton::UIButton(const sf::Vector2f& position, const sf::Font& font, const std::string& labelText)
    : label(labelText)
{
    shape.setSize({160.f, 40.f});
    shape.setPosition(position);
    shape.setFillColor(sf::Color(30, 100, 200));
    shape.setOutlineColor(sf::Color(200, 220, 255));
    shape.setOutlineThickness(3.f);

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setString(labelText);
    centerText();
}

void UIButton::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}

void UIButton::setHover(bool isHovered) {
    shape.setFillColor(isHovered ? sf::Color(50, 130, 255) : sf::Color(30, 100, 200));
}

bool UIButton::contains(const sf::Vector2f& point) const {
    return shape.getGlobalBounds().contains(point);
}

bool UIButton::handleClick(const sf::Vector2f& point) {
    return contains(point);
}

void UIButton::setText(const std::string& newText) {
    label = newText;
    text.setString(newText);
    centerText();
}

const std::string& UIButton::getText() const {
    return label;
}

sf::Vector2f UIButton::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f UIButton::getSize() const {
    return shape.getSize();
}

sf::FloatRect UIButton::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

void UIButton::centerText() {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2, bounds.height / 2);
    text.setPosition(
        shape.getPosition().x + shape.getSize().x / 2.f,
        shape.getPosition().y + shape.getSize().y / 2.f - 5.f
    );
}
