#pragma once
#include "Board.h"
#include "AI.h"
#include <string>
#include <chrono>
#include <random>
#include <vector>

typedef Board GameState;

struct Score {
    std::string winner;
    int score;
    double time;
};

class Game {
private:
    std::vector<GameState> history;
    int historyIndex = -1;
    
    Board board;
    std::string currentPlayer;
    std::string humanPlayer;
    bool singlePlayer;
    std::chrono::steady_clock::time_point gameTimeStart;
    std::chrono::steady_clock::time_point gameTimeEnd;
    std::mt19937 randomGenerator;
    
    std::vector<Score> scores;

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

    void saveToHistory();
    void undo();
    void redo();

    void showScoreboard();
    void saveResult(const std::string& winner, int score, double time);
    void endGame();                                                             
    void loadScores();

    int aiMode;
};
