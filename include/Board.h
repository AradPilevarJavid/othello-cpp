#ifndef BOARD_H_
#define BOARD_H_

#include <string>

class Board {
private:
    static const int SIZE = 8;
    std::string board[SIZE][SIZE];
    bool inBounds(int r, int c) const;
    std::string opponent(const std::string& p) const;
    bool hasFlippable(int r, int c, const std::string& p) const;
    void flip(int r, int c, const std::string& p);

public:
    Board();
    bool placePiece(int r, int c, const std::string& p);
    bool hasAnyMove(const std::string& p) const;
    void print() const;
    int count(const std::string& p) const;
};

#endif
