#include <iostream>
#include "Board.h"

#define RESET "\033[0m"
#define BLUE_FG "\033[34m"

using namespace std;

Board::Board() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = "";

    board[3][3] = u8"🟩";
    board[4][4] = u8"🟩";
    board[3][4] = u8"⬜";
    board[4][3] = u8"⬜";
}

bool Board::inBounds(int row, int col) const {
    return (row >= 0 && row < 8 && col >= 0 && col < 8);
}

string Board::opponent(const string& piece) const {
    return piece == u8"🟩" ? u8"⬜" : u8"🟩";
}

bool Board::hasFlippable(int row, int col, const string& piece) const {
    int dx[8] = {-1,-1,-1,0,0,1,1,1};
    int dy[8] = {-1,0,1,-1,1,-1,0,1};
    string o = opponent(piece);

    for (int d = 0; d < 8; d++) {
        int i = row + dx[d], j = col + dy[d];
        if (!inBounds(i,j) || board[i][j] != o) continue;
        i += dx[d]; j += dy[d];
        while (inBounds(i,j) && board[i][j] == o) {
            i += dx[d]; j += dy[d];
        }
        if (inBounds(i,j) && board[i][j] == piece) return true;
    }
    return false;
}

void Board::flip(int row, int col, const string& piece) {
    int dx[8] = {-1,-1,-1,0,0,1,1,1};
    int dy[8] = {-1,0,1,-1,1,-1,0,1};
    string o = opponent(piece);

    for (int d = 0; d < 8; d++) {
        int i = row + dx[d], j = col + dy[d];
        if (!inBounds(i,j) || board[i][j] != o) continue;
        int si = i, sj = j;
        i += dx[d]; j += dy[d];
        while (inBounds(i,j) && board[i][j] == o) {
            i += dx[d]; j += dy[d];
        }
        if (inBounds(i,j) && board[i][j] == piece) {
            while (si != i || sj != j) {
                board[si][sj] = piece;
                si += dx[d];
                sj += dy[d];
            }
        }
    }
}

bool Board::placePiece(int row, int col, const string& piece) {
    if (!inBounds(row,col) || board[row][col] != "") return false;
    if (!hasFlippable(row,col,piece)) return false;
    board[row][col] = piece;
    flip(row,col,piece);
    return true;
}

bool Board::hasAnyMove(const string& piece) const {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == "" && hasFlippable(i,j,piece))
                return true;
    return false;
}

int Board::count(const string& piece) const {
    int c = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == piece) c++;
    return c;
}

void Board::print() const {
    cout << " " << BLUE_FG 
         << "  ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐" 
         << RESET << "\n";

    for (int i = 0; i < 8; i++) {
        cout << " " << i << " " << BLUE_FG << "│" << RESET;
        for (int j = 0; j < 8; j++) {
            if (board[i][j].length() > 1) {
                cout << " " << board[i][j] << "   " << BLUE_FG << "│" << RESET;
            } else {
                cout << "  " << board[i][j] << "    " << BLUE_FG << "│" << RESET;
            }
        }
        cout << "\n";
        if (i != 7) {
            cout << " " << BLUE_FG 
                 << "  ├──────┼──────┼──────┼──────┼──────┼──────┼──────┼──────┤" 
                 << RESET << "\n";
        }
    }

    cout << " " << BLUE_FG 
         << "  └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘" 
         << RESET << "\n";

    cout << "     0      1      2      3      4      5      6      7 \n";
}