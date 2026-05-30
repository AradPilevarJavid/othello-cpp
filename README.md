# 🎮 Othello (C++)

A terminal-based Othello/Reversi game with single‑player (AI) and two‑player modes.I made this game for the Basic programming course in the uni.

## Compile
```
g++ -std=c++17 -o othello src/main.cpp src/Game.cpp src/Board.cpp src/AI.cpp -I include
```

## Run
```
./othello
```

## Features
- Two player or vs AI (Greedy / Minimax)
- Save and load games
- Undo and redo moves
- Scoreboard

## 📁 Project Structure
```
othello/
├── src/
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Board.cpp
│   └── AI.cpp
├── include/
│   ├── Game.h
│   ├── Board.h
│   └── AI.h
├── README.md
└── LICENSE
```
