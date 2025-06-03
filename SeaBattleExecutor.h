#pragma once

#include <SFML/Graphics.hpp>

#include "GameClasses/Classes.h"

namespace SeaBattleExecutor {
void Menu(sf::RenderWindow& window);

bool ArrangePlayerShips(sf::RenderWindow& window, Board& playerBoard);
void StartGame(sf::RenderWindow& window);

void RunApplication();
}  // namespace SeaBattleExecutor
