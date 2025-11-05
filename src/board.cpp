#include "board.hpp"

Board::Board(size_t nbPlayers) : size(0), grid() {
    if (nbPlayers < 5)
        size = 20;
    else
        size = 30;

    // Allocate grid
    grid = new Cell *[size];
    for (size_t i = 0; i < size; ++i) {
        grid[i] = new Cell[size];
        for (size_t j = 0; j < size; ++j) {
            grid[i][j].type = EMPTY;
            grid[i][j].owner = nullptr;
        }
    }

    // Place bonuses on the board
    placeBonus(nbPlayers);
}

Board::~Board() {
    for (size_t i = 0; i < size; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}
