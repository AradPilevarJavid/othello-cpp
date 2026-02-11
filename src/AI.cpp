#include "AI.h"
#include <chrono>
#include <algorithm>

AI::AI() {
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    rng = std::mt19937(static_cast<unsigned>(seed));
}

std::pair<int, int> AI::getRandomMove(const Board& board, const std::string& piece) {
    std::vector<std::pair<int, int>> validMoves;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.canPlace(i, j, piece)) {
                validMoves.push_back({i, j});
            }
        }
    }
    
    if (validMoves.empty()) {
        return {-1, -1};
    }
    
    std::uniform_int_distribution<int> dist(0, validMoves.size() - 1);
    return validMoves[dist(rng)];
}

std::pair<int, int> AI::getGreedyMove(const Board& board, const std::string& piece) {
    std::vector<std::pair<int, int>> validMoves;
    std::vector<int> moveScores;
    int maxScore = -1;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.canPlace(i, j, piece)) {
                Board tempBoard = board;
                tempBoard.placePiece(i, j, piece);
                int score = tempBoard.count(piece);
                validMoves.push_back({i, j});
                moveScores.push_back(score);
                if (score > maxScore) maxScore = score;
            }
        }
    }
    
    if (validMoves.empty()) {
        return {-1, -1};
    }
    
    std::vector<std::pair<int, int>> bestMoves;
    for (size_t i = 0; i < validMoves.size(); i++) {
        if (moveScores[i] == maxScore) {
            bestMoves.push_back(validMoves[i]);
        }
    }
    
    std::uniform_int_distribution<int> dist(0, bestMoves.size() - 1);
    return bestMoves[dist(rng)];
}

std::pair<int, int> AI::getMove(const Board& board, const std::string& piece) {
    return getGreedyMove(board, piece);
}