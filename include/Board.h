#pragma once
#include <string>

class Board {
private:
    static const int SIZE = 8;
    std::string board[SIZE][SIZE];

public:
    Board();
    std::string getPiece(int row, int col) const;
    void setPiece(int row, int col, const std::string& piece);
    void save(std::ostream& out) const;
    void load(std::istream& in);

    bool inBounds(int row, int col) const;
    std::string opponent(const std::string& piece) const;
    bool hasFlippable(int row, int col, const std::string& piece) const;
    void flip(int row, int col, const std::string& piece);
    bool placePiece(int row, int col, const std::string& piece);
    bool hasAnyMove(const std::string& piece) const;
    int count(const std::string& piece) const;
    bool canPlace(int row, int col, const std::string& piece) const;
    void print(const std::string& highlightPiece = "") const;
    void showValidMoves(const std::string& piece);
};
