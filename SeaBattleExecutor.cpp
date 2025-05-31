#include "SeaBattleExecutor.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GameClasses/Classes.h"
#include "Battlefield/Battlefield.h"

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

// константы для окна вывода результатов игры

const unsigned int kResultsWindowSizeX = 600;
const unsigned int kResultsWindowSizeY = 300;

const unsigned int kResultsWindowCharacterSize = 40;

const int kResultsTextCoordinateY = 50;
const int kReturnButtonCoordinateY = 150;

// константы для дочерних окон
const unsigned int kChildWindowMinSizeX = 600;
const unsigned int kChildWindowMinSizeY = 300;

const unsigned int kChildWindowMaxCharacterSize = 40;

// константы для дочернего окна с дихотомическим вопросом
const int kChildWindowQuestionCoordinateY = 50;
const int kChildWindowButtonsCoordinateY = 150;

// константы для дочернего окна с информацией

const unsigned int kChildWindowIndent = 40;


bool ShowChildWindowWithDichotomousQuestion(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite, const sf::String& text) {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }
    
    // создание лейбла с вопросом
    sf::Text question(helveticaFont);
    question.setFillColor(sf::Color::Blue);
    question.setOutlineColor(sf::Color::White);



    question.setCharacterSize(kChildWindowMaxCharacterSize);
    question.setString(text);


    // создаём надпись кнопки да
    sf::Text yesButtonText(helveticaFont, sf::String("Yes"));
    yesButtonText.setFillColor(sf::Color::Blue);
    yesButtonText.setCharacterSize(kChildWindowMaxCharacterSize);

    // создаём надпись кнопки нет
    sf::Text noButtonText(helveticaFont, sf::String("No"));
    noButtonText.setFillColor(sf::Color::Blue);
    noButtonText.setCharacterSize(kChildWindowMaxCharacterSize);

    float yesButtonTextSizeX = yesButtonText.getGlobalBounds().getCenter().x * 2;
    float yesButtonTextSizeY = yesButtonText.getGlobalBounds().getCenter().y * 2;


    question.setCharacterSize(kChildWindowMaxCharacterSize);
    float questionSizeY = question.getGlobalBounds().getCenter().y * 2;

    while (questionSizeY > window.getSize().y - 3 * kChildWindowIndent - yesButtonTextSizeY) {
        question.setCharacterSize(question.getCharacterSize() / 2);
        questionSizeY = question.getGlobalBounds().getCenter().y * 2;
    }

    float questionSizeX = question.getGlobalBounds().getCenter().x * 2;

    unsigned int childWindowSizeX = questionSizeX + 2 * kChildWindowIndent;

    if (childWindowSizeX < kChildWindowMinSizeX) {
        childWindowSizeX = kChildWindowMinSizeX;
    }

    unsigned int childWindowSizeY = questionSizeY + yesButtonTextSizeY + 3 * kChildWindowIndent;

    // создание окна с подверждением выхода из игры
    sf::RenderTexture childWindow(sf::Vector2u(childWindowSizeX, childWindowSizeY));

    sf::Sprite childWindowSprite(childWindow.getTexture());
    childWindowSprite.setPosition(sf::Vector2f((window.getSize().x - childWindowSizeX) / 2, (window.getSize().y - childWindowSizeY) / 2));

    // размещение вопроса
    float questionCoordinateX = (childWindowSizeX - questionSizeX) / 2;
    question.setPosition(sf::Vector2f(questionCoordinateX, kChildWindowIndent));

    // создаём кнопку да
    sf::RectangleShape yesButton;
    yesButton.setSize(sf::Vector2f(yesButtonTextSizeX, yesButtonTextSizeY));
    yesButton.setFillColor(sf::Color::White);

    // размещение кнопки да
    float yesButtonCoordinateX = (childWindowSizeX - yesButtonTextSizeX) / 4;
    float yesButtonCoordinateY = questionSizeY + 2 * kChildWindowIndent;
    yesButton.setPosition(sf::Vector2f(yesButtonCoordinateX, yesButtonCoordinateY));

    // создаём кнопку нет
    sf::RectangleShape noButton;
    noButton.setSize(sf::Vector2f(yesButtonTextSizeX, yesButtonTextSizeY));
    noButton.setFillColor(sf::Color::White);

    // размещение кнопки нет
    noButton.setPosition(sf::Vector2f(3 * yesButtonCoordinateX, yesButtonCoordinateY));

    // размещение текста кнопки да
    yesButtonText.setPosition(yesButton.getPosition());

    // размещение текста кнопки нет
    noButtonText.setPosition(sf::Vector2f(3 * yesButtonCoordinateX + (yesButtonTextSizeX / 2 - noButtonText.getGlobalBounds().getCenter().x), yesButtonCoordinateY));

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

        if (yesButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - childWindowSprite.getPosition())) {
            yesButtonText.setFillColor(sf::Color::Red);
            num = 1;
        }

        if (noButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - childWindowSprite.getPosition())) {
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

        childWindow.clear(sf::Color::White);
        childWindow.draw(question);

        childWindow.draw(yesButton);
        childWindow.draw(yesButtonText);

        childWindow.draw(noButton);
        childWindow.draw(noButtonText);

        childWindow.display();

        window.draw(windowCurrentStateSprite);
        window.draw(childWindowSprite);

        window.display();
    }

    return true;
}

