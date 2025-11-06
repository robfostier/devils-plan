#include "board.hpp"
#include "utils.hpp"
#include <random>
#include <stdexcept>
#include <iostream>

Board::Board(size_t nbPlayers) : size(nbPlayers < 5 ? 20 : 30), grid(nullptr) {
    setup(nbPlayers);
}

Board::~Board() {
    for (size_t i = 0; i < size; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}

void Board::setup(size_t nbPlayers) {
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
    size_t nbExchange = (nbPlayers * 3 + 1) / 2; // 1.5x per player (rounded up)
    size_t nbStone = (nbPlayers + 1) / 2;        // 0.5x per player (rounded up)
    size_t nbRobbery = nbPlayers;                // 1x per player

    for (size_t i = 0; i < nbExchange; ++i)
        placeBonus(BONUS_EXCHANGE);
    for (size_t i = 0; i < nbStone; ++i)
        placeBonus(BONUS_STONE);
    for (size_t i = 0; i < nbRobbery; ++i)
        placeBonus(BONUS_ROBBERY);
}

void Board::placeBonus(CellType bonusType) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist( 1, size - 2); // Exclude edges from placement

    const size_t MAX_ATTEMPTS = 100; // Prevent infinite loops
    size_t attempts = 0;
    size_t x, y;

    do {
        x = dist(gen);
        y = dist(gen);

        if (++attempts > MAX_ATTEMPTS)
            throw std::runtime_error("Board::placeBonus failed: no valid empty cell found");
    } while (grid[x][y].type != EMPTY ||
             grid[x + 1][y].type != EMPTY ||
             grid[x][y + 1].type != EMPTY ||
             grid[x - 1][y].type != EMPTY ||
             grid[x][y - 1].type != EMPTY);

    grid[x][y].type = bonusType;
}

// Display the board in the terminal.
void Board::display() const {
    for (size_t y = 0; y < size; ++y) {
        for (size_t x = 0; x < size; ++x) {
            switch (grid[x][y].type) {
            case EMPTY:
                std::cout << "・";
                break;
            case GRASS:
                if (!grid[x][y].owner) // Avoid segfault
                    throw std::runtime_error("Board::display: GRASS cell has no owner");

                std::cout << colorize(grid[x][y].owner->getColor())
                          << "██"
                          << resetColor;
                break;
            case BONUS_EXCHANGE:
                std::cout << "Ｅ";
                break;
            case BONUS_STONE:
                std::cout << "Ｓ";
                break;
            case BONUS_ROBBERY:
                std::cout << "Ｒ";
                break;
            }
        }
        std::cout << std::endl;
    }
}

void Board::placeTile(int x, int y, const Tile &tile, const Player &player) {
    const Shape &shape = tile.getShape();

    // place tile in location wiithout any validation for testing
    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j]) {
                grid[x + i][y + j].type = GRASS;
                grid[x + i][y + j].owner = const_cast<Player *>(&player); // Safe cast, player outlives board
            }
        }
    }
}