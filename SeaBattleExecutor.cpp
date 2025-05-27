#include "SeaBattleExecutor.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <iostream>
#include <cstddef>
#include <stdexcept>

namespace {
const size_t kWindowLenght = 1200;
const size_t kWindowHeight = 900;

const size_t kOpponentBattleFieldX = 180;
const size_t kOpponentBattleFieldY = 240;

const size_t kPlayerBattleFieldX = 600;
const size_t kPlayerBattleFieldY = 240;

const size_t kCubePixelSize = 38;
const size_t kCubeOutlineThikness = 1;

}  // namespace

namespace SeaBattleExecutor {
void Menu(sf::RenderWindow& window) {
    // загрузка картинки заднего фона в текстуру
    sf::Texture menuBackground;
    if (!menuBackground.loadFromFile("../images/background.jpg")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape backGround(sf::Vector2f(kWindowLenght, kWindowHeight));
    backGround.setTexture(&menuBackground);
    backGround.setPosition(sf::Vector2f(0, 0));
    window.draw(backGround);

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // создаём лейбл для названия игры
    sf::Text titul(helveticaFont, sf::String("Sea Battle"));
    titul.setPosition(sf::Vector2f(400, 50));
    titul.setFillColor(sf::Color(172, 232, 228));
    titul.setCharacterSize(100);
    titul.setOutlineThickness(3);
    titul.setOutlineColor(sf::Color::White);
    window.draw(titul);

    // создаём кнопку на базе RectangeShape
    sf::RectangleShape button({200.f,150.f});
    button.setPosition(sf::Vector2f(600,300));
    button.setFillColor(sf::Color(0, 0, 0, 0));
    window.draw(button);

    // создаём надпись кнопки, котрую наложим поверх кнопки
    sf::Text buttonText(helveticaFont, sf::String("Push me"));
    buttonText.setPosition(button.getPosition());
    buttonText.setFillColor(sf::Color::Blue);
    buttonText.setCharacterSize(60);
    window.draw(buttonText);


    // std::cout << button.getGlobalBounds().getCenter().x << ' ' << button.getGlobalBounds().getCenter().x << '\n';

    bool continueExecution = true;
    int menuNum = 0;

    while (continueExecution) {

        buttonText.setFillColor(sf::Color::Blue);

        menuNum = 0;
        // auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
        
        auto mousePosRelatedOnWindow = static_cast<sf::Vector2f>(sf::Mouse::getPosition() - window.getPosition());

        std::cout << mousePosRelatedOnWindow.x << ' ' << mousePosRelatedOnWindow.y << '\n';
        std::cout << button.getGlobalBounds().position.x << ' ' << button.getGlobalBounds().position.y << '\n';
        // auto checkingRectangle = buttonText.getGlobalBounds();

        // std::cout << button.getGlobalBounds().getCenter().x << ' ' << button.getGlobalBounds().getCenter().x << '\n';

        if (button.getGlobalBounds().contains(mousePosRelatedOnWindow)) {
            buttonText.setFillColor(sf::Color::Red);
            menuNum = 1;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (menuNum == 1) {
                std::cout << "it works!\n";
                continueExecution = false;
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
    sf::RenderWindow window(sf::VideoMode({kWindowLenght, kWindowHeight}), "Sea Battle");

    // sf::Texture empty;
    // if (!empty.loadFromFile("../images/dot.jpg")) {
    //     throw std::runtime_error("failed to open file");
    // }

    // sf::Texture unknown;
    // if (!unknown.loadFromFile("../images/unknown.jpg")) {
    //     throw std::runtime_error("failed to open file");
    // }

    // sf::Texture hitted;
    // if (!hitted.loadFromFile("../images/cross.webp.png")) {
    //     throw std::runtime_error("failed to open file");
    // }
    
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(186, 206, 219));

        Menu(window);


        // int size = 10;

        // for (int i = 0; i < size; ++i) {
        //     for (int j = 0; j < size; ++j) {
        //         sf::RectangleShape cube(sf::Vector2f(kCubePixelSize, kCubePixelSize));
        //         cube.setFillColor(sf::Color(136, 204, 252));
        //         cube.setPosition(sf::Vector2f(kOpponentBattleFieldX + j * (kCubePixelSize + 2 * kCubeOutlineThikness), kOpponentBattleFieldY + i * (kCubePixelSize + 2 * kCubeOutlineThikness)));
        //         cube.setOutlineThickness(kCubeOutlineThikness);
        //         cube.setOutlineColor(sf::Color::White);
                
        //         cube.setTexture(&empty);

        //         window.draw(cube);
        //     }
        // }

        // sf::RectangleShape cube(sf::Vector2f(kCubePixelSize, kCubePixelSize));
        // cube.setFillColor(sf::Color(136, 204, 252));
        // cube.setPosition(sf::Vector2f(kOpponentBattleFieldX, kOpponentBattleFieldY));
        // cube.setOutlineThickness(kCubeOutlineThikness);
        // cube.setOutlineColor(sf::Color::White);
        
        // cube.setTexture(&hitted);

        // window.draw(cube);

        // for (int i = 0; i < size; ++i) {
        //     for (int j = 0; j < size; ++j) {
        //         sf::RectangleShape cube(sf::Vector2f(kCubePixelSize, kCubePixelSize));
        //         cube.setFillColor(sf::Color(136, 204, 252));
        //         cube.setPosition(sf::Vector2f(kPlayerBattleFieldX + j * (kCubePixelSize + 2 * kCubeOutlineThikness), kPlayerBattleFieldY + i * (kCubePixelSize + 2 * kCubeOutlineThikness)));
        //         cube.setOutlineThickness(kCubeOutlineThikness);
        //         cube.setOutlineColor(sf::Color::White);
                
        //         cube.setTexture(&empty);

        //         window.draw(cube);
        //     }
        // }

        window.display();
    }
}
}  // namespace SeaBattleExecutor
