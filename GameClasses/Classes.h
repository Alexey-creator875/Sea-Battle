#pragma once

namespace {
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

const int kSize = 10;
} //namespace

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

class Board {
protected:
    Cell array[kSize][kSize];
public:
    Board();
    Board(const Board &other);
    ~Board();

    Board& operator=(const Board& obj);

    //Cell getCell(int x, int y) const;
    //void setCell(int x, int y);
    void placeShip(int x, int y, int size, bool pozition);
    bool isValidPlacement(int x, int y, int size, bool pozition);
    ShootResult shoot(int x, int y);
    bool isShipSunk(int x, int y);
    bool allShipsSunk();
    void clearBoard();

    void autoPlaceShips();
};