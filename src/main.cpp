#include <iostream>
#include "Board.h"
using namespace std;

int main() {
    Board gameBoard;
    gameBoard.printBoard();

    int row, col;
    char piece;

    cout << "Enter piece (B/W): ";
    cin >> piece;
    if (piece != ("B" || "W" || "b" || "w")){
        cout << "Enter the piece char correctly";
        return 0;
    }

    cout << "Enter row and column (0-7): ";
    cin >> row >> col;

    if (gameBoard.placePiece(row, col, piece))
        cout << "Piece placed successfully.\n";
    else
        cout << "Invalid position.\n";

    gameBoard.printBoard();
    return 0;
}