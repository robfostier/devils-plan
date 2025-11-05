#pragma once

#include "board.hpp"
#include "player.hpp"
#include "tile_queue.hpp"
#include <vector>

class Game {
  private:
    Board board;
    TileQueue tileQueue;
    std::vector<Player> players;
    size_t currentRound;
    const size_t maxRounds;

  public:
    Game(size_t nbPlayers);

    void setup();
    void play();
    void playTurn(Player &player);
    void endGame();
    void determineWinner();
};