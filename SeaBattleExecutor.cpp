#include "SeaBattleExecutor.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GameClasses/Classes.h"
#include "Battlefield/Battlefield.h"
#include "CustomEntities/Button.h"

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
// константы для меню
const unsigned int kMenuTitulCharacterSize = 100;
const unsigned int kMenuButtonsCharacterSize = 40;

const int kMenuTitulCoordinateY = 50;
const int kMenuStartGameButtonCoordinateY = 250;
const int kMenuShowRulesButtonCoordinateY = 370;
const int kMenuQuitGameButtonCoordinateY = 490;

const float kMenuButtonsSizeX = 550;
const float kMenuButtonsSizeY = 70;

// константы для игры
const unsigned int kArraySize = 10;

const int kPlayerMatrixCoordinateX = 200;
const int kPlayerMatrixCoordinateY = 150;

const int kRobotMatrixCoordinateX = 700;
const int kRobotMatrixCoordinateY = 150;

const int kCellPixelSize = 30;
const int kCellOutlineThickness = 2;

// константы для кнопки сдаться
const unsigned int kGameGiveUpButtonSizeX = 50;
const unsigned int kGameGiveUpButtonSizeY = 50;

const int kGameGiveUpButtonCoordinateX = 40;
const int kGameGiveUpButtonCoordinateY = 40;

const unsigned int kGameGiveUpButtonCharacterSize = 25;


// константы для дочерних окон
const unsigned int kChildWindowMinSizeX = 900;
const unsigned int kChildWindowMinSizeY = 300;

const unsigned int kChildWindowMaxCharacterSize = 40;

const float kChildWindowButtonSizeX = 100.f;
const float kChildWindowButtonSizeY = 70.f;

// константы для дочернего окна с дихотомическим вопросом

// константы для дочернего окна с информацией

const unsigned int kChildWindowIndent = 40;

// константы для цветов
const sf::Color kBrown = sf::Color(70, 55, 45);
const sf::Color kBlue = sf::Color(70, 90, 230);
const sf::Color kGreen = sf::Color(65, 180, 30);
const sf::Color kRed = sf::Color(235, 65, 65);

