#pragma once

#include <SFML/Graphics.hpp>

#include "GameClasses/Classes.h"

namespace SeaBattleExecutor {
void Menu(sf::RenderWindow& window);

void StartGame(sf::RenderWindow& window);
bool ArrangePlayerShips(sf::RenderWindow& window, Board& playerBoard);

void ShowRules(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite);

void ShowResults(sf::RenderWindow& window, const sf::Sprite& windowCurrentStateSprite, bool playerWin);

void RunApplication();
}
