#pragma once

namespace {
enum class CellStatus {
    Empty = 1,
    Ship = 2,
    ShootedEmpty = 3,
    ShootedShip = 4,
};

const int kSize = 10;
} //namespace

class Cell {
protected:
    char column;
    int line;
    CellStatus status;
public:
    Cell();
    ~Cell();
};

class Board {
protected:
    Cell array[kSize][kSize];
public:
    
};