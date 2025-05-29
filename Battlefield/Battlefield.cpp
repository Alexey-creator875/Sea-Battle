#include "Battlefield.h"
#include <random>

namespace {
const int kShipCnt = 10;
const int kPlaceCnt = 5;

Ship places[kPlaceCnt][kShipCnt] = {{Ship(0, 1, 1, true), Ship(2, 3, 1, true), Ship(7, 2, 1, true), Ship(7, 4, 1, true), Ship(4, 1, 2, false), Ship(4, 3, 2, false), Ship(2, 8, 2, false), Ship(0, 3, 3, true), Ship(5, 5, 3, false), Ship(6, 9, 4, false)},
                                    {Ship(0, 8, 1, true), Ship(5, 2, 1, true), Ship(7, 1, 1, true), Ship(8, 8, 1, true), Ship(7, 3, 2, true), Ship(4, 5, 2, false), Ship(5, 8, 2, true), Ship(3, 1, 3, true), Ship(7, 6, 3, false), Ship(1, 3, 4, true)},
                                    {Ship(0, 8, 1, true), Ship(2, 9, 1, true), Ship(6, 0, 1, true), Ship(9, 6, 1, true), Ship(3, 2, 2, false), Ship(7, 5, 2, true), Ship(7, 8, 2, true), Ship(0, 2, 3, true), Ship(7, 2, 3, false), Ship(2, 4, 4, false)},
                                    {Ship(1, 2, 1, true), Ship(2, 9, 1, true), Ship(2, 4, 1, true), Ship(9, 0, 1, true), Ship(4, 7, 2, false), Ship(8, 5, 2, true), Ship(8, 8, 2, false), Ship(6, 2, 3, false), Ship(0, 7, 3, true), Ship(4, 0, 4, true)},
                                    {Ship(5, 0, 1, true), Ship(4, 2, 1, true), Ship(3, 4, 1, true), Ship(0, 9, 1, true), Ship(1, 1, 2, false), Ship(0, 7, 2, false), Ship(8, 2, 2, false), Ship(8, 4, 3, true), Ship(4, 9, 3, false), Ship(3, 6, 4, false)}};
} // namespace 

void randomPlaceShips(Board& obj) {
    int randNumber = std::rand() % 5;
    obj.clearBoard();
    for (int i = 0; i < kShipCnt; ++i) {
        obj.placeShip(places[randNumber][i]);
    }
}


