#pragma once
#include "Board.h"
#include <random>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>


struct Move {
    int row;
    int col;
};

class AI {
private:
    Board& board;
    std::string player;
    std::mt19937 rng;

    int minimax(Board currentBoard, int depth, bool isMaximizing);
    int evaluate(const Board& b) const;

public:
    AI(Board& b, const std::string& p);

    Move findBestMoveGreedy();
    Move findBestMoveMinimax(int depth);
};
