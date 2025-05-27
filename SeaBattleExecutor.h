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

void StartGame();
void ShowRules();
void QuitGame();

void StartPreparations();
void Fight();
void ShowResults();

void RunApplication();
}
