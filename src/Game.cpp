#include <iostream>
#include <chrono>
#include <thread>
#include "Game.h"

#define RESET "\033[0m"
#define BLUE_FG "\033[34m"
#define RED_FG "\033[31m"
#define GREEN_FG "\033[32m"
#define BRIGHTBLACK_FG "\033[90m"

void Game::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
        std::cout.flush();
}

void Game::intro() {
    std::string title = "                 WELCOME TO THE OTHELLO GAME.";
    std::cout << "   ═════════════════════════════════════════════════════════\n";
    for (char c : title) {
        std::cout << "\033[32m" << c << "\033[0m";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    std::cout << "\n   ═════════════════════════════════════════════════════════\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    clearScreen();
}

int Game::menu() {
    std::cout << "\033[34m";
    std::cout << "   ┌───────────────────────────────────────────────────────┐\n";
    for (int i = 0; i < 10; i++) {
        std::cout << "   │";
        if (i == 2) std::cout << "                       Othello                         │";
        else if (i == 4) std::cout << "\033[32m   [0] New Game                                        \033[34m│";
        else if (i == 5) std::cout << "\033[32m   [1] Load Game                                       \033[34m│";
        else if (i == 6) std::cout << "\033[32m   [2] Scoreboard                                      \033[34m│";
        else if (i == 7) std::cout << "\033[31m   [3] Exit                                            \033[34m│";
        else std::cout << "                                                       │";
        std::cout << "\n";
    }
    std::cout << "   └───────────────────────────────────────────────────────┘\n\033[0m";
    int c;
    std::cin >> c;
    return c;
}

Game::Game() {
}

void Game::choosePiece(){
    char choice;
    std::cout << BRIGHTBLACK_FG << "Which piece do you want to play as? (" << GREEN_FG << "G " << BRIGHTBLACK_FG <<"/" << RESET << " W" << BRIGHTBLACK_FG << ") ";
    std::cin >> choice;
    if (choice == 'G' || choice == 'g') currentPlayer = u8"🟩";
    else if (choice == 'W' || choice == 'w') currentPlayer = u8"⬜";
    else exit(0);
}

void Game::run() {
    intro();
    int c = menu();
    clearScreen();
    if (c != 0) return;

    choosePiece();
    clearScreen();

    while (true) {
        board.print();

        if (!board.hasAnyMove(currentPlayer)) {
            std::string other = currentPlayer == u8"🟩" ? u8"⬜" : u8"🟩";
            if (!board.hasAnyMove(other)) break;
            currentPlayer = other;
            continue;
        }

        int row, col;
        std::cout << (currentPlayer == u8"🟩" ? "Green" : "White") << " move (row col): ";
        if (!(std::cin >> row >> col)) return;

        if (!board.placePiece(row, col, currentPlayer)) {
            std::cout << "Invalid move\n";
            clearScreen();
            continue;
        }

        currentPlayer = currentPlayer == u8"🟩" ? u8"⬜" : u8"🟩";
        clearScreen();
    }

    board.print();
    int g = board.count(u8"🟩");
    int w = board.count(u8"⬜");
    if (g > w) std::cout << "Green wins\n";
    else if (w > g) std::cout << "White wins\n";
    else std::cout << "Draw\n";
}