bool ShowChildWindowWithDichotomousQuestion(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite, const sf::String& text) {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка картинки заднего фона в текстуру
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../images/IMG_0801.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры кнопки да
    sf::Texture yesButtonTexture;
    if (!yesButtonTexture.loadFromFile("../images/IMG_0794.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры кнопки нет
    sf::Texture noButtonTexture;
    if (!noButtonTexture.loadFromFile("../images/IMG_0795.png")) {
        throw std::runtime_error("failed to open file");
    }
    
    
    // создание лейбла с вопросом
    sf::Text question(helveticaFont);
    question.setFillColor(kBrown);
    question.setOutlineColor(sf::Color::White);

    question.setCharacterSize(kChildWindowMaxCharacterSize);
    question.setString(text);


    // создаём крутую кнопку да
    Button yesButton(helveticaFont);
    yesButton.setString(L"Да");
    yesButton.setTextFillColor(kBrown);
    yesButton.setCharacterSize(kChildWindowMaxCharacterSize);
    yesButton.setSize(sf::Vector2f(kChildWindowButtonSizeX, kChildWindowButtonSizeY));
    yesButton.setTexture(&yesButtonTexture);


    // создаём крутую кнопку нет
    Button noButton(helveticaFont);
    noButton.setString(L"Нет");
    noButton.setTextFillColor(kBrown);
    noButton.setCharacterSize(kChildWindowMaxCharacterSize);
    noButton.setSize(sf::Vector2f(kChildWindowButtonSizeX, kChildWindowButtonSizeY));
    noButton.setTexture(&noButtonTexture);

    question.setCharacterSize(kChildWindowMaxCharacterSize);
    float questionSizeY = question.getLocalBounds().getCenter().y * 2;

    while (questionSizeY > window.getSize().y - 3 * kChildWindowIndent - yesButton.getSize().y) {
        question.setCharacterSize(question.getCharacterSize() / 2);
        questionSizeY = question.getLocalBounds().getCenter().y * 2;
    }

    float questionSizeX = question.getLocalBounds().getCenter().x * 2;

    unsigned int childWindowSizeX = questionSizeX + 2 * kChildWindowIndent;

    if (childWindowSizeX < kChildWindowMinSizeX) {
        childWindowSizeX = kChildWindowMinSizeX;
    }

    unsigned int childWindowSizeY = questionSizeY + yesButton.getSize().y + 3 * kChildWindowIndent;

    if (childWindowSizeY < kChildWindowMinSizeY) {
        childWindowSizeY = kChildWindowMinSizeY;
    }

    // создание дочернего окна с дихотомическим вопросом
    sf::RenderTexture childWindow(sf::Vector2u(childWindowSizeX, childWindowSizeY));

    sf::RectangleShape childWindowBackground(sf::Vector2f(childWindowSizeX, childWindowSizeY));
    childWindowBackground.setTexture(&backgroundTexture);

    sf::Sprite childWindowSprite(childWindow.getTexture());
    childWindowSprite.setPosition(sf::Vector2f((window.getSize().x - childWindowSizeX) / 2, (window.getSize().y - childWindowSizeY) / 2));

    // размещение вопроса
    float questionCoordinateX = (childWindowSizeX - questionSizeX) / 2;
    question.setPosition(sf::Vector2f(questionCoordinateX, kChildWindowIndent));

    // размещение кнопки да
    float yesButtonCoordinateX = (childWindowSizeX - yesButton.getSize().x) / 4;
    float yesButtonCoordinateY = questionSizeY + 2 * kChildWindowIndent;
    yesButton.setPosition(sf::Vector2f(yesButtonCoordinateX, yesButtonCoordinateY));

    // размещение кнопки нет
    noButton.setPosition(sf::Vector2f(3 * yesButtonCoordinateX, yesButtonCoordinateY));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return true;
            }
        }

        window.clear();

        yesButton.setTextFillColor(kBrown);
        noButton.setTextFillColor(kBrown);

        yesButton.cancelSelection();
        noButton.cancelSelection();

        if (yesButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - childWindowSprite.getPosition())) {
            yesButton.setTextFillColor(kGreen);
            yesButton.select();
        }

        if (noButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - childWindowSprite.getPosition())) {
            noButton.setTextFillColor(kRed);
            noButton.select();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (yesButton.isSelected()) {
                return true;
            }

            if (noButton.isSelected()) {
                return false;
            }
        }

        childWindow.clear(sf::Color::Transparent);

        childWindow.draw(childWindowBackground);
        childWindow.draw(question);

        yesButton.draw(childWindow);
        noButton.draw(childWindow);

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

    // загрузка картинки заднего фона в текстуру
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../images/IMG_0801.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры кнопки
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("../images/IMG_0793.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создание лейбла с информацией
    sf::Text information(helveticaFont);
    information.setString(text);
    information.setFillColor(kBrown);
    information.setCharacterSize(kChildWindowMaxCharacterSize);

    // создаём крутую кнопку продолжить
    Button continueButton(helveticaFont);
    continueButton.setString(L"Продолжить");
    continueButton.setTextFillColor(kBrown);
    continueButton.setCharacterSize(kChildWindowMaxCharacterSize);
    // continueButton.setSize(sf::Vector2f(kChildWindowButtonSizeX, kChildWindowButtonSizeY));
    continueButton.setTexture(&buttonTexture);

    float informationSizeY = information.getLocalBounds().getCenter().y * 2;

    while (informationSizeY > window.getSize().y - 3 * kChildWindowIndent - continueButton.getSize().y) {
        information.setCharacterSize(information.getCharacterSize() / 2);
        informationSizeY = information.getLocalBounds().getCenter().y * 2;
    }

    float informationSizeX = information.getLocalBounds().getCenter().x * 2;

    unsigned int childWindowSizeX = informationSizeX + 2 * kChildWindowIndent;

    if (childWindowSizeX < kChildWindowMinSizeX) {
        childWindowSizeX = kChildWindowMinSizeX;
    }

    unsigned int childWindowSizeY = informationSizeY + continueButton.getSize().y + 3 * kChildWindowIndent;

    if (childWindowSizeY < kChildWindowMinSizeY) {
        childWindowSizeY = kChildWindowMinSizeY;
    }

    // создание дочернего окна с информацией
    sf::RenderTexture childWindow(sf::Vector2u(childWindowSizeX, childWindowSizeY));

    sf::RectangleShape childWindowBackground(sf::Vector2f(childWindowSizeX, childWindowSizeY));
    childWindowBackground.setTexture(&backgroundTexture);

    sf::Sprite childWindowSprite(childWindow.getTexture());
    childWindowSprite.setPosition(sf::Vector2f((window.getSize().x - childWindowSizeX) / 2, (window.getSize().y - childWindowSizeY) / 2));

    // размещение информации
    float informationCoordinateX = (childWindowSizeX - informationSizeX) / 2;
    information.setPosition(sf::Vector2f(informationCoordinateX, kChildWindowIndent));

    // размещение кнопки продолжить
    float continueButtonCoordinateX = (childWindowSizeX - continueButton.getSize().x) / 2;
    float continueButtonCoordinateY = informationSizeY + 2 * kChildWindowIndent;
    continueButton.setPosition(sf::Vector2f(continueButtonCoordinateX, continueButtonCoordinateY));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
        }

        window.clear();

        continueButton.setTextFillColor(kBrown);
        continueButton.cancelSelection();

        if (continueButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - childWindowSprite.getPosition())) {
            continueButton.setTextFillColor(kBlue);
            continueButton.select();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (continueButton.isSelected()) {
                return;
            }
        }

        childWindow.clear(sf::Color::Transparent);

        childWindow.draw(childWindowBackground);
        childWindow.draw(information);
        continueButton.draw(childWindow);

        childWindow.display();

        window.draw(windowCurrentStateSprite);
        window.draw(childWindowSprite);

        window.display();
    }
}

