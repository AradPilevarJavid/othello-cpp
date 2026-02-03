#ifndef BOARD_H
#define BOARD_H

class Board {
private:
    char board[8][8];

public:
    Board();
    void print() const;
};

#endif