#pragma once
#include "Board.h"
#include <string>
#include <chrono>
#include <random>

class Game {
private:
    Board board;
    std::string currentPlayer;
    std::string humanPlayer;
    bool singlePlayer;
    std::chrono::steady_clock::time_point gameTimeStart;
    std::chrono::steady_clock::time_point gameTimeEnd;
    std::mt19937 randomGenerator;

public:
    Game();
    void run();
    void playGame();
    void playSinglePlayer();
    void playTwoPlayer();
    void intro();
    void choosePiece();
    void chooseGameMode();
    int menu();
    void clearScreen();
    bool saveGame();
    bool loadGame();
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    void showSaveMenu();
    double gameDurationSeconds();
    void startTimer();
    void getComputerMove(int& row, int& col);
};