sf::String GetRulesFromFile() {
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
    return sf::String::fromUtf8(text.begin(), text.end());
}
}  // namespace

namespace SeaBattleExecutor {
void StartGame(sf::RenderWindow& window) {
    bool gameContinueExecution = true;
    bool playerMove = true;

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }

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

    // // загрузка текстуры для кнопки выхода из игры
    // sf::Texture exitButtonTexture;
    // if (!exitButtonTexture.loadFromFile("../images/exit_image.png")) {
    //     throw std::runtime_error("failed to load image");
    // }

    // загрузка текстуры кнопок
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("../images/IMG_0793.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создаём крутую кнопку сдаться
    Button giveUpButton(helveticaFont);
    giveUpButton.setString(L"Сдаться");
    giveUpButton.setTextFillColor(kBrown);
    giveUpButton.setCharacterSize(kGameGiveUpButtonCharacterSize);

    // float quitGameCoolButtonCoordinateX = (window.getSize().x - quitGameCoolButton.getSize().x) / 2;

    giveUpButton.setPosition(sf::Vector2f(kGameGiveUpButtonCoordinateX, kGameGiveUpButtonCoordinateY));
    giveUpButton.setTexture(&buttonTexture);
    
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
    bool hittedShip = false;
    int xHit = 0;
    int yHit = 0;

    auto DrawAllEntitiesInWindow = [] (sf::RenderWindow& window, auto& playerShapeMatrix, auto& robotShapeMatrix, auto& giveUpButton) {
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

        giveUpButton.draw(window);
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

        giveUpButton.setTextFillColor(kBrown);
        giveUpButton.cancelSelection();

        if (giveUpButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            giveUpButton.setTextFillColor(kBlue);
            giveUpButton.select();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (giveUpButton.isSelected()) {
                giveUpButton.setTextFillColor(kBrown);

                DrawAllEntitiesInWindow(window, playerShapeMatrix, robotShapeMatrix, giveUpButton);

                sf::Texture windowCurrentStateTexture(window.getSize());
                windowCurrentStateTexture.update(window);
                sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);
                
                if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, L"Вы уверены, что \nхотите сдаться?")) {
                    ShowChildWindowWithInformation(window, windowCurrentStateSprite, L"Облава! Вы проиграли :(");
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
            if (!hittedShip) {
                ShootResult shootResult = randomShoot(playerBoard, robotShootX, robotShootY);

                if (shootResult == ShootResult::Miss) {
                    playerMove = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }

                if (shootResult == ShootResult::Hit) {
                    xHit = robotShootX;
                    yHit = robotShootY;
                    hittedShip = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }

                if (shootResult == ShootResult::Kill) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }
            }

            if (hittedShip) {
                ShootResult shootResult = smartShoot(playerBoard, xHit, yHit, robotShootX, robotShootY);

                if (shootResult == ShootResult::Miss) {
                    playerMove = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }

                if (shootResult == ShootResult::Hit) {
                    xHit = robotShootX;
                    yHit = robotShootY;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }

                if (shootResult == ShootResult::Kill) {
                    hittedShip = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }
            }
        } else if (playerMove) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (numi >= 0 && numj >= 0) {
                    if (robotBoard.shoot(numi, numj) == ShootResult::Miss) {
                        playerMove = false;
                    }
                }
            }
        }
        
        
        

        DrawAllEntitiesInWindow(window, playerShapeMatrix, robotShapeMatrix, giveUpButton);

        window.display();
    }

    sf::Texture windowCurrentStateTexture(window.getSize());
    windowCurrentStateTexture.update(window);
    sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

    if (robotBoard.allShipsSunk()) {
        ShowChildWindowWithInformation(window, windowCurrentStateSprite, L"Поздравляем! Вы выиграли :)");
        return;
    }

    ShowChildWindowWithInformation(window, windowCurrentStateSprite, L"Облава! Вы проиграли :(");
}

