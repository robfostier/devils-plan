#pragma once

#include "player.hpp"
#include <iostream>

// Clear the terminal and print the game header.
void clear_terminal();

// Helper struct to apply color formatting to output streams.
struct Colorize {
    PlayerColor color;
    explicit Colorize(PlayerColor c) : color(c) {}
};

// Create a Colorize object for the given PlayerColor.
Colorize colorize(PlayerColor color);

// Reset the output stream's color formatting to default.
std::ostream &resetColor(std::ostream &os);

/**
Overload the << operator to apply color formatting when a Colorize object is
sent to the stream.
*/
std::ostream &operator<<(std::ostream &os, const Colorize &c);