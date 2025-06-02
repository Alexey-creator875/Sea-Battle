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


// константы для расстановки кораблей
const int kBoardCellPixelSize = 45;
const int kDemonstrationCellPixelSize = 45;

const int kArrangementReturnButtonCoordinateX = 40;
const int kArrangementReturnButtonCoordinateY = 40;

const unsigned int kArrangementTitulCharacterSize = 60;
const unsigned int kArrangementCharacterSize = 25;

const int kRequiredShipsNumber = 10;
const int kShipsVariantsNumber = 4;
const int kShipsMaxSize = 4;

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

    // // загружаем шрифт
    // sf::Font helveticaFont;
    // if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
    //     throw std::runtime_error("failed to open file");
    // }

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/Myfont-Regular 3.otf")) {
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

    // // загружаем шрифт
    // sf::Font helveticaFont;
    // if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
    //     throw std::runtime_error("failed to open file");
    // }

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/Myfont-Regular 3.otf")) {
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
bool ArrangePlayerShips(sf::RenderWindow& window, Board& playerBoard) {
    // // загружаем шрифт
    // sf::Font helveticaFont;
    // if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
    //     throw std::runtime_error("failed to open file");
    // }

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/Myfont-Regular 3.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры для кнопок и лейблов
    sf::Texture parchmentTexture;
    if (!parchmentTexture.loadFromFile("../images/IMG_0793.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка картинки заднего фона в текстуру
    sf::Texture imageArrangementBackground;
    if (!imageArrangementBackground.loadFromFile("../images/IMG_0799.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape arrangementBackground(static_cast<sf::Vector2f>(window.getSize()));
    arrangementBackground.setTexture(&imageArrangementBackground);

    // загрузка текстуры неизведанной клетки
    sf::Texture waterTexture;
    if (!waterTexture.loadFromFile("../images/IMG_0756.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры неизведанной выбранной клетки
    sf::Texture waterSelectedTexture;
    if (!waterSelectedTexture.loadFromFile("../images/IMG_0758.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры корабля
    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile("../images/IMG_0760.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры поля игрока
    sf::Texture playerBoardTexture;
    if (!playerBoardTexture.loadFromFile("../images/IMG_0822.png")) {
        throw std::runtime_error("failed to load image");
    }

    // создание прямоугольника куда пихаем текстуру поля игрока
    sf::RectangleShape playerBoardBackground;
    playerBoardBackground.setSize(sf::Vector2f((kArraySize + 2) * kBoardCellPixelSize, (kArraySize + 5) * kBoardCellPixelSize));

    float playerBoardBackgroundCoordinateX = (window.getSize().x / 2 - playerBoardBackground.getSize().x) / 2;
    float playerBoardBackgroundCoordinateY = (window.getSize().y - playerBoardBackground.getSize().y) / 2;

    playerBoardBackground.setPosition(sf::Vector2f(playerBoardBackgroundCoordinateX, playerBoardBackgroundCoordinateY));
    playerBoardBackground.setTexture(&playerBoardTexture);

    // создаём крутую кнопку вернуться в меню
    Button returnButton(helveticaFont);
    returnButton.setString(L"Вернуться в меню");
    returnButton.setTextFillColor(kBrown);
    returnButton.setCharacterSize(kArrangementCharacterSize);

    returnButton.setPosition(sf::Vector2f(kArrangementReturnButtonCoordinateX, kArrangementReturnButtonCoordinateY));
    returnButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку сражаться
    Button fightButton(helveticaFont);
    fightButton.setString(L"Сражаться");
    fightButton.setTextFillColor(kBrown);
    fightButton.setCharacterSize(kArrangementCharacterSize);

    fightButton.setPosition(sf::Vector2f(kArrangementReturnButtonCoordinateX + 400, kArrangementReturnButtonCoordinateY));
    fightButton.setTexture(&parchmentTexture);


    // создаём крутую кнопку сгенерировать рандомную расстановку
    Button randomLayoutButton(helveticaFont);
    randomLayoutButton.setString(L"Сгенерировать");
    randomLayoutButton.setTextFillColor(kBrown);
    randomLayoutButton.setCharacterSize(kArrangementCharacterSize);

    randomLayoutButton.setPosition(sf::Vector2f(kArrangementReturnButtonCoordinateX + 800, kArrangementReturnButtonCoordinateY));
    randomLayoutButton.setTexture(&parchmentTexture);


    // создаём крутую кнопку очистить борды
    Button clearButton(helveticaFont);
    clearButton.setString(L"Очистить");
    clearButton.setTextFillColor(kBrown);
    clearButton.setCharacterSize(kArrangementCharacterSize);

    clearButton.setPosition(sf::Vector2f(kArrangementReturnButtonCoordinateX + 1200, kArrangementReturnButtonCoordinateY));
    clearButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку поменять длину нынешнего корабля
    Button changeShipSizeButton(helveticaFont);
    changeShipSizeButton.setString(L"Изменить длину");
    changeShipSizeButton.setTextFillColor(kBrown);
    changeShipSizeButton.setCharacterSize(kArrangementCharacterSize);

    changeShipSizeButton.setPosition(sf::Vector2f(kArrangementReturnButtonCoordinateX + 800, kArrangementReturnButtonCoordinateY + 600));
    changeShipSizeButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку вращающую корабль
    Button rotateButton(helveticaFont);
    rotateButton.setString(L"Повернуть");
    rotateButton.setTextFillColor(kBrown);
    rotateButton.setCharacterSize(kArrangementCharacterSize);

    rotateButton.setPosition(sf::Vector2f(kArrangementReturnButtonCoordinateX + 1100, kArrangementReturnButtonCoordinateY + 600));
    rotateButton.setTexture(&parchmentTexture);

    // создаём поле с отображением нынешнего корабля
    sf::RectangleShape currentShipField;
    currentShipField.setSize(sf::Vector2f(6 * kDemonstrationCellPixelSize, 6 * kDemonstrationCellPixelSize));
    currentShipField.setTexture(&parchmentTexture);
    currentShipField.setPosition(sf::Vector2f(kArrangementReturnButtonCoordinateX + 800, kArrangementReturnButtonCoordinateY + 200));

    // создаём массив шейпов для нынешнего корабля
    std::array<sf::RectangleShape, kShipsMaxSize> shipShapeArray;
    for (size_t i = 0; i < kShipsMaxSize; ++i) {
        shipShapeArray[i].setPosition(sf::Vector2f((currentShipField.getPosition().x + kDemonstrationCellPixelSize) + i * kDemonstrationCellPixelSize, currentShipField.getPosition().y + kDemonstrationCellPixelSize));
        shipShapeArray[i].setSize(sf::Vector2f(kDemonstrationCellPixelSize, kDemonstrationCellPixelSize));
        shipShapeArray[i].setTexture(&shipTexture);
    }

    // создание массива шейпов для игрока
    std::array<std::array<sf::RectangleShape, kArraySize>, kArraySize> playerShapeMatrix;

    for (size_t i = 0; i < kArraySize; ++i) {
        for (size_t j = 0; j < kArraySize; ++j) {
            playerShapeMatrix[i][j].setPosition(sf::Vector2f((playerBoardBackgroundCoordinateX + kBoardCellPixelSize) + j * kBoardCellPixelSize, (playerBoardBackgroundCoordinateY + 4 * kBoardCellPixelSize) + i * kBoardCellPixelSize));
            playerShapeMatrix[i][j].setSize(sf::Vector2f(kBoardCellPixelSize, kBoardCellPixelSize));

            if (playerBoard.getCell(i, j) == CellStatus::Ship) {
                playerShapeMatrix[i][j].setTexture(&shipTexture);
            }

            if (playerBoard.getCell(i, j) == CellStatus::Empty) {
                playerShapeMatrix[i][j].setTexture(&waterTexture);
            }
        }
    }


    // playerBoard.autoPlaceShips();
    randomPlaceShips(playerBoard);

    auto DrawAllEntitiesInWindow = [] (auto& window, auto& arrangementBackground, auto& playerBoardBackground, auto& playerShapeMatrix, auto& returnButton, auto& fightButton, auto& randomLayoutButton, auto& clearButton, auto& changeLengthButton, auto& rotateButton, auto& currentShipField, auto& shipShapeArray) {
        window.draw(arrangementBackground);

        window.draw(playerBoardBackground);
        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                window.draw(playerShapeMatrix[i][j]);
            }
        }

        returnButton.draw(window);
        fightButton.draw(window);
        randomLayoutButton.draw(window);
        clearButton.draw(window);
        changeLengthButton.draw(window);
        rotateButton.draw(window);

        window.draw(currentShipField);
        for (size_t i = 0; i < kShipsMaxSize; ++i) {
            window.draw(shipShapeArray[i]);
        }
    };

    auto UpdatePlayerShapeMatrix = [waterTexture, shipTexture] (auto& playerShapeMatrix, auto& playerBoard) {
        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                if (playerBoard.getCell(i, j) == CellStatus::Empty) {
                    playerShapeMatrix[i][j].setTexture(&waterTexture);
                }

                if (playerBoard.getCell(i, j) == CellStatus::Ship) {
                    playerShapeMatrix[i][j].setTexture(&shipTexture);
                }
            }
        }
    };

    int currentShipSize = kShipsMaxSize;
    bool horizontally = true;

    int selectedCellX = -1;
    int selectedCellY = -1;

    auto UpdateShipArray = [currentShipField] (auto& shipShapeArray, auto& currentShipSize, auto& horizontally) {
        for (size_t i = 0; i < currentShipSize; ++i) {
            shipShapeArray[i].setFillColor(sf::Color::White);
        }

        for (size_t i = currentShipSize; i < kShipsMaxSize; ++i) {
            shipShapeArray[i].setFillColor(sf::Color::Transparent);
        }

        for (size_t i = 1; i < kShipsMaxSize; ++i) {
            sf::Vector2f positionRelatedToField = (horizontally)? sf::Vector2f((i + 1) * kDemonstrationCellPixelSize, kDemonstrationCellPixelSize) : sf::Vector2f(kDemonstrationCellPixelSize, (i + 1) * kDemonstrationCellPixelSize);

            shipShapeArray[i].setPosition(currentShipField.getPosition() + positionRelatedToField);
        }
    };

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return false;
            }
        }

        window.clear();

        int selectedCellX = -1;
        int selectedCellY = -1;

        returnButton.setTextFillColor(kBrown);
        returnButton.cancelSelection();

        fightButton.setTextFillColor(kBrown);
        fightButton.cancelSelection();

        randomLayoutButton.setTextFillColor(kBrown);
        randomLayoutButton.cancelSelection();

        clearButton.setTextFillColor(kBrown);
        clearButton.cancelSelection();

        changeShipSizeButton.setTextFillColor(kBrown);
        changeShipSizeButton.cancelSelection();

        rotateButton.setTextFillColor(kBrown);
        rotateButton.cancelSelection();

        UpdatePlayerShapeMatrix(playerShapeMatrix, playerBoard);
        UpdateShipArray(shipShapeArray, currentShipSize, horizontally);

        if (returnButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            returnButton.setTextFillColor(kBlue);
            returnButton.select();
        }

        if (fightButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            fightButton.setTextFillColor(kBlue);
            fightButton.select();
        }

        if (randomLayoutButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            randomLayoutButton.setTextFillColor(kBlue);
            randomLayoutButton.select();
        }

        if (clearButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            clearButton.setTextFillColor(kBlue);
            clearButton.select();
        }

        if (changeShipSizeButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            changeShipSizeButton.setTextFillColor(kBlue);
            changeShipSizeButton.select();
        }

        if (rotateButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            rotateButton.setTextFillColor(kBlue);
            rotateButton.select();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (returnButton.isSelected()) {
                returnButton.setTextFillColor(kBrown);

                DrawAllEntitiesInWindow(window, arrangementBackground, playerBoardBackground, playerShapeMatrix, returnButton, fightButton, randomLayoutButton, clearButton, changeShipSizeButton, rotateButton, currentShipField, shipShapeArray);

                sf::Texture windowCurrentStateTexture(window.getSize());
                windowCurrentStateTexture.update(window);
                sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

                if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, L"Вы уверены, что \nхотите вернуться в меню?")) {
                    return false;
                }
            }

            if (fightButton.isSelected()) {
                return true;
            }

            if (randomLayoutButton.isSelected()) {
                randomPlaceShips(playerBoard);
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            if (clearButton.isSelected()) {
                playerBoard.clearBoard();
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            if (changeShipSizeButton.isSelected()) {
                currentShipSize = currentShipSize % kShipsMaxSize + 1;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            if (rotateButton.isSelected()) {
                horizontally = !horizontally;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
        }

        DrawAllEntitiesInWindow(window, arrangementBackground, playerBoardBackground, playerShapeMatrix, returnButton, fightButton, randomLayoutButton, clearButton, changeShipSizeButton, rotateButton, currentShipField, shipShapeArray);

        window.display();
    }

    return true;
}


void StartGame(sf::RenderWindow& window) {
    bool gameContinueExecution = true;
    bool playerMove = true;

    // загружаем шрифт
    sf::Font helveticaFont;
    if (!helveticaFont.openFromFile("../images/Myfont-Regular 3.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // // загружаем шрифт
    // sf::Font helveticaFont;
    // if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
    //     throw std::runtime_error("failed to open file");
    // }

    // загрузка картинки заднего фона в текстуру
    sf::Texture imageGameBackground;
    if (!imageGameBackground.loadFromFile("../images/IMG_0799.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape gameBackground(static_cast<sf::Vector2f>(window.getSize()));
    gameBackground.setTexture(&imageGameBackground);

    // загрузка текстуры неизведанной клетки
    sf::Texture waterTexture;
    if (!waterTexture.loadFromFile("../images/IMG_0756.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры неизведанной выбранной клетки
    sf::Texture waterSelectedTexture;
    if (!waterSelectedTexture.loadFromFile("../images/IMG_0758.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры пустой клетки
    sf::Texture voidTexture;
    if (!voidTexture.loadFromFile("../images/IMG_0761.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры корабля
    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile("../images/IMG_0760.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры подбитого корабля
    sf::Texture hittedShipTexture;
    if (!hittedShipTexture.loadFromFile("../images/IMG_0759.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры поля игрока
    sf::Texture playerBoardTexture;
    if (!playerBoardTexture.loadFromFile("../images/IMG_0818.png")) {
        throw std::runtime_error("failed to load image");
    }

    // создание прямоугольника куда пихаем текстуру поля игрока
    sf::RectangleShape playerBoardBackground;
    playerBoardBackground.setSize(sf::Vector2f((kArraySize + 2) * kCellPixelSize, (kArraySize + 5) * kCellPixelSize));

    float playerBoardBackgroundCoordinateX = (window.getSize().x / 2 - playerBoardBackground.getSize().x) / 2;
    float playerBoardBackgroundCoordinateY = (window.getSize().y - playerBoardBackground.getSize().y) / 2;

    playerBoardBackground.setPosition(sf::Vector2f(playerBoardBackgroundCoordinateX, playerBoardBackgroundCoordinateY));
    playerBoardBackground.setTexture(&playerBoardTexture);

    // загрузка текстуры поля робота
    sf::Texture robotBoardTexture;
    if (!robotBoardTexture.loadFromFile("../images/IMG_0819.png")) {
        throw std::runtime_error("failed to load image");
    }

    // создание прямоугольника куда пихаем текстуру поля робота
    sf::RectangleShape robotBoardBackground;
    robotBoardBackground.setSize(sf::Vector2f((kArraySize + 2) * kCellPixelSize, (kArraySize + 5) * kCellPixelSize));

    float robotBoardBackgroundCoordinateX = (3 * window.getSize().x / 2 - robotBoardBackground.getSize().x) / 2;
    float robotBoardBackgroundCoordinateY = (window.getSize().y - robotBoardBackground.getSize().y) / 2;

    robotBoardBackground.setPosition(sf::Vector2f(robotBoardBackgroundCoordinateX, robotBoardBackgroundCoordinateY));
    robotBoardBackground.setTexture(&robotBoardTexture);


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

    giveUpButton.setPosition(sf::Vector2f(kGameGiveUpButtonCoordinateX, kGameGiveUpButtonCoordinateY));
    giveUpButton.setTexture(&buttonTexture);
    
    // создание борда для игрока
    Board playerBoard;
    if (!ArrangePlayerShips(window, playerBoard)) {
        return;
    }

    // создание массива шейпов для игрока
    std::array<std::array<sf::RectangleShape, kArraySize>, kArraySize> playerShapeMatrix;

    for (size_t i = 0; i < kArraySize; ++i) {
        for (size_t j = 0; j < kArraySize; ++j) {
            playerShapeMatrix[i][j].setPosition(sf::Vector2f((playerBoardBackgroundCoordinateX + kCellPixelSize) + j * kCellPixelSize, (playerBoardBackgroundCoordinateY + 4 * kCellPixelSize) + i * kCellPixelSize));
            playerShapeMatrix[i][j].setSize(sf::Vector2f(kCellPixelSize, kCellPixelSize));

            if (playerBoard.getCell(i, j) == CellStatus::Ship) {
                playerShapeMatrix[i][j].setTexture(&shipTexture);
            }

            if (playerBoard.getCell(i, j) == CellStatus::Empty) {
                playerShapeMatrix[i][j].setTexture(&waterTexture);
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
            robotShapeMatrix[i][j].setPosition(sf::Vector2f((robotBoardBackgroundCoordinateX + kCellPixelSize) + j * kCellPixelSize, (robotBoardBackgroundCoordinateY + 4 * kCellPixelSize) + i * kCellPixelSize));
            robotShapeMatrix[i][j].setSize(sf::Vector2f(kCellPixelSize, kCellPixelSize));
            robotShapeMatrix[i][j].setTexture(&waterTexture);
        }
    }

    int numi = -1;
    int numj = -1;

    int robotShootX = 0;
    int robotShootY = 0;
    bool hittedShip = false;
    int xHit = 0;
    int yHit = 0;

    auto DrawAllEntitiesInWindow = [] (sf::RenderWindow& window, auto& gameBackground, auto& playerBoardBackground, auto& robotBoardBackground, auto& playerShapeMatrix, auto& robotShapeMatrix, auto& giveUpButton) {
        window.draw(gameBackground);
        window.draw(playerBoardBackground);
        window.draw(robotBoardBackground);

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

    auto UpdatePlayerShapeMatrix = [waterTexture, shipTexture, voidTexture, hittedShipTexture] (auto& playerShapeMatrix, auto& playerBoard) {
        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                if (playerBoard.getCell(i, j) == CellStatus::Empty) {
                    playerShapeMatrix[i][j].setTexture(&waterTexture);
                }

                if (playerBoard.getCell(i, j) == CellStatus::Ship) {
                    playerShapeMatrix[i][j].setTexture(&shipTexture);
                }

                if (playerBoard.getCell(i, j) == CellStatus::ShootedEmpty) {
                    playerShapeMatrix[i][j].setTexture(&voidTexture);
                }

                if (playerBoard.getCell(i, j) == CellStatus::ShootedShip) {
                    playerShapeMatrix[i][j].setTexture(&hittedShipTexture);
                }
            }
        }
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

        window.clear();

        giveUpButton.setTextFillColor(kBrown);
        giveUpButton.cancelSelection();

        // очистка клеток поля робота
        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                if (!robotBoard.isShooted(i, j)) {
                    robotShapeMatrix[i][j].setTexture(&waterTexture);
                    continue;
                }

                if (robotBoard.getCell(i, j) == CellStatus::ShootedEmpty) {
                    robotShapeMatrix[i][j].setTexture(&voidTexture);
                }

                if (robotBoard.getCell(i, j) == CellStatus::ShootedShip) {
                    robotShapeMatrix[i][j].setTexture(&hittedShipTexture);
                }
            }
        }

        if (giveUpButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            giveUpButton.setTextFillColor(kBlue);
            giveUpButton.select();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (giveUpButton.isSelected()) {
                giveUpButton.setTextFillColor(kBrown);

                DrawAllEntitiesInWindow(window, gameBackground, playerBoardBackground, robotBoardBackground, playerShapeMatrix, robotShapeMatrix, giveUpButton);

                sf::Texture windowCurrentStateTexture(window.getSize());
                windowCurrentStateTexture.update(window);
                sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);
                
                if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, L"Вы уверены, что \nхотите сдаться?")) {
                    ShowChildWindowWithInformation(window, windowCurrentStateSprite, L"Поражение! В следующий раз получится :(");
                    return;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(150));
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

            if (numi >= 0 && numj >= 0) {
                robotShapeMatrix[numi][numj].setTexture(&waterSelectedTexture);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (numi >= 0 && numj >= 0) {
                    if (robotBoard.shoot(numi, numj) == ShootResult::Miss) {
                        playerMove = false;
                    }
                }
            }
        } else if (!playerMove) {
            ShootResult shootResult = (hittedShip)? smartShoot(playerBoard, xHit, yHit, robotShootX, robotShootY) : randomShoot(playerBoard, robotShootX, robotShootY);
            UpdatePlayerShapeMatrix(playerShapeMatrix, playerBoard);

            switch(shootResult) {
                case ShootResult::Miss:
                    playerMove = true;
                    break;
                case ShootResult::Hit:
                    xHit = robotShootX;
                    yHit = robotShootY;
                    hittedShip = true;
                    break;
                case ShootResult::Kill:
                    hittedShip = false;
                    break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
        DrawAllEntitiesInWindow(window, gameBackground, playerBoardBackground, robotBoardBackground, playerShapeMatrix, robotShapeMatrix, giveUpButton);

        window.display();
    }

    sf::Texture windowCurrentStateTexture(window.getSize());
    windowCurrentStateTexture.update(window);
    sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

    if (robotBoard.allShipsSunk()) {
        ShowChildWindowWithInformation(window, windowCurrentStateSprite, L"Победа! Поздравляем :)");
        return;
    }

    ShowChildWindowWithInformation(window, windowCurrentStateSprite, L"Поражение! В следующий раз получится :(");
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
    if (!helveticaFont.openFromFile("../images/Myfont-Regular 3.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // // загружаем шрифт
    // sf::Font helveticaFont;
    // if (!helveticaFont.openFromFile("../images/helvetica_light.otf")) {
    //     throw std::runtime_error("failed to open file");
    // }

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
