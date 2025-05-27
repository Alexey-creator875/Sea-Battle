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
    CellStatus status;
public:
    Cell();
    Cell(CellStatus status1);
    Cell(const Cell &other);
    ~Cell();

    CellStatus getStatus();
    void setStatus(const CellStatus status1);


};

class Board {
protected:
    Cell array[kSize][kSize];
public:
    
};