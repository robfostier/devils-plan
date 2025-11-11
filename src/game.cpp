#include "game.hpp"
#include "utils.hpp"
#include <iostream>
#include <optional>

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

void Game::display(Player &player, Tile &tile, bool bDisplayQueue) const {
    clearTerminal();
    std::cout << player.getName() << " - Round " << currentRound << ":" << std::endl
              << std::endl;

    std::cout << "Current Tile:" << std::endl;
    tile.print();
    std::cout << std::endl;

    if (bDisplayQueue) {
        std::cout << "Next Tiles:" << std::endl;
        auto nextTiles = tileQueue.nextTiles();
        tileQueue.printExchangeQueue(nextTiles);
        std::cout << std::endl;
    }

    board.display();
}

void Game::placingTile(Player &player, Tile &tile, bool bStealable, bool bDisplayQueue) {
    bool ready = false;
    while(!ready) {
        display(player, tile, bDisplayQueue);

        std::cout << "1 - Place Tile" << std::endl;
        std::cout << "2 - Rotate Tile" << std::endl;
        std::cout << "3 - Flip Tile" << std::endl;

        int choiceBuffer = getIntegerInputInRange("> ", 1, 3);

        switch (choiceBuffer) {
            case 1:
                ready = true;
                break;
            case 2:
                tile.rotateClockwise();
                break;
            case 3:
                tile.flipHorizontal();
                break;
        }
    }

    display(player, tile, bDisplayQueue);

    std::pair<size_t, size_t> coords = {0, 0};
    while (true) {
        coords = getCoordinatesInput(board.getSize());
        bool canPlace = board.canPlaceTile(coords, tile, player);
        if (canPlace)
            break;
        std::cout << "Cannot place starting tile at (" << coords.first << ", "
                  << coords.second << "). Try again." << std::endl;
    }

    board.placeTile(coords, tile, &player, bStealable);
    display(player, tile, bDisplayQueue);

    size_t pendingStone = player.getStoneBonus();
    while (pendingStone > 0) {
        std::cout << "You have " << pendingStone << " pending stone bonus to use immediately." << std::endl;
        applyStoneBonus(player);
        pendingStone = player.getStoneBonus();
    }

    size_t pendingRobbery = player.getRobberyBonus();
    while (pendingRobbery > 0) {
        std::cout << "You have " << pendingRobbery << " pending robbery bonus to use immediately." << std::endl;
        applyRobberyBonus(player);
        pendingRobbery = player.getRobberyBonus();
    }
}

void Game::play() {
    // board.placeTile({10, 10}, tileQueue.drawTile(), players[0]); // Initial tile placement for testing

    for (currentRound = 0; currentRound <= maxRounds; ++currentRound) {
        for (auto &player : players)
            playTurn(player);
    }

    playTurn(players[0]); // Single turn for testing
}

