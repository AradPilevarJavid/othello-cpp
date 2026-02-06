#include <iostream>
#include "Board.h"
using namespace std;


Board::Board() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = '.';
}



bool Board::placePiece(int row, int col, char piece) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8){
        return false;
    }board[row][col] = piece;
    return true;
}

void Board::printBoard() const {
        cout << "   ┌───┬───┬───┬───┬───┬───┬───┬───┐" << "\n";

    for (int i = 0; i < 8; i++) {
        cout << " " << i << " │";
        for (int j = 0; j < 8; j++){
            cout << " · │";
        }cout << "\n";

        if (i != 7) {
            cout << "   ├───┼───┼───┼───┼───┼───┼───┼───┤" << "\n";
        }
    }
    cout << "   └───┴───┴───┴───┴───┴───┴───┴───┘" << "\n";
    cout << "     0   1   2   3   4   5   6   7 " << "\n";

}