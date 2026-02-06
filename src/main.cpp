#include <iostream>
#include "Board.h"
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;
using namespace this_thread;
using namespace chrono;

void exitProgram() {
    cout << "Invalid input. Closing the program...";
    sleep_for(seconds(3));
    exit(0);
}

int main() {
    Board gameBoard;
    gameBoard.printBoard();

    int row, col;
    char piece;

    cout << "Enter piece (B/W): ";
    cin >> piece;

    if (piece == 'b' || piece == 'B') {
        piece = '●';
    } else if (piece == 'w' || piece == 'W') {
        piece = '○';
    } else {
        exitProgram();
    }

    cout << "Enter row and column (0-7): ";
    if (!(cin >> row >> col)) {
        exitProgram();
    }

    if (row < 0 || row > 7 || col < 0 || col > 7) {
        exitProgram();
    }

    if (gameBoard.placePiece(row, col, piece))
        cout << "Piece placed successfully.\n";
    else
        cout << "Invalid position.\n";

    gameBoard.printBoard();
    return 0;
}
