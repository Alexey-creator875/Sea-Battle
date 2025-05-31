#pragma once

#include <SFML/Graphics.hpp>

namespace SeaBattleExecutor {
enum class Action {
    Wait,
    StartGame,
    ShowRules,
    QuitGame,
};

void Menu(sf::RenderWindow& window);

void StartGame(sf::RenderWindow& window);
void ShowRules(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite);

void ShowResults(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite, bool playerWin);

void RunApplication();
}
