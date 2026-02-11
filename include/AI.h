#pragma once
#include "Board.h"
#include <random>
#include <vector>
#include <utility>

class AI {
private:
    std::mt19937 rng;
    
public:
    AI();
    std::pair<int, int> getMove(const Board& board, const std::string& piece);
    std::pair<int, int> getRandomMove(const Board& board, const std::string& piece);
    std::pair<int, int> getGreedyMove(const Board& board, const std::string& piece);
};