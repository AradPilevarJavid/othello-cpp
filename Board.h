#ifndef BOARD_H
#define BOARD_H

class Board {
    private:
        char board[8][8];

    public:
        Board();
        void printBoard();
        bool isInside(int row, int col);
        bool placePiece(int row, int col, char piece);
};

#endif