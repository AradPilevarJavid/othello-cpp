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

bool Board::inBounds(int r, int c) const {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

string Board::opponent(const string& p) const {
    return p == u8"🟩" ? u8"⬜" : u8"🟩";
}

bool Board::hasFlippable(int r, int c, const string& p) const {
    int dx[8] = {-1,-1,-1,0,0,1,1,1};
    int dy[8] = {-1,0,1,-1,1,-1,0,1};
    string o = opponent(p);

    for (int d = 0; d < 8; d++) {
        int i = r + dx[d], j = c + dy[d];
        if (!inBounds(i,j) || board[i][j] != o) continue;
        i += dx[d]; j += dy[d];
        while (inBounds(i,j) && board[i][j] == o) {
            i += dx[d]; j += dy[d];
        }
        if (inBounds(i,j) && board[i][j] == p) return true;
    }
    return false;
}

void Board::flip(int r, int c, const string& p) {
    int dx[8] = {-1,-1,-1,0,0,1,1,1};
    int dy[8] = {-1,0,1,-1,1,-1,0,1};
    string o = opponent(p);

    for (int d = 0; d < 8; d++) {
        int i = r + dx[d], j = c + dy[d];
        if (!inBounds(i,j) || board[i][j] != o) continue;
        int si = i, sj = j;
        i += dx[d]; j += dy[d];
        while (inBounds(i,j) && board[i][j] == o) {
            i += dx[d]; j += dy[d];
        }
        if (inBounds(i,j) && board[i][j] == p) {
            while (si != i || sj != j) {
                board[si][sj] = p;
                si += dx[d];
                sj += dy[d];
            }
        }
    }
}

bool Board::placePiece(int r, int c, const string& p) {
    if (!inBounds(r,c) || board[r][c] != "") return false;
    if (!hasFlippable(r,c,p)) return false;
    board[r][c] = p;
    flip(r,c,p);
    return true;
}

bool Board::hasAnyMove(const string& p) const {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == "" && hasFlippable(i,j,p))
                return true;
    return false;
}

int Board::count(const string& p) const {
    int c = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (board[i][j] == p) c++;
    return c;
}

void Board::print() const {
    cout << " " << BLUE_FG 
         << "  ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐" 
         << RESET << "\n";

    for (int i = 0; i < 8; i++) {
        cout << " " << i << " " << BLUE_FG << "│" << RESET;
        for (int j = 0; j < 8; j++) {
            if (board[i][j].length() > 1 || board[i][j] == u8"🟩" || board[i][j] == u8"⬜") {
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