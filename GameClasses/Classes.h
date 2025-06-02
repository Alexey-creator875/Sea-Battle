#pragma once

namespace {
const int kSize = 10;
} //namespace

enum class CellStatus {
    Empty = 1,
    Ship = 2,
    ShootedEmpty = 3,
    ShootedShip = 4,
};

enum class ShootResult {
    Miss = 1,
    Hit = 2,
    Kill = 3,
};

class Cell {
protected:
    CellStatus status;
public:
    Cell();
    Cell(CellStatus status1);
    Cell(const Cell &other);
    ~Cell();

    CellStatus getStatus();
    void setStatus(const CellStatus status1);

    Cell& operator=(const Cell& obj);
};

class Ship {
protected:
    int x;
    int y;
    int size;
    bool pozition = true;
public:
    Ship();
    Ship(int x1, int y1, int size1, bool pozition1);
    Ship(const Ship &other);
    ~Ship();

    int getX();
    int getY();
    int getSize();
    bool getPozition();

    Ship& operator=(const Ship& obj);
};

class Board {
protected:
    Cell array[kSize][kSize];
public:
    Board();
    Board(const Board &other);
    ~Board();

    Board& operator=(const Board& obj);

    CellStatus getCell(int x, int y);
    void setCell(int x, int y, CellStatus obj);
    bool isShooted(int x, int y);

    void placeShip(Ship s);
    void deleteShip(Ship s);
    bool isValidPlacement(Ship s);
    ShootResult shoot(int x, int y);
    bool isShipSunk(int x, int y);
    bool allShipsSunk();
    void clearBoard();

    void autoPlaceShips();
};