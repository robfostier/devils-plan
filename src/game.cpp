#include "game.hpp"

Game::Game(size_t nbPlayers)
    : board(nbPlayers), tileQueue(nbPlayers), players(), currentRound(0), maxRounds(20) {
    // Initialize players with default names and colors
    for (size_t i = 0; i < nbPlayers; ++i) {
        std::string playerName = "Player " + std::to_string(i + 1);
        PlayerColor color = static_cast<PlayerColor>(i % 9); // Cycle through available colors
        players.emplace_back(playerName, color);
    }
}