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
    CellType &type;
    Player *owner; // Pointer to the player who owns this cell, nullptr if unowned
};

// Representation of the game board.
class Board {
  private:
    int size;
    Cell **grid;

  public:
    Board(int size);
    ~Board();

    int getSize() const;
    Cell getCell(int x, int y) const;

    void placeBonus(int nbPlayers);

    bool canPlaceTile(int x, int y, const Tile &tile, const Player &player) const;
    void placeTile(int x, int y, const Tile &tile, const Player &player);

    void display() const;
};