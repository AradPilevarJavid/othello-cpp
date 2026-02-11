#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <string>

class Game {
private:
    Board board;
    std::string currentPlayer;
    
public:
    Game();
    void run();
    void intro();
    int menu();
    void clearScreen();
    void choosePiece();
    void playGame();
    // void showScoreboard();
    bool saveGame();
    bool loadGame();
    // void deleteSave();
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    void showSaveMenu();
};

#endif