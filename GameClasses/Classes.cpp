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

//методы для корабля
Ship::Ship() {}
Ship::Ship(int x1, int y1, int size1, bool pozition1) {
    x = x1;
    y = y1;
    size = size1;
    pozition = pozition1;
}
Ship::Ship(const Ship &other) {
    x = other.x;
    y = other.y;
    size = other.size;
    pozition = other.pozition;
}
Ship::~Ship() {}

int Ship::getX() { return x; }
int Ship::getY() { return y; }
int Ship::getSize() { return size; }
bool Ship::getPozition() { return pozition; }

Ship& Ship::operator=(const Ship& obj) {
    if (this == &obj) {
        return *this;
    }
    x = obj.x;
    y = obj.y;
    size = obj.size;
    pozition = obj.pozition;
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

CellStatus Board::getCell(int x, int y) { return array[x][y].getStatus(); }
void Board::setCell(int x, int y, CellStatus obj) { array[x][y].setStatus(obj); }
bool Board::isShooted(int x, int y) {
    return (array[x][y].getStatus() == CellStatus::ShootedEmpty || array[x][y].getStatus() == CellStatus::ShootedShip);
}

//pozition = 1 - горизонтально
//pozition = 0 - вертикально
bool Board::isValidPlacement(Ship s) {
    int x = s.getX();
    int y = s.getY();
    int size = s.getSize();
    bool pozition = s.getPozition();
    bool flag = true;
    if (!pozition) {
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
void Board::placeShip(Ship s) {
    int x = s.getX();
    int y = s.getY();
    int size = s.getSize();
    bool pozition = s.getPozition();
    if (!isValidPlacement(s)) { return; }
    if (!pozition) {
        for (int i = x; i < x + size; ++i) {
            array[i][y] = CellStatus::Ship;
        }
        return;
    }
    for (int j = y; j < y + size; ++j) {
        array[x][j] = CellStatus::Ship;
    }
}
void Board::deleteShip(Ship s) {
    int x = s.getX();
    int y = s.getY();
    int size = s.getSize();
    bool pozition = s.getPozition();
    if (!isValidPlacement(s)) { return; }
    if (!pozition) {
        for (int i = x; i < x + size; ++i) {
            array[i][y] = CellStatus::Empty;
        }
        return;
    }
    for (int j = y; j < y + size; ++j) {
        array[x][j] = CellStatus::Empty;
    }
}
int Board::countShip() {
    int counter = 0;
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            if (array[i][j].getStatus() == CellStatus::Ship) {
                if (!checkCoordinates(i - 1, j) || array[i - 1][j].getStatus() != CellStatus::Ship) {
                    if (!checkCoordinates(i, j - 1) || array[i][j - 1].getStatus() != CellStatus::Ship) {
                        ++counter;
                    }
                }
            }
        }
    }
    return counter;
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
        if (checkCoordinates(i, y) && array[i][y].getStatus() == CellStatus::Empty) {
            array[i][y] = CellStatus::ShootedEmpty;
        }
        if (checkCoordinates(i, y - 1) && array[i][y - 1].getStatus() == CellStatus::Empty) {
            array[i][y - 1] = CellStatus::ShootedEmpty;
        }
        if (checkCoordinates(i, y + 1) && array[i][y + 1].getStatus() == CellStatus::Empty) {
            array[i][y + 1] = CellStatus::ShootedEmpty;
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
        if (checkCoordinates(i, y) && array[i][y].getStatus() == CellStatus::Empty) {
            array[i][y] = CellStatus::ShootedEmpty;
        }
        if (checkCoordinates(i, y - 1) && array[i][y - 1].getStatus() == CellStatus::Empty) {
            array[i][y - 1] = CellStatus::ShootedEmpty;
        }
        if (checkCoordinates(i, y + 1) && array[i][y + 1].getStatus() == CellStatus::Empty) {
            array[i][y + 1] = CellStatus::ShootedEmpty;
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
        if (checkCoordinates(x - 1, j) && array[x - 1][j].getStatus() == CellStatus::Empty) {
            array[x - 1][j] = CellStatus::ShootedEmpty;
        }
        if (checkCoordinates(x + 1, j) && array[x + 1][j].getStatus() == CellStatus::Empty) {
            array[x + 1][j] = CellStatus::ShootedEmpty;
        }
        if (checkCoordinates(x, j) && array[x][j].getStatus() == CellStatus::Empty) {
            array[x][j] = CellStatus::ShootedEmpty;
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
        if (checkCoordinates(x - 1, j) && array[x - 1][j].getStatus() == CellStatus::Empty) {
            array[x - 1][j] = CellStatus::ShootedEmpty;
        }
        if (checkCoordinates(x + 1, j) && array[x + 1][j].getStatus() == CellStatus::Empty) {
            array[x + 1][j] = CellStatus::ShootedEmpty;
        }
        if (checkCoordinates(x, j) && array[x][j].getStatus() == CellStatus::Empty) {
            array[x][j] = CellStatus::ShootedEmpty;
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

void Board::clearBoard() {
    for (int i = 0; i < kSize; ++i) {
        for (int j = 0; j < kSize; ++j) {
            array[i][j] = Cell(CellStatus::Empty);
        }
    }
}

void Board::autoPlaceShips() {
    Board::placeShip(Ship(0, 5, 1, true));
    Board::placeShip(Ship(2, 4, 1, true));
    Board::placeShip(Ship(4, 3, 1, true));
    Board::placeShip(Ship(9, 0, 1, true));

    Board::placeShip(Ship(1, 1, 2, true));
    Board::placeShip(Ship(7, 0, 2, true));
    Board::placeShip(Ship(2, 8, 2, true));

    Board::placeShip(Ship(9, 4, 3, true));
    Board::placeShip(Ship(4, 8, 3, false));

    Board::placeShip(Ship(6, 3, 4, true));
}