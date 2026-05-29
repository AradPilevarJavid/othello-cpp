#include "AI.h"
#include <algorithm>

AI::AI(Board& b, const std::string& p) : board(b), player(p) {
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    rng = std::mt19937(static_cast<unsigned>(seed));
}


Move AI::findBestMoveGreedy() {
    std::vector<Move> bestMoves;
    int maxScore = -1;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.canPlace(i, j, this->player)) {
                Board tempBoard = board;
                tempBoard.placePiece(i, j, this->player);
                int currentScore = tempBoard.count(this->player);

                if (currentScore > maxScore) {
                    maxScore = currentScore;
                    bestMoves.clear();
                    bestMoves.push_back({i, j});
                } else if (currentScore == maxScore) {
                    bestMoves.push_back({i, j});
                }
            }
        }
    }

    if (bestMoves.empty()) {
        return {-1, -1};
    }

    std::uniform_int_distribution<int> dist(0, bestMoves.size() - 1);
    return bestMoves[dist(rng)];
}


Move AI::findBestMoveMinimax(int depth) {
    int bestScore = -10000;
    Move bestMove = {-1, -1};

    if (!board.hasAnyMove(this->player)) {
        return bestMove;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.canPlace(i, j, this->player)) {
                Board tempBoard = board;
                tempBoard.placePiece(i, j, this->player);
                
                int score = minimax(tempBoard, depth - 1, false);

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {i, j};
                }
            }
        }
    }
    return bestMove;
}


int AI::evaluate(const Board& b) const {
    std::string opponent = b.opponent(this->player);
    return b.count(this->player) - b.count(opponent);
}


int AI::minimax(Board currentBoard, int depth, bool isMaximizing) {
    std::string opponent = currentBoard.opponent(this->player);

    if (depth == 0 || (!currentBoard.hasAnyMove(this->player) && !currentBoard.hasAnyMove(opponent))) {
        return evaluate(currentBoard);
    }

    if (isMaximizing) {
        if (!currentBoard.hasAnyMove(this->player)) {
            return minimax(currentBoard, depth - 1, false);
        }
        int bestScore = -10000;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (currentBoard.canPlace(i, j, this->player)) {
                    Board tempBoard = currentBoard;
                    tempBoard.placePiece(i, j, this->player);
                    int score = minimax(tempBoard, depth - 1, false);
                    bestScore = std::max(bestScore, score);
                }
            }
        }
        return bestScore;
    } 
    else {
        if (!currentBoard.hasAnyMove(opponent)) {
            return minimax(currentBoard, depth - 1, true);
        }
        int bestScore = 10000;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (currentBoard.canPlace(i, j, opponent)) {
                    Board tempBoard = currentBoard;
                    tempBoard.placePiece(i, j, opponent);
                    int score = minimax(tempBoard, depth - 1, true);
                    bestScore = std::min(bestScore, score);
                }
            }
        }
        return bestScore;
    }
}