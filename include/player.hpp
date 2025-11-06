#pragma once

#include <string>

enum PlayerColor { PURPLE, RED, GREEN, YELLOW , BLUE, MAGENTA, CYAN, TURQUOISE, ORANGE };

class Player {
  private:
    const std::string name;
    const PlayerColor color;
    int coupons; // Number of exchange coupons the player has

  public:
    Player(const std::string &name, PlayerColor color);

    const std::string& getName() const { return name; } // Returns a constant reference to the player's name
    PlayerColor getColor() const { return color; } // Returns the player's color by value
    int getCoupons() const { return coupons; }

    // Increase the number of coupons by 1.
    void addCoupon() { ++coupons; }

    /**
    Use a coupon if available.
    Returns true if successful, false otherwise.
    */
    bool useCoupon();
};