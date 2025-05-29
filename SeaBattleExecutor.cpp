#include "SeaBattleExecutor.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GameClasses/Classes.h"

#include <vector>
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <array>


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

// константы для окна с правилами
const unsigned int kRulesWindowSizeX = 900;
const unsigned int kRulesWindowSizeY = 500;

const unsigned int kRulesCharacterSize = 20;

const int kRulesTextCoordinateY = 10;
const int kRulesButtonCoordinateY = 400;

// константы для окна проверки выхода
const unsigned int kCheckWindowSizeX = 600;
const unsigned int kCheckWindowSizeY = 300;

const unsigned int kExitCheckCharacterSize = 40;

const int kExitCheckQuestionCoordinateY = 50;
const int kExitCheckButtonsCoordinateY = 150;

// константы для игры
const unsigned int kArraySize = 10;

const int kPlayerMatrixCoordinateX = 200;
const int kPlayerMatrixCoordinateY = 150;

const int kRobotMatrixCoordinateX = 700;
const int kRobotMatrixCoordinateY = 150;

const int kCellPixelSize = 30;
const int kCellOutlineThickness = 2;

// константы для кнопки выхода из игры

const unsigned int kGameExitButtonSizeX = 50;
const unsigned int kGameExitButtonSizeY = 50;

const int kGameExitButtonCoordinateX = 40;
const int kGameExitButtonCoordinateY = 40;



std::string GetRulesFromFile() {
    std::string text;

    std::ifstream rulesFile("../images/rules.txt");

    if (!rulesFile) {
        throw std::runtime_error("failed to open file");
    }

    std::string line;

    while (std::getline(rulesFile, line)) {
        text += line;
        text += '\n';
    }

    rulesFile.close();
    return text;
}
}  // namespace

namespace SeaBattleExecutor {
void StartGame(sf::RenderWindow& window) {
    bool gameContinueExecution = true;
    bool playerMove = true;

    // закгрузка текстуры крестика
    sf::Texture crossTexture;
    if (!crossTexture.loadFromFile("../images/exit_image.png")) {
        throw std::runtime_error("failed to load image");
    }

    // закгрузка текстуры точки
    sf::Texture dotTexture;
    if (!dotTexture.loadFromFile("../images/dot.jpg")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры для кнопки выхода из игры
    sf::Texture exitButtonTexture;
    if (!exitButtonTexture.loadFromFile("../images/exit_image.png")) {
        throw std::runtime_error("failed to load image");
    }

    // создание кнопки для выхода из игры
    sf::RectangleShape exitButton;
    exitButton.setSize(sf::Vector2f(kGameExitButtonSizeX, kGameExitButtonSizeY));
    exitButton.setPosition(sf::Vector2f(kGameExitButtonCoordinateX, kGameExitButtonCoordinateY));
    exitButton.setTexture(&exitButtonTexture);
    exitButton.setOutlineThickness(1);
    exitButton.setOutlineColor(sf::Color::White);

    bool exitButtonChosen = false;
    
    // создание борда для игрока
    Board playerBoard;
    playerBoard.autoPlaceShips();

    for (size_t i = 0; i < kArraySize; ++i) {
        for (size_t j = 0; j < kArraySize; ++j) {
            std::cout << static_cast<int>(playerBoard.getCell(i, j)) << ' ';
        }

        std::cout << '\n';
    }

    // создание массива шейпов для игрока
    std::array<std::array<sf::RectangleShape, kArraySize>, kArraySize> playerShapeMatrix;

    for (size_t i = 0; i < kArraySize; ++i) {
        for (size_t j = 0; j < kArraySize; ++j) {
            playerShapeMatrix[i][j].setFillColor(sf::Color::Blue);
            playerShapeMatrix[i][j].setPosition(sf::Vector2f(kPlayerMatrixCoordinateX + j * kCellPixelSize + (j + 1) * kCellOutlineThickness, kPlayerMatrixCoordinateY + i * kCellPixelSize + (i + 1) * kCellOutlineThickness));
            playerShapeMatrix[i][j].setSize(sf::Vector2f(kCellPixelSize, kCellPixelSize));
            playerShapeMatrix[i][j].setOutlineThickness(kCellOutlineThickness);

            if (playerBoard.getCell(i, j) == CellStatus::Ship) {
                playerShapeMatrix[i][j].setOutlineColor(sf::Color::Red);
            }

            if (playerBoard.getCell(i, j) == CellStatus::Empty) {
                playerShapeMatrix[i][j].setOutlineColor(sf::Color::Transparent);
            }
        }
    }

    // создание борда для робота
    Board robotBoard;
    robotBoard.autoPlaceShips();

    // создание массива шейпов для робота
    std::array<std::array<sf::RectangleShape, kArraySize>, kArraySize> robotShapeMatrix;

    for (size_t i = 0; i < kArraySize; ++i) {
        for (size_t j = 0; j < kArraySize; ++j) {
            robotShapeMatrix[i][j].setFillColor(sf::Color::Blue);
            robotShapeMatrix[i][j].setPosition(sf::Vector2f(kRobotMatrixCoordinateX + j * kCellPixelSize + (j + 1) * kCellOutlineThickness, kRobotMatrixCoordinateY + i * kCellPixelSize + (i + 1) * kCellOutlineThickness));
            robotShapeMatrix[i][j].setSize(sf::Vector2f(kCellPixelSize, kCellPixelSize));
            robotShapeMatrix[i][j].setOutlineColor(sf::Color::Transparent);
            robotShapeMatrix[i][j].setOutlineThickness(kCellOutlineThickness);
        }
    }

    int numi = -1;
    int numj = -1;

    while (window.isOpen() && gameContinueExecution) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
        }

        window.clear(sf::Color::White);

        exitButton.setOutlineColor(sf::Color::White);
        bool exitButtonChosen = false;

        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                if (robotShapeMatrix[i][j].getFillColor() != sf::Color::Transparent) {
                    robotShapeMatrix[i][j].setFillColor(sf::Color::Blue);
                }
                
            }
        }

        numi = -1;
        numj = -1;


        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                if (robotShapeMatrix[i][j].getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                    numi = i;
                    numj = j;
                }
            }
        }

        if (exitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            exitButtonChosen = true;
        }


        if (numi >= 0 && numj >= 0) {
            robotShapeMatrix[numi][numj].setFillColor(sf::Color::Red);
        }

        if (exitButtonChosen) {
            exitButton.setOutlineColor(sf::Color::Red);
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (exitButtonChosen) {
                sf::Texture windowCurrentStateTexture(window.getSize());
                windowCurrentStateTexture.update(window);
                sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

                if (QuitGame(window, windowCurrentStateSprite)) {
                    window.close();
                    continue;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (numi >= 0 && numj >= 0) {
                switch(robotBoard.shoot(numi, numj)) {
                    case ShootResult::Miss:
                        robotShapeMatrix[numi][numj].setTexture(&dotTexture);
                        robotShapeMatrix[numi][numj].setFillColor(sf::Color::Transparent);
                        playerMove = false;
                        break;
                    case ShootResult::Hit:
                        robotShapeMatrix[numi][numj].setTexture(&crossTexture);
                        robotShapeMatrix[numi][numj].setOutlineColor(sf::Color::Red);
                        robotShapeMatrix[numi][numj].setFillColor(sf::Color::Transparent);
                        break;
                    case ShootResult::Kill:
                        robotShapeMatrix[numi][numj].setTexture(&crossTexture);
                        robotShapeMatrix[numi][numj].setOutlineColor(sf::Color::Red);
                        robotShapeMatrix[numi][numj].setFillColor(sf::Color::Transparent);
                        break;
                }
            }
                
        }

        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                window.draw(playerShapeMatrix[i][j]);
            }
        }

        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                window.draw(robotShapeMatrix[i][j]);
            }
        }

        window.draw(exitButton);

        window.display();
    }
}

