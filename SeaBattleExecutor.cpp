#include "SeaBattleExecutor.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <thread>
#include <chrono>

namespace {
const int kDeffaultWindowLenght = 1440;
const int kDeffaultWindowHeight = 900;

// константы для меню

const unsigned int kMenuTitulCharacterSize = 100;
const unsigned int kMenuButtonsCharacterSize = 40;

const int kMenuTitulCoordinateY = 50;
const int kMenuStartGameButtonCoordinateY = 250;
const int kMenuShowRulesButtonCoordinateY = 350;
const int kMenuQuitGameButtonCoordinateY = 450;


// константы для окна проверки выхода
const unsigned int kCheckWindowSizeX = 600;
const unsigned int kCheckWindowSizeY = 300;

const unsigned int kExitCheckCharacterSize = 40;

const int kExitCheckQuestionCoordinateY = 50;
const int kExitCheckButtonsCoordinateY = 150;


}  // namespace

namespace SeaBattleExecutor {
void StartGame(sf::RenderWindow& window) {
    window.clear(sf::Color::White);

    bool gameContinueExecution = true;

    while (window.isOpen() && gameContinueExecution) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
        }

        window.clear(sf::Color::White);

        window.display();
    }
}


bool QuitGame(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite) {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    // создание окна с подверждением выхода из игры
    sf::RenderTexture exitCheckTexture(sf::Vector2u(kCheckWindowSizeX, kCheckWindowSizeY));

    sf::Sprite exitCheckSprite(exitCheckTexture.getTexture());
    exitCheckSprite.setPosition(sf::Vector2f((window.getSize().x - kCheckWindowSizeX) / 2, (window.getSize().y - kCheckWindowSizeY) / 2));

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }

    sf::Text question(helveticaFont, sf::String("Are you sure you want to exit?"));
    question.setFillColor(sf::Color::Blue);
    question.setCharacterSize(kExitCheckCharacterSize);
    question.setOutlineColor(sf::Color::White);

    float questionCoordinateX = (exitCheckTexture.getSize().x / 2) - question.getGlobalBounds().getCenter().x;
    question.setPosition(sf::Vector2f(questionCoordinateX, kExitCheckQuestionCoordinateY));


    // создаём надпись кнопки да
    sf::Text yesButtonText(helveticaFont, sf::String("Yes"));
    yesButtonText.setFillColor(sf::Color::Blue);
    yesButtonText.setCharacterSize(kExitCheckCharacterSize);

    float yesButtonCoordinateX = (exitCheckTexture.getSize().x / 4) - yesButtonText.getGlobalBounds().getCenter().x;
    yesButtonText.setPosition(sf::Vector2f(yesButtonCoordinateX, kExitCheckButtonsCoordinateY));

    // создаём кнопку да
    sf::RectangleShape yesButton;
    yesButton.setPosition(yesButtonText.getPosition());
    float yesButtonSizeX = (yesButtonText.getGlobalBounds().getCenter().x - yesButtonText.getPosition().x) * 2;
    float yesButtonSizeY = (yesButtonText.getGlobalBounds().getCenter().y - yesButtonText.getPosition().y) * 2;

    yesButton.setSize(sf::Vector2f(yesButtonSizeX, yesButtonSizeY));
    yesButton.setFillColor(sf::Color(0, 0, 0, 0));

    

    // создаём надпись кнопки нет
    sf::Text noButtonText(helveticaFont, sf::String("No"));
    noButtonText.setFillColor(sf::Color::Blue);
    noButtonText.setCharacterSize(kExitCheckCharacterSize);

    float noButtonCoordinateX = ((exitCheckTexture.getSize().x / 4) - noButtonText.getGlobalBounds().getCenter().x) * 3;
    noButtonText.setPosition(sf::Vector2f(noButtonCoordinateX, kExitCheckButtonsCoordinateY));

    // создаём кнопку нет
    sf::RectangleShape noButton;
    noButton.setPosition(noButtonText.getPosition());
    float noButtonSizeX = (noButtonText.getGlobalBounds().getCenter().x - noButtonText.getPosition().x) * 2;
    float noButtonSizeY = (noButtonText.getGlobalBounds().getCenter().y - noButtonText.getPosition().y) * 2;

    noButton.setSize(sf::Vector2f(noButtonSizeX, noButtonSizeY));
    noButton.setFillColor(sf::Color(0, 0, 0, 0));

    int num = 0;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return true;
            }
        }

        window.clear();
        yesButtonText.setFillColor(sf::Color::Blue);
        noButtonText.setFillColor(sf::Color::Blue);
        num = 0;

        if (yesButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - exitCheckSprite.getPosition())) {
            yesButtonText.setFillColor(sf::Color::Red);
            num = 1;
        }

        if (noButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - exitCheckSprite.getPosition())) {
            noButtonText.setFillColor(sf::Color::Red);
            num = 2;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            switch (num) {
                case 0:
                    break;
                case 1:
                    return true;
                    break;
                case 2:
                    return false;
                    break;
            }
        }

        exitCheckTexture.clear(sf::Color::White);
        exitCheckTexture.draw(question);

        exitCheckTexture.draw(yesButton);
        exitCheckTexture.draw(yesButtonText);

        exitCheckTexture.draw(noButton);
        exitCheckTexture.draw(noButtonText);

        exitCheckTexture.display();

        window.draw(windowCurrentStateSprite);
        window.draw(exitCheckSprite);

        window.display();
    }

    return true;
}



