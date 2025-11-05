#pragma once

#include "player.hpp"
#include "tile.hpp"

/**
Types of cells on the board.
Can be empty, grass, stone, or various bonus types.
*/
enum CellType {
    EMPTY,
    GRASS,
    STONE,
    BONUS_EXCHANGE,
    BONUS_STONE,
    BONUS_ROBBERY
};

/** 
Representation of a cell on the board.
Contains a reference to its type and a pointer to the owning player, if any.
*/
struct Cell {
    CellType type;
    Player *owner; // Pointer to the player who owns this cell, nullptr if unowned
};

// Representation of the game board.
class Board {
  private:
    size_t size;
    Cell **grid; // Dynamic 2D array of Cells

  public:
    Board(size_t nbPlayers);
    ~Board();

    int getSize() const;
    const Cell& getCell(int x, int y) const;

    void placeBonus(size_t nbPlayers);

    bool canPlaceTile(int x, int y, const Tile &tile, const Player &player) const;
    void placeTile(int x, int y, const Tile &tile, const Player &player);

    void display() const;
};