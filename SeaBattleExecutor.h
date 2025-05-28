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
void ShowRules();
bool QuitGame(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite);

void StartPreparations();
void Fight();
void ShowResults();

void RunApplication();
}