void ShowChildWindowWithInformation(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite, const sf::String& text) {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // создание лейбла с информацией
    sf::Text information(helveticaFont, text);
    information.setFillColor(sf::Color::Blue);
    information.setOutlineColor(sf::Color::White);

    // создание надписи кнопки продолжить
    sf::Text continueButtonText(helveticaFont, sf::String("Continue"));
    continueButtonText.setFillColor(sf::Color::Blue);
    continueButtonText.setCharacterSize(kChildWindowMaxCharacterSize);

    float continueButtonTextSizeX = continueButtonText.getGlobalBounds().getCenter().x * 2;
    float continueButtonTextSizeY = continueButtonText.getGlobalBounds().getCenter().y * 2;


    information.setCharacterSize(kChildWindowMaxCharacterSize);
    float informationSizeY = information.getGlobalBounds().getCenter().y * 2;

    while (informationSizeY > window.getSize().y - 3 * kChildWindowIndent - continueButtonTextSizeY) {
        information.setCharacterSize(information.getCharacterSize() / 2);
        informationSizeY = information.getGlobalBounds().getCenter().y * 2;
    }

    float informationSizeX = information.getGlobalBounds().getCenter().x * 2;

    unsigned int childWindowSizeX = informationSizeX + 2 * kChildWindowIndent;

    if (childWindowSizeX < kChildWindowMinSizeX) {
        childWindowSizeX = kChildWindowMinSizeX;
    }

    unsigned int childWindowSizeY = informationSizeY + continueButtonTextSizeY + 3 * kChildWindowIndent;

    // создание дочернего окна с результатами
    sf::RenderTexture childWindow(sf::Vector2u(childWindowSizeX, childWindowSizeY));

    sf::Sprite childWindowSprite(childWindow.getTexture());
    childWindowSprite.setPosition(sf::Vector2f((window.getSize().x - childWindowSizeX) / 2, (window.getSize().y - childWindowSizeY) / 2));

    // размещение информации
    float informationCoordinateX = (childWindowSizeX - informationSizeX) / 2;
    information.setPosition(sf::Vector2f(informationCoordinateX, kChildWindowIndent));

    // размещение текста кнопки продолжить
    float continueButtonCoordinateX = (childWindowSizeX - continueButtonTextSizeX) / 2;
    float continueButtonCoordinateY = informationSizeY + 2 * kChildWindowIndent;
    continueButtonText.setPosition(sf::Vector2f(continueButtonCoordinateX, continueButtonCoordinateY));

    // создаём кнопку продолжить
    sf::RectangleShape continueButton;
    continueButton.setPosition(continueButtonText.getPosition());
    continueButton.setSize(sf::Vector2f(continueButtonTextSizeX, continueButtonTextSizeY));
    continueButton.setFillColor(sf::Color::Transparent);

    bool continueButtonChosen = false;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
        }

        window.clear();

        continueButtonText.setFillColor(sf::Color::Blue);
        continueButtonChosen = false;

        if (continueButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - childWindowSprite.getPosition())) {
            continueButtonText.setFillColor(sf::Color::Red);
            continueButtonChosen = true;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (continueButtonChosen) {
                return;
            }
        }

        childWindow.clear(sf::Color::White);

        childWindow.draw(information);
        childWindow.draw(continueButton);
        childWindow.draw(continueButtonText);

        childWindow.display();

        window.draw(windowCurrentStateSprite);
        window.draw(childWindowSprite);

        window.display();
    }
}

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
    if (!crossTexture.loadFromFile("../images/cross.webp.png")) {
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
    // playerBoard.autoPlaceShips();
    randomPlaceShips(playerBoard);

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
    // robotBoard.autoPlaceShips();
    randomPlaceShips(robotBoard);

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

    int robotShootX = 0;
    int robotShootY = 0;

    auto DrawAllEntitiesInWindow = [] (sf::RenderWindow& window, auto& playerShapeMatrix, auto& robotShapeMatrix, auto& exitButton) {
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
    };

    while (window.isOpen() && gameContinueExecution) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
        }

        if (playerBoard.allShipsSunk() || robotBoard.allShipsSunk()) {
            gameContinueExecution = false;
            continue;
        }

        window.clear(sf::Color::White);

        exitButton.setOutlineColor(sf::Color::White);
        bool exitButtonChosen = false;

        if (exitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            exitButtonChosen = true;
        }

        if (exitButtonChosen) {
            exitButton.setOutlineColor(sf::Color::Red);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (exitButtonChosen) {
                DrawAllEntitiesInWindow(window, playerShapeMatrix, robotShapeMatrix, exitButton);

                sf::Texture windowCurrentStateTexture(window.getSize());
                windowCurrentStateTexture.update(window);
                sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);
                
                if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, "Do you want to exit to menu?")) {
                    return;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
        }

        // замена клеток поля робота
        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                if (!robotBoard.isShooted(i, j)) {
                    robotShapeMatrix[i][j].setFillColor(sf::Color::Blue);
                    continue;
                }

                if (robotBoard.getCell(i, j) == CellStatus::ShootedEmpty) {
                    robotShapeMatrix[i][j].setFillColor(sf::Color::White);
                    robotShapeMatrix[i][j].setTexture(&dotTexture);
                }

                if (robotBoard.getCell(i, j) == CellStatus::ShootedShip) {
                    robotShapeMatrix[i][j].setFillColor(sf::Color::White);
                    robotShapeMatrix[i][j].setTexture(&crossTexture);
                }
            }
        }

        // замена клеток поля игрока
        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                if (!playerBoard.isShooted(i, j)) {
                    playerShapeMatrix[i][j].setFillColor(sf::Color::Blue);
                    continue;
                }

                if (playerBoard.getCell(i, j) == CellStatus::ShootedEmpty) {
                    playerShapeMatrix[i][j].setFillColor(sf::Color::White);
                    playerShapeMatrix[i][j].setTexture(&dotTexture);
                }

                if (playerBoard.getCell(i, j) == CellStatus::ShootedShip) {
                    playerShapeMatrix[i][j].setFillColor(sf::Color::White);
                    playerShapeMatrix[i][j].setTexture(&crossTexture);
                }
            }
        }

        numi = -1;
        numj = -1;

        if (playerMove) {
            for (size_t i = 0; i < kArraySize; ++i) {
                for (size_t j = 0; j < kArraySize; ++j) {
                    if (robotBoard.isShooted(i, j)) {
                        continue;
                    }

                    if (robotShapeMatrix[i][j].getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                        numi = i;
                        numj = j;
                    }
                }
            }
        }

        if (playerMove && numi >= 0 && numj >= 0) {
            robotShapeMatrix[numi][numj].setFillColor(sf::Color::Red);
        }

        if (!playerMove) {
            if (randomShoot(playerBoard, robotShootX, robotShootY) == ShootResult::Miss) {
                playerMove = true;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

            }
        } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (numi >= 0 && numj >= 0) {
                if (robotBoard.shoot(numi, numj) == ShootResult::Miss) {
                    playerMove = false;
                }
            }
        }

        DrawAllEntitiesInWindow(window, playerShapeMatrix, robotShapeMatrix, exitButton);

        window.display();
    }

    sf::Texture windowCurrentStateTexture(window.getSize());
    windowCurrentStateTexture.update(window);
    sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

    if (robotBoard.allShipsSunk()) {
        ShowChildWindowWithInformation(window, windowCurrentStateSprite, "My congrats! You won!");
        return;
    }

    ShowChildWindowWithInformation(window, windowCurrentStateSprite, "Unluck! You have lost");
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

    auto DrawAllEntitiesInWindow = [] (sf::RenderWindow& window, auto& backGround, auto& titul, auto& startGameButton, auto& startGameButtonText, auto& showRulesButton, auto& showRulesButtonText, auto& quitGameButton, auto& quitGameButtonText) {
        window.draw(backGround);
        window.draw(titul);

        window.draw(startGameButton);
        window.draw(startGameButtonText);

        window.draw(showRulesButton);
        window.draw(showRulesButtonText);
        
        window.draw(quitGameButton);
        window.draw(quitGameButtonText);
    };

    Action menuAction = Action::Wait;
    sf::Texture windowCurrentStateTexture(window.getSize());
    windowCurrentStateTexture.update(window);
    sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

    while (window.isOpen()) {
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
                    return;
                case Action::ShowRules:
                    showRulesButtonText.setFillColor(sf::Color::Blue);
                    DrawAllEntitiesInWindow(window, backGround, titul,startGameButton, startGameButtonText, showRulesButton, showRulesButtonText, quitGameButton, quitGameButtonText);

                    windowCurrentStateTexture.update(window);
                    windowCurrentStateSprite.setTexture(windowCurrentStateTexture);

                    ShowChildWindowWithInformation(window, windowCurrentStateSprite, GetRulesFromFile());

                    std::this_thread::sleep_for(std::chrono::milliseconds(150));
                    break;
                case Action::QuitGame:
                    quitGameButtonText.setFillColor(sf::Color::Blue);
                    DrawAllEntitiesInWindow(window, backGround, titul,startGameButton, startGameButtonText, showRulesButton, showRulesButtonText, quitGameButton, quitGameButtonText);


                    windowCurrentStateTexture.update(window);
                    windowCurrentStateSprite.setTexture(windowCurrentStateTexture);
                    
                    if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, "Are you sure you want to exit?")) {
                        window.close();
                        return;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(150));

                    break;
            }
        }

        DrawAllEntitiesInWindow(window, backGround, titul,startGameButton, startGameButtonText, showRulesButton, showRulesButtonText, quitGameButton, quitGameButtonText);     
        window.display();
    }
}

void RunApplication() {
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Sea Battle", sf::State::Fullscreen);

    while (window.isOpen()) {
        Menu(window);

        if (!window.isOpen()) {
            break;
        }

        StartGame(window);
    }
}
}  // namespace SeaBattleExecutor
