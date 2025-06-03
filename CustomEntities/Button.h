#pragma once

#include <SFML/Graphics.hpp>

class Button {
 private:
    sf::Text text;
    sf::RectangleShape shape;
    bool selected;

    void setDefaultButtonSize();
    void setTextInCenter();

 public:
    Button() = delete;
    Button(const sf::Font& font) : text(font), shape(), selected(false) {}
    Button(const Button& object) : text(object.text), shape(object.shape), selected(object.selected) {}
    Button& operator=(const Button& object);

    void select();
    void cancelSelection();
    bool isSelected() const;

    void setString(sf::String string);
    void setCharacterSize(unsigned int size);

    void setTextFillColor(sf::Color color);
    void setTextOutlineColor(sf::Color color);
    void setTextOutlineThickess(float thickness);

    sf::String getString() const;
    unsigned int getCharacterSize() const;

    sf::Color getTextFillColor() const;
    sf::Color getTextOutlineColor() const;
    float getTextOutlineThickess() const;

    void setSize(sf::Vector2f size);
    void setPosition(sf::Vector2f position);

    void setFillColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setOutlineThickess(float thickness);

    void setTexture(const sf::Texture* texture);

    sf::Vector2f getSize() const;
    sf::Vector2f getPosition() const;

    sf::Color getFillColor() const;
    sf::Color getOutlineColor() const;
    float getOutlineThickess() const;
    const sf::Texture* getTexture() const;

    sf::FloatRect getGlobalBounds() const;
    sf::FloatRect getLocalBounds() const;

    void draw(sf::RenderWindow& window);
    void draw(sf::RenderTexture& texture);
};
