#include "game.hpp"

Game::Game(size_t nbPlayers)
    : board(nbPlayers), tileQueue(nbPlayers), nbPlayers(nbPlayers), players(), currentRound(0) {
    setup();
}

void Game::setup() {
    // Initialize players
    players.reserve(nbPlayers);
    for (size_t i = 0; i < nbPlayers; ++i) {
        std::string name = "Player " + std::to_string(i + 1);
        PlayerColor color = static_cast<PlayerColor>(i % 9); // Cycle colors
        players.emplace_back(name, color);
    }
}

void Game::play() {
    board.placeTile(10, 10, tileQueue.drawTile(), players[0]); // Initial tile placement for testing
    board.display();
}