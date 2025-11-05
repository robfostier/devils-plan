#include "player.hpp"

Player::Player(const std::string &name, PlayerColor color)
    : name(name), color(color), coupons(1) {}

bool Player::useCoupon() {
    if (coupons > 0) {
        --coupons;
        return true;
    }
    return false;
}