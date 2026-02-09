#include <iostream>
#include <cstdlib>
#include "Game.h"

Game::Game() {
    char choice;
    std::cout << "Which peice do you want to play as? (G / W) ";
    std::cin >> choice;
    if (choice == 'G' || choice == 'g') {currentPlayer = u8"🟩";}
    else if(choice == 'W' || choice == 'w') {currentPlayer = u8"⬜";}
    else{std::cout << "Invalid input."; exit(0);}
}

void Game::run() {
    while (true) {
        board.print();

        if (!board.hasAnyMove(currentPlayer)) {
            std::string otherPlayer = currentPlayer == u8"🟩" ? u8"⬜" : u8"🟩";
            if (!board.hasAnyMove(otherPlayer)) break;
            currentPlayer = otherPlayer;
            continue;
        }

        int row, col;
        std::cout << (currentPlayer == u8"🟩" ? "Green" : "White") << " move (row / col): ";
        if (!(std::cin >> row >> col)) return;

        if (!board.placePiece(row,col,currentPlayer)) {
            std::cout << "Invalid move\n";
            continue;
        }

        currentPlayer = currentPlayer == u8"🟩" ? u8"⬜" : u8"🟩";
    }

    board.print();
    int greenScore = board.count(u8"🟩");
    int whiteScore = board.count(u8"⬜");

    if (greenScore > whiteScore) std::cout << "Green wins\n";
    else if (whiteScore > greenScore) std::cout << "White wins\n";
    else std::cout << "Draw\n";
}