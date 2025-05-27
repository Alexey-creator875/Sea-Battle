#include "SeaBattleExecutor.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <iostream>
#include <cstddef>
#include <stdexcept>

namespace {
const int kDeffaultWindowLenght = 1200;
const int kDeffaultWindowHeight = 900;

const unsigned int kMenuTitulCharacterSize = 100;
const unsigned int kMenuButtonCharacterSize = 40;

const int kMenuTitulCoordinateY = 50;
const int kMenuButtonCoordinateY = 250;


}  // namespace

namespace SeaBattleExecutor {
void Menu(sf::RenderWindow& window) {
    // загрузка картинки заднего фона в текстуру
    sf::Texture menuBackground;
    if (!menuBackground.loadFromFile("../images/background.jpg")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape backGround(sf::Vector2f(kDeffaultWindowLenght, kDeffaultWindowHeight));
    backGround.setTexture(&menuBackground);
    window.draw(backGround);



    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // создаём лейбл для названия игры
    sf::Text titul(helveticaFont, sf::String("Sea Battle"));
    titul.setFillColor(sf::Color::Blue);
    titul.setCharacterSize(kMenuTitulCharacterSize);
    titul.setOutlineThickness(3);
    titul.setOutlineColor(sf::Color::White);

    float titulCoordinateX = (kDeffaultWindowLenght / 2) - titul.getGlobalBounds().getCenter().x;
    titul.setPosition(sf::Vector2f(titulCoordinateX, kMenuTitulCoordinateY));

    window.draw(titul);

    // создаём надпись кнопки, котрую наложим поверх кнопки
    sf::Text buttonText(helveticaFont, sf::String("Exit the game"));
    buttonText.setFillColor(sf::Color::Blue);
    buttonText.setCharacterSize(kMenuButtonCharacterSize);

    float buttonCoordinateX = (kDeffaultWindowLenght / 2) - buttonText.getGlobalBounds().getCenter().x;
    buttonText.setPosition(sf::Vector2f(buttonCoordinateX, kMenuButtonCoordinateY));

    // создаём кнопку на базе RectangeShape
    sf::RectangleShape button;
    button.setPosition(buttonText.getPosition());
    float buttonSizeX = (buttonText.getGlobalBounds().getCenter().x - buttonText.getPosition().x) * 2;
    float buttonSizeY = (buttonText.getGlobalBounds().getCenter().y - buttonText.getPosition().y) * 2;

    button.setSize(sf::Vector2f(buttonSizeX, buttonSizeY));
    button.setFillColor(sf::Color(0, 0, 0, 0));

    window.draw(buttonText);
    window.draw(button);

    bool menuContinueExecution = true;
    int action = 0;

    while (menuContinueExecution) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
        }

        buttonText.setFillColor(sf::Color::Blue);

        action = 0;

        if (button.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            buttonText.setFillColor(sf::Color::Red);
            action = 1;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (action == 1) {
                std::cout << "it works! Exit.\n";
                window.close();
                return;
            }
        }

        window.draw(backGround);
        window.draw(titul);
        window.draw(button);
        window.draw(buttonText);

        window.display();
    }

    // window.display();
}

void RunApplication() {
    sf::RenderWindow window(sf::VideoMode({kDeffaultWindowLenght, kDeffaultWindowHeight}), "Sea Battle");
    
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // window.clear(sf::Color(186, 206, 219));

        Menu(window);

        window.display();
    }
}
}  // namespace SeaBattleExecutor
