#include <iostream>
#include "Game.h"

Game::Game() {
    current = u8"🟩";
}

void Game::run() {
    while (true) {
        board.print();

        if (!board.hasAnyMove(current)) {
            std::string other = current == u8"🟩" ? u8"⬜" : u8"🟩";
            if (!board.hasAnyMove(other)) break;
            current = other;
            continue;
        }

        int r, c;
        std::cout << (current == u8"🟩" ? "Green" : "White") << " move (row col): ";
        if (!(std::cin >> r >> c)) return;

        if (!board.placePiece(r,c,current)) {
            std::cout << "Invalid move\n";
            continue;
        }

        current = current == u8"🟩" ? u8"⬜" : u8"🟩";
    }

    board.print();
    int g = board.count(u8"🟩");
    int w = board.count(u8"⬜");

    if (g > w) std::cout << "Green wins\n";
    else if (w > g) std::cout << "White wins\n";
    else std::cout << "Draw\n";
}