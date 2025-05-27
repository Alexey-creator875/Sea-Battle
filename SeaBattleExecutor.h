#pragma once

#include <SFML/Graphics.hpp>

namespace SeaBattleExecutor {
void Menu(sf::RenderWindow& window);

void StartGame();
void ShowRules();
void QuitGame();

void StartPreparations();
void Fight();
void ShowResults();

void RunApplication();
}