void Game::playTurn(Player &player) {
    clearTerminal();

    // Place starting tile in the first round
    if (currentRound == 0) {
        board.display();

        Tile startingTile = Tile(STARTING_TILE);

        std::cout << player.getName() << " - Starting location:" << std::endl
                  << std::endl;

        std::pair<size_t, size_t> coords = {0, 0};
        while (true) {
            coords = getCoordinatesInput(board.getSize());
            bool canPlace = board.canPlaceTile(coords, startingTile, player);
            if (canPlace)
                break;
            std::cout << "Cannot place starting tile at ("
                      << coords.first << ", " << coords.second
                      << "). Try again." << std::endl
                      << std::endl;
        }

        board.placeTile(coords, startingTile, &player, false);
        return;
    }

    // --- Turn logic goes here ---
    Tile currentTile = tileQueue.drawTile();

    display(player, currentTile, true);

    // While player has exchange coupons, offer choice to use them
    size_t exchangeCoupons = player.getCoupons();
    bool bDone = false; // Player can only use one coupon per turn, and allows to exit loop if Take Tile
    while (exchangeCoupons > 0 && !bDone) {
        std::cout << "1 - Take Tile" << std::endl;
        std::cout << "2 - Exchange (" << exchangeCoupons << " available)" << std::endl;

        int choiceAction = getIntegerInputInRange("> ", 1, 2);

        std::cout << std::endl;

        switch (choiceAction) {
        case 1:
            bDone = true;
            break;
        case 2: {
            std::cout << "1 - Exchange a tile from the queue" << std::endl;
            std::cout << "2 - Remove a stone from the board" << std::endl;
            int choiceExchange = getIntegerInputInRange("> ", 1, 2);

            std::cout << std::endl;

            switch (choiceExchange) {
            case 1: {
                size_t queueSize = tileQueue.nextTiles().size();
                size_t maxChoices = std::min(static_cast<size_t>(5), queueSize);

                if (maxChoices > 0) {
                    std::cout << "Choose a tile to take (1 - " << maxChoices << "), or 0 to cancel: " << std::endl;
                    int choiceTileIndex = getIntegerInputInRange("> ", 0, static_cast<int>(maxChoices));

                    if (choiceTileIndex == 0)
                        std::cout << "Exchange cancelled. Keeping first tile." << std::endl;
                    else {
                        int tileIndex = choiceTileIndex - 1;
                        currentTile = tileQueue.exchangeTile(tileIndex);
                        player.useCoupon();
                        bDone = true; // We chose that player can only use one coupon per turn
                    }
                } else
                    std::cout << "No tiles available in queue to exchange." << std::endl;
                
                std::cout << std::endl;
                break;
            }
            case 2: {
                // Making sure there are stones on the board
                bool bExists = false;
                for (size_t x = 0; x < board.getSize(); ++x) {
                    for (size_t y = 0; y < board.getSize(); ++y) {
                        if (board.getCell({x, y}).type == STONE) {
                            bExists = true;
                            break;
                        }
                    }
                }

                if (!bExists)
                    std::cout << "No stone on the board to exchange." << std::endl;
                else {
                    std::pair<size_t, size_t> coords = getCoordinatesInput(board.getSize());
                    const Cell &cell = board.getCell(coords);

                    if (cell.type == STONE) {
                        board.setCell(coords, EMPTY, nullptr);
                        player.useCoupon();
                        bDone = true; // We chose that player can only use one coupon per turn
                        std::cout << "Stone successfully removed at ("
                                  << coords.first << ", " << coords.second
                                  << ")." << std::endl;
                        break;
                    } else
                        std::cout << "No stone found at ("
                                  << coords.first << ", " << coords.second
                                  << "). Try again." << std::endl;
                }

                std::cout << std::endl;
                break;
            }
            default:
                break;
            }
            break;
        }
        default:
            break;
        }

        exchangeCoupons = player.getCoupons();
    }

    placingTile(player, currentTile, true, true);
}

void Game::applyStoneBonus(Player &player) {
    std::pair<size_t, size_t> coords = {0, 0};

    while (true) {
        coords = getCoordinatesInput(board.getSize());

        if (board.getCell(coords).type == EMPTY)
            break;
        else
            std::cout << "Cannot place stone tile at ("
                      << coords.first << ", " << coords.second
                      << "). Try again." << std::endl;
    }

    board.setCell(coords, STONE, nullptr);

    player.useStoneBonus();
    std::cout << "Stone bonus used successfully at (" << coords.first << " " << coords.second << ")" << std::endl
              << std::endl;
}

void Game::applyRobberyBonus(Player &player) {
    std::pair<size_t, size_t> coords = {0, 0};
    std::optional<Tile> stolenTile;

    player.useRobberyBonus();

    // Need to make sure there is a tile to steal before entering the loop.
    // If none exist, player can't use bonus but still loses coupon.
    bool bExists = false;
    for (const PlacedTile &placedTile : board.getPlacedTiles()) {
        if (placedTile.owner != &player && placedTile.bStealable) {
            bExists = true;
            break;
        }
    }

    if (!bExists) {
        std::cout << "No enemy tiles available to steal." << std::endl
                  << std::endl;
        return;
    }

    while (true) {
        coords = getCoordinatesInput(board.getSize());
        stolenTile = board.stealTile(coords, &player);

        if (stolenTile.has_value())
            break;
        else
            std::cout << "Cannot rob tile at ("
                      << coords.first << ", " << coords.second
                      << "). Try again." << std::endl;
    }
    
    placingTile(player, stolenTile.value(), true, false);

    std::cout << "Robbery bonus used successfully at ("
              << coords.first << " " << coords.second
              << ")" << std::endl
              << std::endl;
}