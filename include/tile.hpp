#pragma once

#include "tile_shapes.hpp"
#include <vector>

class Tile {
  private:
    const Shape& shape;
    int rotation;

  public:
    Tile(const Shape& shape) : shape(shape), rotation(0) {}

    const Shape& getShape() const { return shape; }
    
    // Rotate the tile 90 degrees clockwise.
    void rotateClockwise();
};