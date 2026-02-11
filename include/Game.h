#pragma once
#include "Board.h"
#include <string>
#include <chrono>

class Game {
private:
    Board board;
    std::string currentPlayer;
    std::chrono::steady_clock::time_point gameTimeStart;
    std::chrono::steady_clock::time_point gameTimeEnd;

public:
    Game();
    void run();
    void playGame();
    void intro();
    void choosePiece();
    int menu();
    void clearScreen();
    bool saveGame();
    bool loadGame();
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    void showSaveMenu();
    double gameDurationSeconds();
    void startTimer();
};
