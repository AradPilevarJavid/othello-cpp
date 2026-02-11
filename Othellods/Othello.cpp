#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOARD_SIZE = 8;
const char EMPTY = '.';
const char BLACK = 'B';
const char WHITE = 'W';

class OthelloGame {
private:
    char board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer;
    int blackScore;
    int whiteScore;
    bool gameOver;
    
    const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

public:
    OthelloGame() {
        initializeBoard();
        currentPlayer = BLACK;
        gameOver = false;
        updateScores();
    }
    
    void initializeBoard() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = EMPTY;
            }
        }
        
        int mid = BOARD_SIZE / 2;
        board[mid-1][mid-1] = WHITE;
        board[mid][mid] = WHITE;
        board[mid-1][mid] = BLACK;
        board[mid][mid-1] = BLACK;
    }
    
    void displayBoard() {
        cout << "\n   ";
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << i << " ";
        }
        cout << endl;
        
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << i << " |";
            for (int j = 0; j < BOARD_SIZE; j++) {
                cout << board[i][j] << "|";
            }
            cout << endl;
        }
        
        updateScores();
        cout << "\nScore: Black (" << BLACK << ") = " << blackScore 
             << ", White (" << WHITE << ") = " << whiteScore << endl;
        cout << "Current player: " << currentPlayer << endl;
    }
    
    void updateScores() {
        blackScore = 0;
        whiteScore = 0;
        
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == BLACK) {
                    blackScore++;
                } else if (board[i][j] == WHITE) {
                    whiteScore++;
                }
            }
        }
    }
    
    bool isValidMove(int row, int col) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != EMPTY) {
            return false;
        }
        
        for (int dir = 0; dir < 8; dir++) {
            if (canFlipInDirection(row, col, dx[dir], dy[dir])) {
                return true;
            }
        }
        
        return false;
    }
    
    bool canFlipInDirection(int row, int col, int dx, int dy) {
        char opponent = (currentPlayer == BLACK) ? WHITE : BLACK;
        int x = row + dx;
        int y = col + dy;
        bool foundOpponent = false;
        
        while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
            if (board[x][y] == EMPTY) {
                return false;
            }
            if (board[x][y] == opponent) {
                foundOpponent = true;
            } else if (board[x][y] == currentPlayer) {
                return foundOpponent;
            } else {
                return false;
            }
            x += dx;
            y += dy;
        }
        
        return false;
    }
    
    void makeMove(int row, int col) {
        if (!isValidMove(row, col)) {
            cout << "Invalid move! Try again.\n";
            return;
        }
        
        board[row][col] = currentPlayer;
        
        for (int dir = 0; dir < 8; dir++) {
            flipPiecesInDirection(row, col, dx[dir], dy[dir]);
        }
        
        switchPlayer();
        
        if (!hasValidMove()) {
            cout << "No valid moves for " << currentPlayer << ". Switching player.\n";
            switchPlayer();
            
            if (!hasValidMove()) {
                gameOver = true;
                cout << "\nGame Over!\n";
                determineWinner();
            }
        }
    }
    
    void flipPiecesInDirection(int row, int col, int dx, int dy) {
        if (!canFlipInDirection(row, col, dx, dy)) {
            return;
        }
        
        char opponent = (currentPlayer == BLACK) ? WHITE : BLACK;
        int x = row + dx;
        int y = col + dy;
        
        while (board[x][y] == opponent) {
            board[x][y] = currentPlayer;
            x += dx;
            y += dy;
        }
    }
    
    bool hasValidMove() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY && isValidMove(i, j)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    void switchPlayer() {
        currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
    }
    
    void determineWinner() {
        updateScores();
        cout << "\nFinal Score:\n";
        cout << "Black: " << blackScore << endl;
        cout << "White: " << whiteScore << endl;
        
        if (blackScore > whiteScore) {
            cout << "Black wins!\n";
        } else if (whiteScore > blackScore) {
            cout << "White wins!\n";
        } else {
            cout << "It's a tie!\n";
        }
    }
    
    void makeAIMove() {
        int bestRow = -1, bestCol = -1;
        int maxFlips = -1;
        
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (isValidMove(i, j)) {
                    int flips = countFlips(i, j);
                    if (flips > maxFlips) {
                        maxFlips = flips;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
        
        if (bestRow != -1 && bestCol != -1) {
            cout << "AI chooses position (" << bestRow << ", " << bestCol << ")\n";
            makeMove(bestRow, bestCol);
        }
    }
    
    int countFlips(int row, int col) {
        int totalFlips = 0;
        board[row][col] = currentPlayer;
        
        for (int dir = 0; dir < 8; dir++) {
            char opponent = (currentPlayer == BLACK) ? WHITE : BLACK;
            int x = row + dx[dir];
            int y = col + dy[dir];
            int dirFlips = 0;
            
            while (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == opponent) {
                dirFlips++;
                x += dx[dir];
                y += dy[dir];
            }
            
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == currentPlayer) {
                totalFlips += dirFlips;
            }
        }
        
        board[row][col] = EMPTY;
        
        return totalFlips;
    }
    
    void play() {
        int gameMode;
        cout << "=== OTHELLO GAME ===\n";
        cout << "Select game mode:\n";
        cout << "1. Player vs Player\n";
        cout << "2. Player vs AI\n";
        cout << "3. AI vs AI\n";
        cout << "Enter choice (1-3): ";
        cin >> gameMode;
        
        srand(time(0));
        
        while (!gameOver) {
            displayBoard();
            
            if (gameMode == 1 || (gameMode == 2 && currentPlayer == BLACK)) {
                int row, col;
                cout << "\nEnter your move (row column): ";
                cin >> row >> col;
                
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Enter two numbers between 0 and 7.\n";
                    continue;
                }
                
                makeMove(row, col);
            } else {
                cout << "\nAI (" << currentPlayer << ") is thinking...\n";
                makeAIMove();
            }
        }
    }
};

int main() {
    OthelloGame game;
    game.play();
    return 0;
}