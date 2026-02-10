#ifndef GAME_H_
#define GAME_H_

#include "Board.h"
#include <string>

class Game {
private:
    Board board;
    std::string currentPlayer;

    void clearScreen();
    void intro();
    int menu();

public:
    Game();
    void run();
    void choosePiece();
};

#endif