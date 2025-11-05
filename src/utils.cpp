#include "utils.hpp"

#include <iostream>

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