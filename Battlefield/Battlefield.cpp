#include "Battlefield.h"
#include <random>

namespace {
const int kShipCnt = 10;
const int kPlaceCnt = 5;

Ship places[kPlaceCnt][kShipCnt] = {{Ship(1, 0, 1, true), Ship(3, 2, 1, true), Ship(2, 7, 1, true), Ship(4, 7, 1, true), Ship(1, 4, 2, false), Ship(3, 4, 2, false), Ship(8, 2, 2, false), Ship(3, 0, 3, true), Ship(5, 5, 3, false), Ship(9, 6, 4, false)},
                                    {Ship(8, 0, 1, true), Ship(2, 5, 1, true), Ship(1, 7, 1, true), Ship(8, 8, 1, true), Ship(3, 7, 2, true), Ship(5, 4, 2, false), Ship(8, 5, 2, true), Ship(1, 3, 3, true), Ship(6, 7, 3, false), Ship(3, 1, 4, true)},
                                    {Ship(8, 0, 1, true), Ship(9, 2, 1, true), Ship(0, 6, 1, true), Ship(6, 9, 1, true), Ship(2, 3, 2, false), Ship(5, 7, 2, true), Ship(8, 7, 2, true), Ship(2, 0, 3, true), Ship(2, 7, 3, false), Ship(4, 2, 4, false)},
                                    {Ship(2, 1, 1, true), Ship(9, 2, 1, true), Ship(4, 2, 1, true), Ship(0, 9, 1, true), Ship(7, 4, 2, false), Ship(5, 8, 2, true), Ship(8, 8, 2, false), Ship(2, 6, 3, false), Ship(7, 0, 3, true), Ship(0, 4, 4, true)},
                                    {Ship(0, 5, 1, true), Ship(2, 4, 1, true), Ship(4, 3, 1, true), Ship(9, 0, 1, true), Ship(1, 1, 2, false), Ship(7, 0, 2, false), Ship(2, 8, 2, false), Ship(4, 8, 3, true), Ship(9, 4, 3, false), Ship(6, 3, 4, false)}};
} // namespace 

void randomPlaceShips(Board& obj) {
    int randNumber = std::rand() % 5;
    obj.clearBoard();
    for (int i = 0; i < kShipCnt; ++i) {
        obj.placeShip(places[randNumber][i]);
    }
}

ShootResult randomShoot(Board& obj) {
    int randX = std::rand() % 10;
    int randY = std::rand() % 10;
    while (obj.getCell(randX, randY) == CellStatus::ShootedEmpty || obj.getCell(randX, randY) == CellStatus::ShootedShip) {
        randX = std::rand() % 10;
        randY = std::rand() % 10;
    }
    return obj.shoot(randX, randY);
}

// ShootResult smartShoot(Board& obj) {
// }

// bool checker = false;
// Board playerBoard;
// void programmShoot() {
//     if (!checker) {
//         if (randomShoot(playerBoard) == ShootResult::Hit) {
//             checker = true;
//         }
//     }
// }

