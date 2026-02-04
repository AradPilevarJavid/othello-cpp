#include <iostream>
#include "Board.h"
using namespace std;

Board::Board() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = '.';
}

bool Board::placePiece(int row, int col, char Piece){
    return true;
}

void Board::printBoard() const {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
            cout << board[i][j] << ' ';
        cout << '\n';
    }
}