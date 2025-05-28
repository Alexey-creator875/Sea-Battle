#include "Classes.h"

namespace {
bool checkCoordinates(int x, int y) {
    return 0 <= y && y < kSize && 0 <= x && x < kSize;
}
} //namespace

//методы для клетки
Cell::Cell() { status = CellStatus::Empty; }
Cell::Cell(CellStatus status1) { status = status1; }
Cell::Cell(const Cell &other) { status = other.status; }
Cell::~Cell() {}

CellStatus Cell::getStatus() { return status; }
void Cell::setStatus(const CellStatus status1) { status = status1; }
Cell& Cell::operator=(const Cell& obj) {
    if (this == &obj) {
        return *this;
    }
    status = obj.status;
    return *this;
}

//методы для поля
Board::Board() {}
Board::Board(const Board &other) {
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            array[i][j] = other.array[i][j];
        }
    }
}
Board::~Board() {}

Board& Board::operator=(const Board& obj) {
    if (this == &obj) {
        return *this;
    }
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            array[i][j] = obj.array[i][j];
        }
    }
    return *this;
}

//Cell Board::getCell(int x, int y) const { return array[x][y]; }
//void Board::setCell(int x, int y) {}

//pozition = 1 - горизонтально
//pozition = 0 - вертикально
bool Board::isValidPlacement(int x, int y, int size, bool pozition) {
    bool flag = true;
    if (pozition) {
        flag = 0 <= y && y < kSize && 0 <= x && x + size <= kSize;
        if (!flag) {
            return flag;
        }
        for (int i = x - 1; i <= x + size; ++i) {
            for (int j = y - 1; j <= y + 1; ++j) {
                if (checkCoordinates(i, j) && array[i][j].getStatus() == CellStatus::Ship) {
                    flag = false;
                    break;
                }
            }
        }
        return flag;
    }
    flag = 0 <= x && x < kSize && 0 <= y && y + size <= kSize;
    if (!flag) {
        return flag;
    }
    for (int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + size; ++j) {
            if (checkCoordinates(i, j) && array[i][j].getStatus() == CellStatus::Ship) {
                flag = false;
                break;
            }
        }
    }
    return flag;
}
void Board::placeShip(int x, int y, int size, bool pozition) {
    if (!isValidPlacement(x, y, size, pozition)) { return; }
    if (pozition) {
        for (int i = x; i < x + size; ++i) {
            array[i][y] = CellStatus::Ship;
        }
    }
    for (int j = y; j < y + size; ++j) {
        array[x][j] = CellStatus::Ship;
    }
}
bool Board::isShipSunk(int x, int y) {
    bool flag = true;
    int i = x;
    while (checkCoordinates(i, y) && (array[i][y].getStatus() != CellStatus::Empty && array[i][y].getStatus() != CellStatus::ShootedEmpty)) {
        if (checkCoordinates(i, y) && array[i][y].getStatus() == CellStatus::Ship) {
            flag = false;
            break;
        }
        ++i;
    }
    i = x;
    while (checkCoordinates(i, y) && (array[i][y].getStatus() != CellStatus::Empty && array[i][y].getStatus() != CellStatus::ShootedEmpty)) {
        if (checkCoordinates(i, y) && array[i][y].getStatus() == CellStatus::Ship) {
            flag = false;
            break;
        }
        --i;
    }

    int j = y;
    while (checkCoordinates(x, j) && (array[x][j].getStatus() != CellStatus::Empty && array[x][j].getStatus() != CellStatus::ShootedEmpty)) {
        if (checkCoordinates(x, j) && array[x][j].getStatus() == CellStatus::Ship) {
            flag = false;
            break;
        }
        ++j;
    }
    j = y;
    while (checkCoordinates(x, j) && (array[x][j].getStatus() != CellStatus::Empty && array[x][j].getStatus() != CellStatus::ShootedEmpty)) {
        if (checkCoordinates(x, j) && array[x][j].getStatus() == CellStatus::Ship) {
            flag = false;
            break;
        }
        --j;
    }
      
    return flag;
}
ShootResult Board::shoot(int x, int y) {
    if (!checkCoordinates(x, y)) { return ShootResult::Miss; }
    if (array[x][y].getStatus() == CellStatus::Ship) {
        array[x][y] = CellStatus::ShootedShip;
        if (!isShipSunk(x, y)) { return ShootResult::Hit; }
        
        int i = x;
        while (checkCoordinates(i, y) && (array[i][y].getStatus() != CellStatus::Empty && array[i][y].getStatus() != CellStatus::ShootedEmpty)) {
            if (checkCoordinates(i, y - 1) && array[i][y - 1].getStatus() == CellStatus::Empty) {
                array[i][y - 1] = CellStatus::ShootedEmpty;
            }
            if (checkCoordinates(i, y + 1) && array[i][y + 1].getStatus() == CellStatus::Empty) {
                array[i][y + 1] = CellStatus::ShootedEmpty;
            }
            ++i;
        }
        i = x;
        while (checkCoordinates(i, y) && (array[i][y].getStatus() != CellStatus::Empty && array[i][y].getStatus() != CellStatus::ShootedEmpty)) {
            if (checkCoordinates(i, y - 1) && array[i][y - 1].getStatus() == CellStatus::Empty) {
                array[i][y - 1] = CellStatus::ShootedEmpty;
            }
            if (checkCoordinates(i, y + 1) && array[i][y + 1].getStatus() == CellStatus::Empty) {
                array[i][y + 1] = CellStatus::ShootedEmpty;
            }
            --i;
        }

        int j = y;
        while (checkCoordinates(x, j) && (array[x][j].getStatus() != CellStatus::Empty && array[x][j].getStatus() != CellStatus::ShootedEmpty)) {
            if (checkCoordinates(x - 1, j) && array[x - 1][j].getStatus() == CellStatus::Empty) {
                array[x - 1][j] = CellStatus::ShootedEmpty;
            }
            if (checkCoordinates(x + 1, j) && array[x + 1][j].getStatus() == CellStatus::Empty) {
                array[x + 1][j] = CellStatus::ShootedEmpty;
            }
            ++j;
        }
        j = y;
        while (checkCoordinates(x, j) && (array[x][j].getStatus() != CellStatus::Empty && array[x][j].getStatus() != CellStatus::ShootedEmpty)) {
            if (checkCoordinates(x - 1, j) && array[x - 1][j].getStatus() == CellStatus::Empty) {
                array[x - 1][j] = CellStatus::ShootedEmpty;
            }
            if (checkCoordinates(x + 1, j) && array[x + 1][j].getStatus() == CellStatus::Empty) {
                array[x + 1][j] = CellStatus::ShootedEmpty;
            }
            --j;
        }

        return ShootResult::Kill;
    }
    if (array[x][y].getStatus() == CellStatus::Empty) {
        array[x][y] = CellStatus::ShootedEmpty;
    }
    return ShootResult::Miss;
}
bool Board::allShipsSunk() {
    bool flag = true;
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            if (array[i][j].getStatus() == CellStatus::Ship) { flag = false; break;}
        }
    }
    return flag;
}

void Board::autoPlaceShips() {
    Board::placeShip(0, 5, 1, true);
    Board::placeShip(2, 4, 1, true);
    Board::placeShip(4, 3, 1, true);
    Board::placeShip(9, 0, 1, true);

    Board::placeShip(1, 1, 2, true);
    Board::placeShip(7, 0, 2, true);
    Board::placeShip(2, 8, 2, true);

    Board::placeShip(9, 4, 3, true);
    Board::placeShip(4, 8, 3, false);

    Board::placeShip(6, 3, 4, true);
}