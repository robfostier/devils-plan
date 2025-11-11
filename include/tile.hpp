#pragma once

#include "tile_shapes.hpp"
#include <vector>

class Tile {
  private:
    Shape baseShape;
    int rotation = 0;
    bool flipped = false;

    static Shape rotateShape(const Shape &shape);

  public:
    Tile(const Shape &shape) : baseShape(shape), rotation(0), flipped(false) {}

    Shape getShape() const;

    // Rotate the tile 90 degrees clockwise.
    void rotateClockwise() { rotation = (rotation + 90) % 360; }

    // Filp the tile horizontally
    void flipHorizontal() { flipped = !flipped; }

    // Print the tile shape to the console.
    void print() const;
};