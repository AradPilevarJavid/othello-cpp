#ifndef GAME_H_
#define GAME_H_

#include "Board.h"
#include <string>

class Game {
private:
    Board board;
    std::string current;

public:
    Game();
    void run();
};

#endif
