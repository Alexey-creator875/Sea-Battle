#include "SeaBattleExecutor.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "GameClasses/Classes.h"
#include "Battlefield/Battlefield.h"
#include "CustomEntities/Button.h"
#include "Delay/Delay.h"

#include <stack>
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <fstream>
#include <array>
#include <map>


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

const unsigned int kArrangementTitulCharacterSize = 60;
const unsigned int kArrangementCharacterSize = 30;

const int kRequiredShipsNumber = 10;
const int kShipsMaxSize = 4;

const int kArrangementReturnButtonCoordinateX = 40;
const int kArrangementReturnButtonCoordinateY = 40;

const int kArrangementTitulCoordinateY = 40;

const int kArrangementIndent = 20;

const int kArrangementButtonSizeX = 200;
const int kArrangementButtonSizeY = 55;


// константы для игры
const unsigned int kArraySize = 10;

const int kCellPixelSize = 45;

const int kShootPromptCoordinateY = 40;

// константы для кнопки сдаться

const int kGameGiveUpButtonCoordinateX = 40;
const int kGameGiveUpButtonCoordinateY = 40;

const unsigned int kGameGiveUpButtonCharacterSize = 30;


// константы для дочерних окон
const unsigned int kChildWindowMinSizeX = 1050;
const unsigned int kChildWindowMinSizeY = 300;

const unsigned int kChildWindowMaxCharacterSize = 40;

const float kChildWindowButtonSizeX = 100.f;
const float kChildWindowButtonSizeY = 70.f;

// константы для дочернего окна с информацией

const unsigned int kChildWindowIndent = 40;

// константы для цветов
const sf::Color kBrown = sf::Color(70, 55, 45);
const sf::Color kBlue = sf::Color(70, 90, 230);
const sf::Color kGreen = sf::Color(65, 180, 30);
const sf::Color kRed = sf::Color(235, 65, 65);

// константы для задержек
const int kRobotMinDelay = 500;
const int kRobotMaxDelay = 1000;
const int kSystemDelay = 200;
const int kSoundDelay = 700;

// константы для звука
const unsigned int kDeffaultVolume = 50;


