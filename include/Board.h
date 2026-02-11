#ifndef BOARD_H_
#define BOARD_H_

#include <string>

class Board {
private:
    static const int SIZE = 8;
    std::string board[SIZE][SIZE];
    bool inBounds(int row, int col) const;
    std::string opponent(const std::string& piece) const;
    bool hasFlippable(int row, int col, const std::string& piece) const;
    void flip(int row, int col, const std::string& piece);

public:
    Board();
    bool placePiece(int row, int col, const std::string& piece);
    bool hasAnyMove(const std::string& piece) const;
    void print() const;
    int count(const std::string& piece) const;
    void save(std::ostream& out) const;
    void load(std::istream& in);
    std::string getPiece(int row, int col) const;
    void setPiece(int row, int col, const std::string& piece);
};

#endif