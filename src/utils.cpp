#include "utils.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

void clear_terminal(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001); // Set console to UTF-8 on Windows
#endif

    std::cout << "\033[2J\033[H"; // Clear terminal using ANSI escape codes

    // Header
    std::cout << "+--------------------------------+" << std::endl;
    std::cout << "| The Devil's Plan - 데블스 플랜 |" << std::endl;
    std::cout << "+--------------------------------+" << std::endl << std::endl;
}

Colorize colorize(PlayerColor color) {
    return Colorize(color);
}

std::ostream &resetColor(std::ostream &os) {
    return os << "\033[0m";
}

std::ostream& operator<<(std::ostream& os, const Colorize& c) {
    switch (c.color) {
        case PURPLE:
            return os << "\033[35m";
        case RED:
            return os << "\033[31m";
        case GREEN:
            return os << "\033[32m";
        case YELLOW:
            return os << "\033[33m";
        case BLUE:
            return os << "\033[34m";
        case MAGENTA:
            return os << "\033[95m";
        case CYAN:
            return os << "\033[36m";
        case TURQUOISE:
            return os << "\033[38;5;45m";
        case ORANGE:
            return os << "\033[38;5;208m";
        default:
            return os;
    }
}