bool ShowChildWindowWithDichotomousQuestion(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite, const sf::String& text) {
    Delay::Delay(kSystemDelay);

    // загрузка звука нажатия кнопки
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../assets/Music/meowSound.mp3")) {
        throw std::runtime_error("failed to open file");
    }

    sf::Sound pushButtonSound(soundBuffer);
    pushButtonSound.setVolume(kDeffaultVolume);

    // загружаем шрифт
    sf::Font myFont;
    if (!myFont.openFromFile("../assets/Fonts/MyFont.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка картинки заднего фона в текстуру
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../assets/images/scroll.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры кнопки да
    sf::Texture yesButtonTexture;
    if (!yesButtonTexture.loadFromFile("../assets/Images/parchmentGreen.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры кнопки нет
    sf::Texture noButtonTexture;
    if (!noButtonTexture.loadFromFile("../assets/Images/parchmentRed.png")) {
        throw std::runtime_error("failed to open file");
    }
    
    
    // создание лейбла с вопросом
    sf::Text question(myFont);
    question.setFillColor(kBrown);
    question.setOutlineColor(sf::Color::White);

    question.setCharacterSize(kChildWindowMaxCharacterSize);
    question.setString(text);


    // создаём крутую кнопку да
    Button yesButton(myFont);
    yesButton.setString(L"Да");
    yesButton.setTextFillColor(kBrown);
    yesButton.setCharacterSize(kChildWindowMaxCharacterSize);
    yesButton.setSize(sf::Vector2f(kChildWindowButtonSizeX, kChildWindowButtonSizeY));
    yesButton.setTexture(&yesButtonTexture);


    // создаём крутую кнопку нет
    Button noButton(myFont);
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
                pushButtonSound.play();
                Delay::Delay(kSoundDelay);
                return true;
            }

            if (noButton.isSelected()) {
                pushButtonSound.play();
                Delay::Delay(kSoundDelay);
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
    Delay::Delay(kSystemDelay);

    // загрузка звука нажатия кнопки
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../assets/Music/meowSound.mp3")) {
        throw std::runtime_error("failed to open file");
    }

    sf::Sound pushButtonSound(soundBuffer);
    pushButtonSound.setVolume(kDeffaultVolume);

    // загружаем шрифт
    sf::Font myFont;
    if (!myFont.openFromFile("../assets/Fonts/MyFont.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка картинки заднего фона в текстуру
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../assets/images/scroll.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры кнопки
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("../assets/images/parchment.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создание лейбла с информацией
    sf::Text information(myFont);
    information.setString(text);
    information.setFillColor(kBrown);
    information.setCharacterSize(kChildWindowMaxCharacterSize);

    // создаём крутую кнопку продолжить
    Button continueButton(myFont);
    continueButton.setString(L"Продолжить");
    continueButton.setTextFillColor(kBrown);
    continueButton.setCharacterSize(kChildWindowMaxCharacterSize);
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
                pushButtonSound.play();
                Delay::Delay(kSoundDelay);
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

    std::ifstream rulesFile("../assets/Texts/rules.txt");

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

sf::String TranslateIndicesToBoardCoordinates(int x, int y) {
    if (x < 0 || x >= kArraySize || y < 0 || y >= kArraySize) {
        throw std::runtime_error("index out of boundary");
    }

    sf::String letters = L"абвгдежзик";
    sf::String boardCoordinates = letters[y];
    boardCoordinates += std::to_wstring(x + 1);
    return boardCoordinates;

}

struct GameStatistics {
    int shootsNumber = 0;
    int killedShipsNumber = 0;
    int unbrokenShipsNumber = kRequiredShipsNumber;
};
}  // namespace

namespace SeaBattleExecutor {
bool ArrangePlayerShips(sf::RenderWindow& window, Board& playerBoard) {
    // загрузка звука нажатия кнопки
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../assets/Music/meowSound.mp3")) {
        throw std::runtime_error("failed to open file");
    }

    sf::Sound pushButtonSound(soundBuffer);
    pushButtonSound.setVolume(kDeffaultVolume);

    // загружаем шрифт
    sf::Font myFont;
    if (!myFont.openFromFile("../assets/Fonts/MyFont.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры для кнопок и лейблов
    sf::Texture parchmentTexture;
    if (!parchmentTexture.loadFromFile("../assets/images/parchment.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка картинки заднего фона в текстуру
    sf::Texture imageArrangementBackground;
    if (!imageArrangementBackground.loadFromFile("../assets/images/background.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape arrangementBackground(static_cast<sf::Vector2f>(window.getSize()));
    arrangementBackground.setTexture(&imageArrangementBackground);

    // загрузка текстуры неизведанной клетки
    sf::Texture waterTexture;
    if (!waterTexture.loadFromFile("../assets/Images/water.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры неизведанной выбранной клетки
    sf::Texture waterSelectedTexture;
    if (!waterSelectedTexture.loadFromFile("../assets/Images/waterSelected.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры корабля
    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile("../assets/Images/ship.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры поля игрока
    sf::Texture playerBoardTexture;
    if (!playerBoardTexture.loadFromFile("../assets/Images/playerBackground.png")) {
        throw std::runtime_error("failed to load image");
    }

    // создание прямоугольника куда пихаем текстуру поля игрока
    sf::RectangleShape playerBoardBackground;
    playerBoardBackground.setSize(sf::Vector2f((kArraySize + 2) * kBoardCellPixelSize, (kArraySize + 5) * kBoardCellPixelSize));

    float playerBoardBackgroundCoordinateX = (window.getSize().x / 2 - playerBoardBackground.getSize().x) / 2;
    float playerBoardBackgroundCoordinateY = (window.getSize().y - playerBoardBackground.getSize().y) / 2;

    playerBoardBackground.setPosition(sf::Vector2f(playerBoardBackgroundCoordinateX, playerBoardBackgroundCoordinateY));
    playerBoardBackground.setTexture(&playerBoardTexture);

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

    sf::Text arrangementTitul(myFont);
    arrangementTitul.setCharacterSize(kArrangementTitulCharacterSize);
    arrangementTitul.setString(L"Расстановка кораблей");
    arrangementTitul.setFillColor(kBlue);
    arrangementTitul.setOutlineColor(sf::Color::White);
    arrangementTitul.setOutlineThickness(2);

    float arrangementTitulCoordinateX = window.getSize().x / 2 - arrangementTitul.getLocalBounds().getCenter().x;
    arrangementTitul.setPosition(sf::Vector2f(arrangementTitulCoordinateX, kArrangementTitulCoordinateY));

    // создаём поле с отображением нынешнего корабля
    sf::RectangleShape currentShipField;
    currentShipField.setSize(sf::Vector2f(6 * kDemonstrationCellPixelSize, 6 * kDemonstrationCellPixelSize));
    currentShipField.setTexture(&parchmentTexture);

    float currentShipFieldCoordinateX = window.getSize().x / 2 + kArrangementIndent;
    float currentShipFieldCoordinateY = playerBoardBackgroundCoordinateY + 3 * kBoardCellPixelSize;

    currentShipField.setPosition(sf::Vector2f(currentShipFieldCoordinateX, currentShipFieldCoordinateY));

    // создаём крутую кнопку вернуться в меню
    Button returnButton(myFont);
    returnButton.setString(L"Вернуться в меню");
    returnButton.setTextFillColor(kBrown);
    returnButton.setCharacterSize(kArrangementCharacterSize);

    returnButton.setPosition(sf::Vector2f(kArrangementReturnButtonCoordinateX, kArrangementReturnButtonCoordinateY));
    returnButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку поменять длину нынешнего корабля
    Button changeShipSizeButton(myFont);
    changeShipSizeButton.setString(L"Изменить длину");
    changeShipSizeButton.setTextFillColor(kBrown);
    changeShipSizeButton.setCharacterSize(kArrangementCharacterSize);
    changeShipSizeButton.setSize(sf::Vector2f(kArrangementButtonSizeX, kArrangementButtonSizeY));

    float changeShipSizeButtonCoordinateX = currentShipFieldCoordinateX;
    float changeShipSizeButtonCoordinateY = currentShipFieldCoordinateY + currentShipField.getSize().y + kArrangementIndent;

    changeShipSizeButton.setPosition(sf::Vector2f(changeShipSizeButtonCoordinateX, changeShipSizeButtonCoordinateY));
    changeShipSizeButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку вращающую корабль
    Button rotateButton(myFont);
    rotateButton.setString(L"Повернуть");
    rotateButton.setTextFillColor(kBrown);
    rotateButton.setCharacterSize(kArrangementCharacterSize);
    rotateButton.setSize(sf::Vector2f(kArrangementButtonSizeX, kArrangementButtonSizeY));

    float rotateButtonCoordinateX = changeShipSizeButtonCoordinateX + rotateButton.getSize().x + kArrangementIndent;
    float rotateButtonCoordinateY = changeShipSizeButtonCoordinateY;

    rotateButton.setPosition(sf::Vector2f(rotateButtonCoordinateX, rotateButtonCoordinateY));
    rotateButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку сгенерировать рандомную расстановку
    Button randomLayoutButton(myFont);
    randomLayoutButton.setString(L"Сгенерировать");
    randomLayoutButton.setTextFillColor(kBrown);
    randomLayoutButton.setCharacterSize(kArrangementCharacterSize);
    randomLayoutButton.setSize(sf::Vector2f(kArrangementButtonSizeX, kArrangementButtonSizeY));

    float randomLayoutButtonCoordinateX = changeShipSizeButtonCoordinateX;
    float randomLayoutButtonCoordinateY = changeShipSizeButtonCoordinateY + changeShipSizeButton.getSize().y + kArrangementIndent;

    randomLayoutButton.setPosition(sf::Vector2f(randomLayoutButtonCoordinateX, randomLayoutButtonCoordinateY));
    randomLayoutButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку очистить борды
    Button clearButton(myFont);
    clearButton.setString(L"Очистить");
    clearButton.setTextFillColor(kBrown);
    clearButton.setCharacterSize(kArrangementCharacterSize);
    clearButton.setSize(sf::Vector2f(kArrangementButtonSizeX, kArrangementButtonSizeY));

    float clearButtonCoordinateX = randomLayoutButtonCoordinateX + randomLayoutButton.getSize().x + kArrangementIndent;
    float clearButtonCoordinateY = randomLayoutButtonCoordinateY;

    clearButton.setPosition(sf::Vector2f(clearButtonCoordinateX, clearButtonCoordinateY));
    clearButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку отменяющую размещение корабля
    Button undoButton(myFont);
    undoButton.setString(L"Отменить");
    undoButton.setTextFillColor(kBrown);
    undoButton.setCharacterSize(kArrangementCharacterSize);
    undoButton.setSize(sf::Vector2f(kArrangementButtonSizeX, kArrangementButtonSizeY));

    float undoButtonCoordinateX = randomLayoutButtonCoordinateX;
    float undoButtonCoordinateY = randomLayoutButtonCoordinateY + randomLayoutButton.getSize().y + kArrangementIndent;

    undoButton.setPosition(sf::Vector2f(undoButtonCoordinateX, undoButtonCoordinateY));
    undoButton.setTexture(&parchmentTexture);

    // создаём крутую кнопку сражаться
    Button fightButton(myFont);
    fightButton.setString(L"Сражаться");
    fightButton.setTextFillColor(kBrown);
    fightButton.setCharacterSize(kArrangementCharacterSize);
    fightButton.setSize(sf::Vector2f(kArrangementButtonSizeX, kArrangementButtonSizeY));

    float fightButtonCoordinateX = undoButtonCoordinateX + undoButton.getSize().x + kArrangementIndent;
    float fightButtonCoordinateY = undoButtonCoordinateY;

    fightButton.setPosition(sf::Vector2f(fightButtonCoordinateX, fightButtonCoordinateY));
    fightButton.setTexture(&parchmentTexture);

    // передвигаем поле в центр кнопок
    currentShipFieldCoordinateX += (changeShipSizeButton.getSize().x + rotateButton.getSize().x + kArrangementIndent - currentShipField.getSize().x) / 2;
    currentShipField.setPosition(sf::Vector2f(currentShipFieldCoordinateX, currentShipFieldCoordinateY));

    // создаём массив шейпов для нынешнего корабля
    std::array<sf::RectangleShape, kShipsMaxSize> shipShapeArray;
    for (size_t i = 0; i < kShipsMaxSize; ++i) {
        shipShapeArray[i].setPosition(sf::Vector2f((currentShipField.getPosition().x + kDemonstrationCellPixelSize) + i * kDemonstrationCellPixelSize, currentShipField.getPosition().y + kDemonstrationCellPixelSize));
        shipShapeArray[i].setSize(sf::Vector2f(kDemonstrationCellPixelSize, kDemonstrationCellPixelSize));
        shipShapeArray[i].setTexture(&shipTexture);
    }

    randomPlaceShips(playerBoard);

    auto DrawAllEntitiesInWindow = [] (auto& window, auto& arrangementBackground, auto& arrangementTitul, auto& playerBoardBackground, auto& playerShapeMatrix, auto& returnButton, auto& fightButton, auto& randomLayoutButton, auto& clearButton, auto& changeLengthButton, auto& rotateButton, auto& undoButton, auto& currentShipField, auto& shipShapeArray) {
        window.draw(arrangementBackground);

        window.draw(arrangementTitul);

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
        undoButton.draw(window);

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

    // int currentShipSize = kShipsMaxSize;
    // bool horizontally = true;

    int selectedCellX = -1;
    int selectedCellY = -1;

    Ship currentShip(selectedCellX, selectedCellY, kShipsMaxSize, true);

    std::map<int, int> shipsCountMap;

    auto ResetShipsCountMap = [] (auto& shipsCountMap) {
        for (int size = 1; size <= kShipsMaxSize; ++size) {
            shipsCountMap[size] = kShipsMaxSize - size + 1;
        }
    };

    auto FillShipsCountMap = [] (auto& shipsCountMap) {
        for (int size = 1; size <= kShipsMaxSize; ++size) {
            shipsCountMap[size] = 0;
        }
    };

    FillShipsCountMap(shipsCountMap);

    std::stack<Ship> shipsPlacementLog;

    auto UpdateShipArray = [currentShipField] (auto& shipShapeArray, auto& currentShip) {
        for (size_t i = 0; i < currentShip.getSize(); ++i) {
            shipShapeArray[i].setFillColor(sf::Color::White);
        }

        for (size_t i = currentShip.getSize(); i < kShipsMaxSize; ++i) {
            shipShapeArray[i].setFillColor(sf::Color::Transparent);
        }

        for (size_t i = 1; i < kShipsMaxSize; ++i) {
            sf::Vector2f positionRelatedToField = (currentShip.getPosition())? sf::Vector2f((i + 1) * kDemonstrationCellPixelSize, kDemonstrationCellPixelSize) : sf::Vector2f(kDemonstrationCellPixelSize, (i + 1) * kDemonstrationCellPixelSize);

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

        undoButton.setTextFillColor(kBrown);
        undoButton.cancelSelection();

        UpdatePlayerShapeMatrix(playerShapeMatrix, playerBoard);
        UpdateShipArray(shipShapeArray, currentShip);

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

        if (undoButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            undoButton.setTextFillColor(kBlue);
            undoButton.select();
        }

        if (playerBoard.countShip() < kRequiredShipsNumber) {
            for (size_t i = 0; i < kArraySize; ++i) {
                for (size_t j = 0; j < kArraySize; ++j) {
                    if (playerBoard.getCell(i, j) == CellStatus::Ship) {
                        continue;
                    }

                    if (playerShapeMatrix[i][j].getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                        selectedCellX = i;
                        selectedCellY = j;
                    }
                }
            }

            if (selectedCellX >= 0 && selectedCellY >= 0) {
                playerShapeMatrix[selectedCellX][selectedCellY].setTexture(&waterSelectedTexture);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (selectedCellX >= 0 && selectedCellY >= 0 && shipsCountMap[currentShip.getSize()] > 0) {
                    currentShip.setX(selectedCellX);
                    currentShip.setY(selectedCellY);

                    if (playerBoard.isValidPlacement(currentShip)) {
                        playerBoard.placeShip(currentShip);
                        shipsPlacementLog.push(currentShip);
                        --shipsCountMap[currentShip.getSize()];
                    }

                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (returnButton.isSelected()) {
                pushButtonSound.play();
                returnButton.setTextFillColor(kBrown);

                DrawAllEntitiesInWindow(window, arrangementBackground, arrangementTitul, playerBoardBackground, playerShapeMatrix, returnButton, fightButton, randomLayoutButton, clearButton, changeShipSizeButton, rotateButton, undoButton, currentShipField, shipShapeArray);

                sf::Texture windowCurrentStateTexture(window.getSize());
                windowCurrentStateTexture.update(window);
                sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

                if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, L"Вы уверены, что \nхотите вернуться в меню?")) {
                    return false;
                }
            }

            if (fightButton.isSelected() && playerBoard.countShip() == kRequiredShipsNumber) {
                pushButtonSound.play();
                Delay::Delay(kSoundDelay);
                return true;
            }

            if (randomLayoutButton.isSelected()) {
                pushButtonSound.play();
                randomPlaceShips(playerBoard);
                shipsPlacementLog = std::stack<Ship>();
                FillShipsCountMap(shipsCountMap);
            }

            if (clearButton.isSelected()) {
                pushButtonSound.play();
                playerBoard.clearBoard();
                shipsPlacementLog = std::stack<Ship>();
                ResetShipsCountMap(shipsCountMap);
            }

            if (changeShipSizeButton.isSelected()) {
                pushButtonSound.play();
                currentShip.setSize(currentShip.getSize() % kShipsMaxSize + 1);
            }

            if (rotateButton.isSelected()) {
                pushButtonSound.play();
                currentShip.setPosition(!currentShip.getPosition());
            }

            if (undoButton.isSelected() && !shipsPlacementLog.empty()) {
                pushButtonSound.play();
                playerBoard.deleteShip(shipsPlacementLog.top());
                ++shipsCountMap[shipsPlacementLog.top().getSize()];
                shipsPlacementLog.pop();
            }

            Delay::Delay(kSystemDelay);
        }

        DrawAllEntitiesInWindow(window, arrangementBackground, arrangementTitul, playerBoardBackground, playerShapeMatrix, returnButton, fightButton, randomLayoutButton, clearButton, changeShipSizeButton, rotateButton, undoButton, currentShipField, shipShapeArray);

        window.display();
    }

    return true;
}


void StartGame(sf::RenderWindow& window) {
    // загрузка звука нажатия кнопки
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../assets/Music/meowSound.mp3")) {
        throw std::runtime_error("failed to open file");
    }

    sf::Sound pushButtonSound(soundBuffer);
    pushButtonSound.setVolume(kDeffaultVolume);

    // загружаем шрифт
    sf::Font myFont;
    if (!myFont.openFromFile("../assets/Fonts/MyFont.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка картинки заднего фона в текстуру
    sf::Texture imageGameBackground;
    if (!imageGameBackground.loadFromFile("../assets/images/background.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape gameBackground(static_cast<sf::Vector2f>(window.getSize()));
    gameBackground.setTexture(&imageGameBackground);

    // загрузка текстуры неизведанной клетки
    sf::Texture waterTexture;
    if (!waterTexture.loadFromFile("../assets/Images/water.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры неизведанной выбранной клетки
    sf::Texture waterSelectedTexture;
    if (!waterSelectedTexture.loadFromFile("../assets/Images/waterSelected.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры пустой клетки
    sf::Texture voidTexture;
    if (!voidTexture.loadFromFile("../assets/Images/empty.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры корабля
    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile("../assets/Images/ship.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры подбитого корабля
    sf::Texture hittedShipTexture;
    if (!hittedShipTexture.loadFromFile("../assets/Images/shipHitted.png")) {
        throw std::runtime_error("failed to load image");
    }

    // загрузка текстуры поля игрока
    sf::Texture playerBoardTexture;
    if (!playerBoardTexture.loadFromFile("../assets/Images/playerBackground.png")) {
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
    if (!robotBoardTexture.loadFromFile("../assets/Images/enemyBackground.png")) {
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
    if (!buttonTexture.loadFromFile("../assets/images/parchment.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создаём крутую кнопку сдаться
    Button giveUpButton(myFont);
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
    Delay::Delay(kSystemDelay);

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

    // создание подсказки с информацией какой выстрел произведён
    Button shootPrompt(myFont);
    shootPrompt.setString(L"Удачной игры!");
    shootPrompt.setTextFillColor(kBrown);
    shootPrompt.setCharacterSize(kGameGiveUpButtonCharacterSize);
    shootPrompt.setTexture(&buttonTexture);

    auto CenterShootPrompt = [] (auto& shootPrompt, auto& window) {
        shootPrompt.setPosition(sf::Vector2f((window.getSize().x - shootPrompt.getSize().x) / 2, kShootPromptCoordinateY));
    };

    CenterShootPrompt(shootPrompt, window);

    bool gameContinueExecution = true;
    bool playerMove = true;

    int selectedCellX = -1;
    int selectedCellY = -1;

    int robotShootX = 0;
    int robotShootY = 0;
    bool hittedShip = false;
    int xHit = 0;
    int yHit = 0;

    GameStatistics playerStatistics{};

    auto DrawAllEntitiesInWindow = [] (sf::RenderWindow& window, auto& gameBackground, auto& shootPrompt, auto& playerBoardBackground, auto& robotBoardBackground, auto& playerShapeMatrix, auto& robotShapeMatrix, auto& giveUpButton) {
        window.draw(gameBackground);

        shootPrompt.draw(window);

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

    auto UpdateRobotShapeMatrix = [waterTexture, voidTexture, hittedShipTexture] (auto& robotShapeMatrix, auto& robotBoard) {
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
    };

    // раскрыть оставшиеся корабли
    auto RevealRobotBoard = [shipTexture] (auto& robotShapeMatrix, auto& robotBoard) {
        for (size_t i = 0; i < kArraySize; ++i) {
            for (size_t j = 0; j < kArraySize; ++j) {
                if (robotBoard.getCell(i, j) == CellStatus::Ship) {
                    robotShapeMatrix[i][j].setTexture(&shipTexture);
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

        window.clear();

        giveUpButton.setTextFillColor(kBrown);
        giveUpButton.cancelSelection();

        UpdateRobotShapeMatrix(robotShapeMatrix, robotBoard);

        if (playerBoard.allShipsSunk() || robotBoard.allShipsSunk()) {
            gameContinueExecution = false;
            RevealRobotBoard(robotShapeMatrix, robotBoard);
            DrawAllEntitiesInWindow(window, gameBackground, shootPrompt, playerBoardBackground, robotBoardBackground, playerShapeMatrix, robotShapeMatrix, giveUpButton);
            window.display();
            continue;
        }

        if (giveUpButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
            giveUpButton.setTextFillColor(kBlue);
            giveUpButton.select();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (giveUpButton.isSelected()) {
                pushButtonSound.play();
                giveUpButton.setTextFillColor(kBrown);

                DrawAllEntitiesInWindow(window, gameBackground, shootPrompt, playerBoardBackground, robotBoardBackground, playerShapeMatrix, robotShapeMatrix, giveUpButton);

                sf::Texture windowCurrentStateTexture(window.getSize());
                windowCurrentStateTexture.update(window);
                sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);
                
                if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, L"Вы уверены, что \nхотите сдаться?")) {
                    RevealRobotBoard(robotShapeMatrix, robotBoard);

                    DrawAllEntitiesInWindow(window, gameBackground, shootPrompt, playerBoardBackground, robotBoardBackground, playerShapeMatrix, robotShapeMatrix, giveUpButton);

                    windowCurrentStateTexture.update(window);
                    windowCurrentStateSprite.setTexture(windowCurrentStateTexture);

                    sf::String text = L"Поражение! В следующий раз получится :(";
                    text += L"\nКоличество выстрелов: " + std::to_wstring(playerStatistics.shootsNumber);
                    text += L"\nУничтожено кораблей: " +  std::to_wstring(playerStatistics.killedShipsNumber);
                    ShowChildWindowWithInformation(window, windowCurrentStateSprite, text);
                    return;
                }

                Delay::Delay(kSystemDelay);
            }
        }

        selectedCellX = -1;
        selectedCellY = -1;

        if (playerMove) {
            for (size_t i = 0; i < kArraySize; ++i) {
                for (size_t j = 0; j < kArraySize; ++j) {
                    if (robotBoard.isShooted(i, j)) {
                        continue;
                    }

                    if (robotShapeMatrix[i][j].getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                        selectedCellX = i;
                        selectedCellY = j;
                    }
                }
            }

            if (selectedCellX >= 0 && selectedCellY >= 0) {
                robotShapeMatrix[selectedCellX][selectedCellY].setTexture(&waterSelectedTexture);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (selectedCellX >= 0 && selectedCellY >= 0) {
                    ShootResult shootResult = robotBoard.shoot(selectedCellX, selectedCellY);
                    UpdateRobotShapeMatrix(robotShapeMatrix, robotBoard);
                    ++playerStatistics.shootsNumber;

                    switch(shootResult) {
                        case ShootResult::Miss:
                            playerMove = false;
                            break;
                        case ShootResult::Kill:
                            ++playerStatistics.killedShipsNumber;
                            break;
                        case ShootResult::Hit:
                            break;
                    }

                    shootPrompt.setString(L"Вы выстрелили: " + TranslateIndicesToBoardCoordinates(selectedCellX, selectedCellY));
                    CenterShootPrompt(shootPrompt, window);
                }
            }
        } else if (!playerMove) {
            ShootResult shootResult = (hittedShip)? smartShoot(playerBoard, xHit, yHit, robotShootX, robotShootY) : randomShoot(playerBoard, robotShootX, robotShootY);
            UpdatePlayerShapeMatrix(playerShapeMatrix, playerBoard);
            shootPrompt.setString(L"Противник выстрелил: " + TranslateIndicesToBoardCoordinates(robotShootX, robotShootY));
            CenterShootPrompt(shootPrompt, window);

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
                    --playerStatistics.unbrokenShipsNumber;
                    break;
            }

            Delay::Delay(Delay::GetRandomDelay(kRobotMinDelay, kRobotMaxDelay));
        }
        
        DrawAllEntitiesInWindow(window, gameBackground, shootPrompt, playerBoardBackground, robotBoardBackground, playerShapeMatrix, robotShapeMatrix, giveUpButton);

        window.display();
    }

    RevealRobotBoard(robotShapeMatrix, robotBoard);
    DrawAllEntitiesInWindow(window, gameBackground, shootPrompt, playerBoardBackground, robotBoardBackground, playerShapeMatrix, robotShapeMatrix, giveUpButton);

    sf::Texture windowCurrentStateTexture(window.getSize());
    windowCurrentStateTexture.update(window);
    sf::Sprite windowCurrentStateSprite(windowCurrentStateTexture);

    if (robotBoard.allShipsSunk()) {
        sf::String text = L"Победа! Поздравляем :)";
        text += L"\nКоличество выстрелов: " + std::to_wstring(playerStatistics.shootsNumber);
        text += L"\nОсталось кораблей: " +  std::to_wstring(playerStatistics.unbrokenShipsNumber);
        ShowChildWindowWithInformation(window, windowCurrentStateSprite, text);
        return;
    }

    sf::String text = L"Поражение! В следующий раз получится :(";
    text += L"\nКоличество выстрелов: " + std::to_wstring(playerStatistics.shootsNumber);
    text += L"\nУничтожено кораблей: " +  std::to_wstring(playerStatistics.killedShipsNumber);
    ShowChildWindowWithInformation(window, windowCurrentStateSprite, text);
}

void Menu(sf::RenderWindow& window) {
    // загрузка звука нажатия кнопки
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("../assets/Music/meowSound.mp3")) {
        throw std::runtime_error("failed to open file");
    }

    sf::Sound pushButtonSound(soundBuffer);
    pushButtonSound.setVolume(kDeffaultVolume);
    
    // загрузка картинки заднего фона в текстуру
    sf::Texture imageBackground;
    if (!imageBackground.loadFromFile("../assets/images/background.png")) {
        throw std::runtime_error("failed to open file");
    }

    // загрузка текстуры кнопок
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("../assets/images/parchment.png")) {
        throw std::runtime_error("failed to open file");
    }

    // создание прямоугольника куда пихаем текстуру фона
    sf::RectangleShape backGround(static_cast<sf::Vector2f>(window.getSize()));
    backGround.setTexture(&imageBackground);

    // загружаем шрифт
    sf::Font myFont;
    if (!myFont.openFromFile("../assets/Fonts/MyFont.otf")) {
        throw std::runtime_error("failed to open file");
    }

    // создаём лейбл для названия игры
    sf::Text titul(myFont, sf::String(L"Морской бой"));
    titul.setFillColor(kBlue);
    titul.setCharacterSize(kMenuTitulCharacterSize);
    titul.setOutlineThickness(2);
    titul.setOutlineColor(sf::Color::White);

    float titulCoordinateX = (window.getSize().x / 2) - titul.getGlobalBounds().getCenter().x;
    titul.setPosition(sf::Vector2f(titulCoordinateX, kMenuTitulCoordinateY));


    // создаём крутую кнопку, начинающую игру
    Button startGameButton(myFont);
    startGameButton.setString(L"Начать игру");
    startGameButton.setTextFillColor(kBrown);
    startGameButton.setCharacterSize(kMenuButtonsCharacterSize);
    startGameButton.setSize(sf::Vector2f(kMenuButtonsSizeX, kMenuButtonsSizeY));
    
    float startGameButtonCoordinateX = (window.getSize().x - startGameButton.getSize().x) / 2;

    startGameButton.setPosition(sf::Vector2f(startGameButtonCoordinateX, kMenuStartGameButtonCoordinateY));
    startGameButton.setTexture(&buttonTexture);


    // создаём крутую кнопку, показывающую правила игры
    Button showRulesButton(myFont);
    showRulesButton.setString(L"Посмотреть правила игры");
    showRulesButton.setTextFillColor(kBrown);
    showRulesButton.setCharacterSize(kMenuButtonsCharacterSize);
    showRulesButton.setSize(sf::Vector2f(kMenuButtonsSizeX, kMenuButtonsSizeY));

    float showRulesButtonCoordinateX = (window.getSize().x - showRulesButton.getSize().x) / 2;
    showRulesButton.setPosition(sf::Vector2f(showRulesButtonCoordinateX, kMenuShowRulesButtonCoordinateY));
    showRulesButton.setTexture(&buttonTexture);


    // создаём крутую кнопку, закрывающую игру
    Button quitGameCoolButton(myFont);
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
                pushButtonSound.play();
                Delay::Delay(kSoundDelay); 
                return;
            }

            if (showRulesButton.isSelected()) {
                pushButtonSound.play();
                showRulesButton.setTextFillColor(kBlue);
                DrawAllEntitiesInWindow(window, backGround, titul, startGameButton, showRulesButton, quitGameCoolButton);

                windowCurrentStateTexture.update(window);
                windowCurrentStateSprite.setTexture(windowCurrentStateTexture);

                ShowChildWindowWithInformation(window, windowCurrentStateSprite, GetRulesFromFile());

                Delay::Delay(kSystemDelay);
            }

            if (quitGameCoolButton.isSelected()) {
                pushButtonSound.play();
                quitGameCoolButton.setTextFillColor(kBrown);
                DrawAllEntitiesInWindow(window, backGround, titul,startGameButton, showRulesButton, quitGameCoolButton);

                windowCurrentStateTexture.update(window);
                windowCurrentStateSprite.setTexture(windowCurrentStateTexture);
                
                if (ShowChildWindowWithDichotomousQuestion(window, windowCurrentStateSprite, L"Вы уверены, что \nхотите выйти из игры?")) {
                    window.close();
                    return;
                }

                Delay::Delay(kSystemDelay);
            }
        }

        DrawAllEntitiesInWindow(window, backGround, titul,startGameButton, showRulesButton, quitGameCoolButton);     
        window.display();
    }
}

void RunApplication() {
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), L"Морской бой", sf::State::Fullscreen);

    sf::Music music;

    if (!music.openFromFile("../assets/Music/musicFromPiratesOfTheCaribbean.mp3")) {
        throw std::runtime_error("failed to load image");
    }

    music.setLooping(true);
    music.setVolume(kDeffaultVolume);
    music.play();

    while (window.isOpen()) {
        Menu(window);

        if (!window.isOpen()) {
            break;
        }

        StartGame(window);
        Delay::Delay(kSystemDelay);
    }
}
}  // namespace SeaBattleExecutor
