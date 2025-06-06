#include "BoardInteractions.h"
#include <random>

namespace {
std::mt19937 get_rnd(time(nullptr));

const int kShipCnt = 10;
const int kPlaceCnt = 5;

Ship places[kPlaceCnt][kShipCnt] = {{Ship(0, 1, 1, true), Ship(2, 3, 1, true), Ship(7, 2, 1, true), Ship(7, 4, 1, true), Ship(4, 1, 2, false), Ship(4, 3, 2, false), Ship(2, 8, 2, false), Ship(0, 3, 3, true), Ship(5, 6, 3, false), Ship(6, 9, 4, false)},
                                    {Ship(0, 8, 1, true), Ship(5, 2, 1, true), Ship(7, 1, 1, true), Ship(8, 8, 1, true), Ship(7, 3, 2, true), Ship(4, 5, 2, false), Ship(5, 8, 2, true), Ship(3, 1, 3, true), Ship(7, 6, 3, false), Ship(1, 3, 4, true)},
                                    {Ship(0, 8, 1, true), Ship(2, 9, 1, true), Ship(6, 0, 1, true), Ship(9, 6, 1, true), Ship(3, 2, 2, false), Ship(7, 5, 2, true), Ship(7, 8, 2, true), Ship(0, 2, 3, true), Ship(7, 2, 3, false), Ship(2, 4, 4, false)},
                                    {Ship(1, 2, 1, true), Ship(2, 9, 1, true), Ship(2, 4, 1, true), Ship(9, 0, 1, true), Ship(4, 7, 2, false), Ship(8, 5, 2, true), Ship(8, 8, 2, false), Ship(6, 2, 3, false), Ship(0, 7, 3, true), Ship(4, 0, 4, true)},
                                    {Ship(5, 0, 1, true), Ship(4, 2, 1, true), Ship(3, 4, 1, true), Ship(0, 9, 1, true), Ship(1, 1, 2, false), Ship(0, 7, 2, false), Ship(8, 2, 2, false), Ship(8, 4, 3, true), Ship(4, 9, 3, false), Ship(3, 6, 4, false)}};

bool checkCoordinates(int x, int y) {
    return 0 <= y && y < kSize && 0 <= x && x < kSize;
}
} // namespace 


void randomPlaceShips(Board& obj) {
    int randNumber = get_rnd() % 5;
    obj.clearBoard();
    for (int i = 0; i < kShipCnt; ++i) {
        obj.placeShip(places[randNumber][i]);
    }
}

ShootResult randomShoot(Board& obj, int& x, int& y) {
    int randX = get_rnd() % 10;
    int randY = get_rnd() % 10;
    while (obj.getCell(randX, randY) == CellStatus::ShootedEmpty || obj.getCell(randX, randY) == CellStatus::ShootedShip) {
        randX = get_rnd() % 10;
        randY = get_rnd() % 10;
    }
    x = randX;
    y = randY;
    return obj.shoot(x, y);
}

ShootResult smartShoot(Board& obj, int& xHit, int& yHit, int& x, int& y) {
    int cntShooted = 0, xShooted = 0, yShooted = 0, xEmpty = 0, yEmpty = 0;
    x = xHit;
    y = yHit;
    if (checkCoordinates(x, y + 1) && obj.getCell(x, y + 1) == CellStatus::ShootedShip) {
        xShooted = x;
        yShooted = y + 1;
        ++cntShooted;
        if (checkCoordinates(x, y - 1) && !obj.isShooted(x, y - 1)) {
            return obj.shoot(x, --y);
        } else {
            while (obj.isShooted(x, y)) {
                ++y;
            }
            return obj.shoot(x, y);
        }
    } else if (checkCoordinates(x, y + 1) && !obj.isShooted(x, y + 1)) {
        xEmpty = x;
        yEmpty = y + 1;
    }
    if (checkCoordinates(x, y - 1) && obj.getCell(x, y - 1) == CellStatus::ShootedShip) {
        xShooted = x;
        yShooted = y - 1;
        ++cntShooted;
        if (checkCoordinates(x, y + 1) && !obj.isShooted(x, y + 1)) {
            return obj.shoot(x, ++y);
        } else {
            while (obj.isShooted(x, y)) {
                --y;
            }
            return obj.shoot(x, y);
        }
    } else if (checkCoordinates(x, y - 1) && !obj.isShooted(x, y - 1)) {
        xEmpty = x;
        yEmpty = y - 1;
    }

    if (checkCoordinates(x + 1, y) && obj.getCell(x + 1, y) == CellStatus::ShootedShip) {
        xShooted = x + 1;
        yShooted = y;
        ++cntShooted;
        if (checkCoordinates(x - 1, y) && !obj.isShooted(x - 1, y)) {
            return obj.shoot(--x, y);
        } else {
            while (obj.isShooted(x, y)) {
                ++x;
            }
            return obj.shoot(x, y);
        }
    } else if (checkCoordinates(x + 1, y) && !obj.isShooted(x + 1, y)) {
        xEmpty = x + 1;
        yEmpty = y;
    }
    if (checkCoordinates(x - 1, y) && obj.getCell(x - 1, y) == CellStatus::ShootedShip) {
        xShooted = x - 1;
        yShooted = y;
        ++cntShooted;
        if (checkCoordinates(x + 1, y) && !obj.isShooted(x + 1, y)) {
            return obj.shoot(++x, y);
        } else {
            while (obj.isShooted(x, y)) {
                --x;
            }
            return obj.shoot(x, y);
        }
    } else if (checkCoordinates(x - 1, y) && !obj.isShooted(x - 1, y)) {
        xEmpty = x - 1;
        yEmpty = y;
    }
    if (cntShooted == 0) {
        x = xEmpty;
        y = yEmpty;
        return obj.shoot(x, y);
    }
    return obj.shoot(x, y);
}