void Menu(sf::RenderWindow& window) {
    // загрузка картинки заднего фона в текстуру
    sf::Texture imageBackground;
    if (!imageBackground.loadFromFile("../images/background.jpg")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape backGround(sf::Vector2f(kDeffaultWindowLenght, kDeffaultWindowHeight));
    backGround.setTexture(&imageBackground);
    // window.draw(backGround);



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

    float titulCoordinateX = (window.getSize().x / 2) - titul.getGlobalBounds().getCenter().x;
    titul.setPosition(sf::Vector2f(titulCoordinateX, kMenuTitulCoordinateY));



    // создаём надпись кнопки, запускающей игру
    sf::Text startGameButtonText(helveticaFont, sf::String("Start the game"));
    startGameButtonText.setFillColor(sf::Color::Blue);
    startGameButtonText.setCharacterSize(kMenuButtonsCharacterSize);

    float startGameButtonCoordinateX = (window.getSize().x / 2) - startGameButtonText.getGlobalBounds().getCenter().x;
    startGameButtonText.setPosition(sf::Vector2f(startGameButtonCoordinateX, kMenuStartGameButtonCoordinateY));

    // создаём кнопку, запускающую игру
    sf::RectangleShape startGameButton;
    startGameButton.setPosition(startGameButtonText.getPosition());
    float startGameButtonSizeX = (startGameButtonText.getGlobalBounds().getCenter().x - startGameButtonText.getPosition().x) * 2;
    float startGameButtonSizeY = (startGameButtonText.getGlobalBounds().getCenter().y - startGameButtonText.getPosition().y) * 2;

    startGameButton.setSize(sf::Vector2f(startGameButtonSizeX, startGameButtonSizeY));
    startGameButton.setFillColor(sf::Color(0, 0, 0, 0));


    // создаём надпись кнопки, показывающей правила игры
    sf::Text showRulesButtonText(helveticaFont, sf::String("Check Rules"));
    showRulesButtonText.setFillColor(sf::Color::Blue);
    showRulesButtonText.setCharacterSize(kMenuButtonsCharacterSize);

    float showRulesButtonCoordinateX = (window.getSize().x / 2) - showRulesButtonText.getGlobalBounds().getCenter().x;
    showRulesButtonText.setPosition(sf::Vector2f(showRulesButtonCoordinateX, kMenuShowRulesButtonCoordinateY));

    // создаём кнопку, показывающую правила игры
    sf::RectangleShape showRulesButton;
    showRulesButton.setPosition(showRulesButtonText.getPosition());
    float showRulesButtonSizeX = (showRulesButtonText.getGlobalBounds().getCenter().x - showRulesButtonText.getPosition().x) * 2;
    float showRulesButtonSizeY = (showRulesButtonText.getGlobalBounds().getCenter().y - showRulesButtonText.getPosition().y) * 2;

    showRulesButton.setSize(sf::Vector2f(showRulesButtonSizeX, showRulesButtonSizeY));
    showRulesButton.setFillColor(sf::Color(0, 0, 0, 0));
    

    // создаём надпись кнопки, закрывающей игру
    sf::Text quitGameButtonText(helveticaFont, sf::String("Exit the game"));
    quitGameButtonText.setFillColor(sf::Color::Blue);
    quitGameButtonText.setCharacterSize(kMenuButtonsCharacterSize);

    float quitGameButtonCoordinateX = (window.getSize().x / 2) - quitGameButtonText.getGlobalBounds().getCenter().x;
    quitGameButtonText.setPosition(sf::Vector2f(quitGameButtonCoordinateX, kMenuQuitGameButtonCoordinateY));

    // создаём кнопку, закрывающую игру
    sf::RectangleShape quitGameButton;
    quitGameButton.setPosition(quitGameButtonText.getPosition());
    float quitGameButtonSizeX = (quitGameButtonText.getGlobalBounds().getCenter().x - quitGameButtonText.getPosition().x) * 2;
    float quitGameButtonSizeY = (quitGameButtonText.getGlobalBounds().getCenter().y - quitGameButtonText.getPosition().y) * 2;

    quitGameButton.setSize(sf::Vector2f(quitGameButtonSizeX, quitGameButtonSizeY));
    quitGameButton.setFillColor(sf::Color(0, 0, 0, 0));

    bool menuContinueExecution = true;
    Action menuAction = Action::Wait;

    while (window.isOpen() && menuContinueExecution) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
        }

        window.clear();

        startGameButtonText.setFillColor(sf::Color::Blue);
        showRulesButtonText.setFillColor(sf::Color::Blue);
        quitGameButtonText.setFillColor(sf::Color::Blue);

        menuAction = Action::Wait;

        if (startGameButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            startGameButtonText.setFillColor(sf::Color::Red);
            menuAction = Action::StartGame;
        }

        if (showRulesButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            showRulesButtonText.setFillColor(sf::Color::Red);
            menuAction = Action::ShowRules;
        }

        if (quitGameButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            quitGameButtonText.setFillColor(sf::Color::Red);
            menuAction = Action::QuitGame;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            switch (menuAction) {
                case Action::Wait:
                    break;
                case Action::StartGame:
                    StartGame(window);
                    break;
                case Action::ShowRules:
                    std::cout << "Show Rules\n";
                    break;
                case Action::QuitGame:
                    quitGameButtonText.setFillColor(sf::Color::Blue);
                    window.draw(backGround);
                    window.draw(titul);

                    window.draw(startGameButton);
                    window.draw(startGameButtonText);

                    window.draw(showRulesButton);
                    window.draw(showRulesButtonText);
                    
                    window.draw(quitGameButton);
                    window.draw(quitGameButtonText);

                    sf::Texture windowCurrentStateTexture(window.getSize());
                    windowCurrentStateTexture.update(window);

                    sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

                    if (QuitGame(window, windowCurrentStateSprite)) {
                        window.close();
                        continue;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(150));

                    break;
            }
        }

        window.draw(backGround);
        window.draw(titul);

        window.draw(startGameButton);
        window.draw(startGameButtonText);

        window.draw(showRulesButton);
        window.draw(showRulesButtonText);
        
        window.draw(quitGameButton);
        window.draw(quitGameButtonText);

        window.display();
    }
}

void RunApplication() {
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Sea Battle", sf::State::Fullscreen);
    // window.setFramerateLimit(5);
    // window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        Menu(window);
    }
}
}  // namespace SeaBattleExecutor
