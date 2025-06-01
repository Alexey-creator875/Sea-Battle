#include "Button.h"
#include <iostream>
#include <SFML/Graphics.hpp>

namespace {
const float kIndentX = 10.f;
const float kIndentY = 5.f;

}

Button& Button::operator=(const Button& object) {
    text = object.text;
    shape = object.shape;

    return *this;
}

void Button::setDefaultButtonSize() {
    float newShapeSizeX = 2 * (text.getLocalBounds().getCenter().x + kIndentX);
    float newShapeSizeY = 2 * (text.getLocalBounds().getCenter().y + kIndentY);

    shape.setSize(sf::Vector2f(newShapeSizeX, newShapeSizeY));
}

void Button::setTextInCenter() {
    // std::cout << shape.getSize().x << ' ' << text.getPosition().x << ' ' << text.getLocalBounds().getCenter().x << '\n';


    float textCoordinateX = (shape.getSize().x / 2) - text.getLocalBounds().getCenter().x;
    float textCoordinateY = (shape.getSize().y / 2) - text.getLocalBounds().getCenter().y;

    // std::cout << text.getPosition().x << ' ' << text.getPosition().y << '\n';
    // std::cout << shape.getPosition().x << ' ' << shape.getPosition().y << '\n';

    // std::cout << shape.getPosition().x << ' ' << shape.getPosition().y << '\n';
    // std::cout << text.getPosition().x << ' ' << text.getPosition().y << '\n';
    // std::cout << textCoordinateX << ' ' << textCoordinateY << '\n';

    text.setPosition(shape.getPosition() + sf::Vector2f(textCoordinateX, textCoordinateY));

    // std::cout << shape.getPosition().x << ' ' << shape.getPosition().y << '\n';
    // std::cout << text.getPosition().x << ' ' << text.getPosition().y << '\n';
}


void Button::setString(sf::String string) {
    text.setString(string);

    setDefaultButtonSize();
    setTextInCenter();
}

void Button::setCharacterSize(unsigned int size) {
    text.setCharacterSize(size);

    // std::cout << shape.getSize().x << ' ' << shape.getSize().y << '\n';
    // std::cout << (2 * text.getLocalBounds().getCenter().x) << ' ' << (2 * text.getLocalBounds().getCenter().y) << '\n';


    setDefaultButtonSize();

    // std::cout << shape.getSize().x << ' ' << shape.getSize().y << '\n';
    // std::cout << (2 * text.getLocalBounds().getCenter().x) << ' ' << (2 * text.getLocalBounds().getCenter().y) << '\n';

    // std::cout << shape.getSize().x << ' ' << shape.getSize().y << '\n';

    setTextInCenter();

    // std::cout << shape.getSize().x << ' ' << shape.getSize().y << '\n';
}


void Button::setTextFillColor(sf::Color color) {
    text.setFillColor(color);
}

void Button::setTextOutlineColor(sf::Color color) {
    text.setOutlineColor(color);
}

void Button::setTextOutlineThickess(float thickness) {
    text.setOutlineThickness(thickness);
}

sf::String Button::getString() const {
    return text.getString();
}

unsigned int Button::getCharacterSize() const {
    return text.getCharacterSize();
}


sf::Color Button::getTextFillColor() const {
    return text.getFillColor();
}

sf::Color Button::getTextOutlineColor() const {
    return text.getOutlineColor();
}

float Button::getTextOutlineThickess() const {
    return text.getOutlineThickness();
}


void Button::setSize(sf::Vector2f size) {
    std::cout << shape.getSize().x << ' ' << shape.getSize().y << '\n';
    std::cout << size.x << ' ' << size.y << '\n';

    if (size.x < shape.getSize().x || size.y < shape.getSize().y) {
        return;
    }

    // std::cout << shape.getSize().x << ' ' << shape.getSize().y << '\n';
    // std::cout << size.x << ' ' << size.y << '\n';

    shape.setSize(size);
    setTextInCenter();

    // std::cout << shape.getSize().x << ' ' << shape.getSize().y << '\n';
    // std::cout << (2 * text.getLocalBounds().getCenter().x) << ' ' << (2 * text.getLocalBounds().getCenter().y) << '\n';
    // std::cout << text.getPosition().x << ' ' << text.getPosition().y << '\n';
    // std::cout << shape.getPosition().x << ' ' << shape.getPosition().y << '\n';

}

void Button::setPosition(sf::Vector2f position) {
    // std::cout << "Set position:\n";
    // std::cout << position.x << ' ' << position.y << '\n';
    // std::cout << shape.getPosition().x << ' ' << shape.getPosition().y << '\n';
    // std::cout << text.getPosition().x << ' ' << text.getPosition().y << '\n';

    shape.setPosition(position);

    // std::cout << shape.getPosition().x << ' ' << shape.getPosition().y << '\n';
    // std::cout << text.getPosition().x << ' ' << text.getPosition().y << '\n';

    setTextInCenter();
    
    // std::cout << shape.getPosition().x << ' ' << shape.getPosition().y << '\n';
    // std::cout << text.getPosition().x << ' ' << text.getPosition().y << '\n';
}

void Button::setFillColor(sf::Color color) {
    shape.setFillColor(color);
}

void Button::setOutlineColor(sf::Color color) {
    shape.setOutlineColor(color);
}

void Button::setOutlineThickess(float thickness) {
    shape.setOutlineThickness(thickness);
}

void Button::setTexture(const sf::Texture* texture) {
    shape.setTexture(texture);
}


sf::Vector2f Button::getSize() const {
    return shape.getSize();
}

sf::Vector2f Button::getPosition() const {
    return shape.getPosition();
}

sf::Color Button::getFillColor() const {
    return shape.getFillColor();
}

sf::Color Button::getOutlineColor() const {
    return shape.getOutlineColor();
}

float Button::getOutlineThickess() const {
    return shape.getOutlineThickness();
}

const sf::Texture* Button::getTexture() const {
    return shape.getTexture();
}

sf::FloatRect Button::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

sf::FloatRect Button::getLocalBounds() const {
    return shape.getLocalBounds();
}


void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

void Button::draw(sf::RenderTexture& texture) {
    texture.draw(shape);
    texture.draw(text);
}
