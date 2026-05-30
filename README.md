# Othello (C++)

A terminal-based Othello/Reversi game with single‑player (AI) and two‑player modes.

## Compile

```bash
g++ -std=c++17 -o othello src/main.cpp src/Game.cpp src/Board.cpp src/AI.cpp -I include
