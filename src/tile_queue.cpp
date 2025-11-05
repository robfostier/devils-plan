#include "tile_queue.hpp"

#include <algorithm>
#include <random>
#include <stdexcept>

TileQueue::TileQueue(size_t nbPlayers) {
    size_t nbTiles = (nbPlayers * 32 + 1) / 3; // 32 / 3 = 10.67 tiles per player, + 1 for rounding

    // Make a shuffled list of indices to select random shapes
    std::vector<size_t> indices(TILE_SHAPES.size());
    std::iota(indices.begin(), indices.end(), 0); // Fills indices with 0..N-1
    std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});

    for (size_t i = 0; i < nbTiles && i < indices.size(); ++i)
        tiles.emplace_back(Tile(TILE_SHAPES[indices[i]]));
}

Tile TileQueue::drawTile() {
    if (tiles.empty())
        throw std::out_of_range("No more tiles to draw.");

    Tile drawnTile = tiles.front();
    tiles.pop_front();
    return drawnTile;
}

Tile TileQueue::exchangeTile(int index) {
    if (index < 0 ||
        static_cast<size_t>(index) >= tiles.size()) // Casting to size_t for STL compatibility
        throw std::out_of_range("Tile index out of range.");

    // Move all tiles before the chosen one to the back of the deque
    for (size_t i = 0; i < static_cast<size_t>(index); i++) {
        tiles.push_back(tiles.front());
        tiles.pop_front();
    }

    Tile chosen = tiles.front();
    tiles.pop_front();

    return chosen;
}

std::vector<std::reference_wrapper<const Tile>> TileQueue::nextTiles(size_t n) const {
    std::vector<std::reference_wrapper<const Tile>> nextTiles;

    size_t count = std::min(n, tiles.size()); // Ensure we don't exceed available tiles

    for (size_t i = 0; i < count; ++i)
        nextTiles.push_back(std::cref(tiles[i])); // Store constant references

    return nextTiles;
}