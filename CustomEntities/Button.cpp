#include "Button.h"

#include <SFML/Graphics.hpp>
#include <iostream>

namespace {
const float kIndentX = 10.f;
const float kIndentY = 5.f;
}  // namespace

Button& Button::operator=(const Button& object) {
    text = object.text;
    shape = object.shape;
    selected = object.selected;

    return *this;
}

void Button::select() {
    selected = true;
}
void Button::cancelSelection() {
    selected = false;
}
bool Button::isSelected() const {
    return selected;
}

void Button::setDefaultButtonSize() {
    float newShapeSizeX = 2 * (text.getLocalBounds().getCenter().x + kIndentX);
    float newShapeSizeY = 2 * (text.getLocalBounds().getCenter().y + kIndentY);

    shape.setSize(sf::Vector2f(newShapeSizeX, newShapeSizeY));
}

void Button::setTextInCenter() {
    float textCoordinateX = (shape.getSize().x / 2) - text.getLocalBounds().getCenter().x;
    float textCoordinateY = (shape.getSize().y / 2) - text.getLocalBounds().getCenter().y;

    text.setPosition(shape.getPosition() + sf::Vector2f(textCoordinateX, textCoordinateY));
}

void Button::setString(sf::String string) {
    text.setString(string);

    setDefaultButtonSize();
    setTextInCenter();
}

void Button::setCharacterSize(unsigned int size) {
    text.setCharacterSize(size);
    setDefaultButtonSize();
    setTextInCenter();
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
    if (size.x < shape.getSize().x || size.y < shape.getSize().y) {
        return;
    }

    shape.setSize(size);
    setTextInCenter();
}

void Button::setPosition(sf::Vector2f position) {
    shape.setPosition(position);

    setTextInCenter();
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
