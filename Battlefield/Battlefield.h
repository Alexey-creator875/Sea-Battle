#pragma once
#include "../GameClasses/Classes.h"

void randomPlaceShips(Board& obj);
ShootResult randomShoot(Board& obj, int& x, int& y);
ShootResult smartShoot(Board& obj, int& x, int& y);