void Menu(sf::RenderWindow& window) {
    // загрузка картинки заднего фона в текстуру
    sf::Texture imageBackground;
    if (!imageBackground.loadFromFile("../images/IMG_0799.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры кнопок
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("../images/IMG_0793.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape backGround(static_cast<sf::Vector2f>(window.getSize()));
    backGround.setTexture(&imageBackground);



    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // создаём лейбл для названия игры
    sf::Text titul(helveticaFont, sf::String(L"Морской бой"));
    titul.setFillColor(kBlue);
    titul.setCharacterSize(kMenuTitulCharacterSize);
    titul.setOutlineThickness(2);
    titul.setOutlineColor(sf::Color::White);

    float titulCoordinateX = (window.getSize().x / 2) - titul.getGlobalBounds().getCenter().x;
    titul.setPosition(sf::Vector2f(titulCoordinateX, kMenuTitulCoordinateY));


    // создаём крутую кнопку, начинающую игру
    Button startGameButton(helveticaFont);
    startGameButton.setString(L"Начать игру");
    startGameButton.setTextFillColor(kBrown);
    startGameButton.setCharacterSize(kMenuButtonsCharacterSize);
    startGameButton.setSize(sf::Vector2f(kMenuButtonsSizeX, kMenuButtonsSizeY));
    
    float startGameButtonCoordinateX = (window.getSize().x - startGameButton.getSize().x) / 2;

    startGameButton.setPosition(sf::Vector2f(startGameButtonCoordinateX, kMenuStartGameButtonCoordinateY));
    startGameButton.setTexture(&buttonTexture);


    // создаём крутую кнопку, показывающую правила игры
    Button showRulesButton(helveticaFont);
    showRulesButton.setString(L"Посмотреть правила игры");
    showRulesButton.setTextFillColor(kBrown);
    showRulesButton.setCharacterSize(kMenuButtonsCharacterSize);
    showRulesButton.setSize(sf::Vector2f(kMenuButtonsSizeX, kMenuButtonsSizeY));

    float showRulesButtonCoordinateX = (window.getSize().x - showRulesButton.getSize().x) / 2;
    showRulesButton.setPosition(sf::Vector2f(showRulesButtonCoordinateX, kMenuShowRulesButtonCoordinateY));
    showRulesButton.setTexture(&buttonTexture);


    // создаём крутую кнопку, закрывающую игру
    Button quitGameCoolButton(helveticaFont);
    quitGameCoolButton.setString(L"Выйти из игры");
    quitGameCoolButton.setTextFillColor(kBrown);
    quitGameCoolButton.setCharacterSize(kMenuButtonsCharacterSize);
    quitGameCoolButton.setSize(sf::Vector2f(kMenuButtonsSizeX, kMenuButtonsSizeY));

    float quitGameCoolButtonCoordinateX = (window.getSize().x - quitGameCoolButton.getSize().x) / 2;

    quitGameCoolButton.setPosition(sf::Vector2f(quitGameCoolButtonCoordinateX, kMenuQuitGameButtonCoordinateY));
    quitGameCoolButton.setTexture(&buttonTexture);
    

    auto DrawAllEntitiesInWindow = [] (sf::RenderWindow& window, auto& backGround, auto& titul, auto& startGameButton, auto& showRulesButton, auto& quitGameCoolButton) {
        window.draw(backGround);
        window.draw(titul);

        startGameButton.draw(window);
        showRulesButton.draw(window);
        quitGameCoolButton.draw(window);
    };

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

        startGameButton.setTextFillColor(kBrown);
        showRulesButton.setTextFillColor(kBrown);
        quitGameCoolButton.setTextFillColor(kBrown);

        startGameButton.cancelSelection();
        showRulesButton.cancelSelection();
        quitGameCoolButton.cancelSelection();


        if (startGameButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            startGameButton.setTextFillColor(kBlue);
            startGameButton.select();
        }

        if (showRulesButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            showRulesButton.setTextFillColor(kBlue);
            showRulesButton.select();
        }

        if (quitGameCoolButton.getLocalBounds().contains(static_cast<sf::Vector2f>(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - quitGameCoolButton.getPosition()))) {
            quitGameCoolButton.setTextFillColor(kBlue);
            quitGameCoolButton.select();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (startGameButton.isSelected()) {
                return;
            }

            if (showRulesButton.isSelected()) {
                showRulesButton.setTextFillColor(kBlue);
                DrawAllEntitiesInWindow(window, backGround, titul, startGameButton, showRulesButton, quitGameCoolButton);

                windowCurrentStateTexture.update(window);
                windowCurrentStateSprite.setTexture(windowCurrentStateTexture);

                ShowChildWindowWithInformation(window, windowCurrentStateSprite, GetRulesFromFile());

                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }

            if (quitGameCoolButton.isSelected()) {
                quitGameCoolButton.setTextFillColor(kBrown);
                DrawAllEntitiesInWindow(window, backGround, titul,startGameButton, showRulesButton, quitGameCoolButton);

                windowCurrentStateTexture.update(window);
                windowCurrentStateSprite.setTexture(windowCurrentStateTexture);
                
                if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, L"Вы уверены, что \nхотите выйти из игры?")) {
                    window.close();
                    return;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
        }

        DrawAllEntitiesInWindow(window, backGround, titul,startGameButton, showRulesButton, quitGameCoolButton);     
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