void ShowRules(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite) {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    // создание окна с правилами
    sf::RenderTexture rulesTexture(sf::Vector2u(kRulesWindowSizeX, kRulesWindowSizeY));

    sf::Sprite rulesSprite(rulesTexture.getTexture());
    rulesSprite.setPosition(sf::Vector2f((window.getSize().x - kRulesWindowSizeX) / 2, (window.getSize().y - kRulesWindowSizeY) / 2));

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }

    sf::Text rules(helveticaFont);
    rules.setString(GetRulesFromFile());
    rules.setFillColor(sf::Color::Blue);
    rules.setCharacterSize(kRulesCharacterSize);
    rules.setOutlineColor(sf::Color::White);

    float rulesCoordinateX = (rulesTexture.getSize().x / 2) - rules.getGlobalBounds().getCenter().x;
    rules.setPosition(sf::Vector2f(rulesCoordinateX, kRulesTextCoordinateY));


    // создаём надпись кнопки ок
    sf::Text okButtonText(helveticaFont, sf::String("Ok"));
    okButtonText.setFillColor(sf::Color::Blue);
    okButtonText.setCharacterSize(kExitCheckCharacterSize);

    float okButtonCoordinateX = (rulesTexture.getSize().x / 2) - okButtonText.getGlobalBounds().getCenter().x;
    okButtonText.setPosition(sf::Vector2f(okButtonCoordinateX, kRulesButtonCoordinateY));

    // создаём кнопку да
    sf::RectangleShape okButton;
    okButton.setPosition(okButtonText.getPosition());
    float okButtonSizeX = (okButtonText.getGlobalBounds().getCenter().x - okButtonText.getPosition().x) * 2;
    float okButtonSizeY = (okButtonText.getGlobalBounds().getCenter().y - okButtonText.getPosition().y) * 2;

    okButton.setSize(sf::Vector2f(okButtonSizeX, okButtonSizeY));
    okButton.setFillColor(sf::Color(0, 0, 0, 0));

    int num = 0;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
        }

        window.clear();
        okButtonText.setFillColor(sf::Color::Blue);
        num = 0;

        if (okButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - rulesSprite.getPosition())) {
            okButtonText.setFillColor(sf::Color::Red);
            num = 1;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            switch (num) {
                case 0:
                    break;
                case 1:
                    return;
                    break;
            }
        }

        rulesTexture.clear(sf::Color::White);
        rulesTexture.draw(rules);

        rulesTexture.draw(okButton);
        rulesTexture.draw(okButtonText);

        rulesTexture.display();

        window.draw(windowCurrentStateSprite);
        window.draw(rulesSprite);

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
    sf::Texture windowCurrentStateTexture(window.getSize());
    windowCurrentStateTexture.update(window);
    sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

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
                    showRulesButtonText.setFillColor(sf::Color::Blue);
                    window.draw(backGround);
                    window.draw(titul);

                    window.draw(startGameButton);
                    window.draw(startGameButtonText);

                    window.draw(showRulesButton);
                    window.draw(showRulesButtonText);
                    
                    window.draw(quitGameButton);
                    window.draw(quitGameButtonText);

                    windowCurrentStateTexture.update(window);
                    windowCurrentStateSprite.setTexture(windowCurrentStateTexture);
                    
                    ShowRules(window, windowCurrentStateSprite);

                    std::this_thread::sleep_for(std::chrono::milliseconds(150));
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

                    windowCurrentStateTexture.update(window);
                    windowCurrentStateSprite.setTexture(windowCurrentStateTexture);

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
