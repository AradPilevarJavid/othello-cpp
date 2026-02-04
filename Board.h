#ifndef BOARD_H_
#define BOARD_H_
#define size 8

class Board {
private:
    char board[size][size];

public:
    Board();
    void printBoard() const;
    bool placePiece(int row, int col, char piece);
};

#endif