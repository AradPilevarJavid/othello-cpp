#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>
#include <random>
#include <limits>
#include "Game.h"

#define RESET "\033[0m"
#define BLUE_FG "\033[34m"
#define RED_FG "\033[31m"
#define GREEN_FG "\033[32m"
#define YELLOW_FG "\033[33m"
#define BRIGHTBLACK_FG "\033[90m"

Game::Game() {
    currentPlayer = u8"🟩";
    singlePlayer = false;
    humanPlayer = u8"🟩";
    historyIndex = -1;
    aiMode = 1;
    
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    randomGenerator = std::mt19937(seed);
}

void Game::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    std::cout.flush();
}

void Game::chooseGameMode() {
    clearScreen();
    std::cout << BLUE_FG << "   ┌───────────────────────────────────────────────────────┐\n";
    std::cout << "   │" << RESET << "                   Select Game Mode                    " << BLUE_FG << "│\n";
    std::cout << "   ├───────────────────────────────────────────────────────┤\n";
    std::cout << "   │" << RESET << "   [1] Single Player (vs AI)                           " << BLUE_FG << "│\n";
    std::cout << "   │" << RESET << "   [2] Two Players                                     " << BLUE_FG << "│\n";
    std::cout << "   └───────────────────────────────────────────────────────┘\n" << RESET;
    
    int choice;
    std::cout << "   Enter your choice: ";
    std::cin >> choice;

    if (choice == 1) {
        singlePlayer = true;
        clearScreen();

        std::cout << BLUE_FG << "   ┌───────────────────────────────────────────────────────┐\n";
        std::cout << "   │" << RESET << "                   Choose AI:                          " << BLUE_FG << "│\n";
        std::cout << "   ├───────────────────────────────────────────────────────┤\n";
        std::cout << "   │" << RESET << "   1. Greedy AI                                        " << BLUE_FG << "│\n";
        std::cout << "   │" << RESET << "   2. Minimax AI                                       " << BLUE_FG << "│\n";
        std::cout << "   └───────────────────────────────────────────────────────┘\n" << RESET;
        std::cout << "Choice: ";
        std::cin >> aiMode;

        choosePiece();
        humanPlayer = currentPlayer;
    } else {
        singlePlayer = false;
        choosePiece();
    }
}

void Game::startTimer() {
    gameTimeStart = std::chrono::steady_clock::now();
}

double Game::gameDurationSeconds() {
    gameTimeEnd = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(gameTimeEnd - gameTimeStart);
    return duration.count() / 1000.0;
}

bool Game::saveToFile(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    out << (singlePlayer ? 'S' : 'M') << '\n';
    if (singlePlayer) {
        out << (humanPlayer == u8"🟩" ? 'G' : 'W') << ' ' << aiMode << '\n';
    }

    out << (currentPlayer == u8"🟩" ? 'G' : 'W') << '\n';

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::string piece = board.getPiece(i, j);
            if (piece == u8"🟩") out << 'G';
            else if (piece == u8"⬜") out << 'W';
            else out << '.';
        }
        out << '\n';
    }

    out << gameDurationSeconds() << '\n';
    return out.good();
}

bool Game::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    // Clear the board before filling it
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            board.setPiece(i, j, "");

    // 1. Game mode
    char mode;
    if (!(in >> mode)) return false;

    if (mode == 'S') {
        singlePlayer = true;
        char humanChar;
        int aiChoice;
        if (!(in >> humanChar >> aiChoice)) return false;
        humanPlayer = (humanChar == 'G') ? u8"🟩" : u8"⬜";
        aiMode = aiChoice;
    } else if (mode == 'M') {
        singlePlayer = false;
        humanPlayer = u8"🟩";
    } else {
        return false;
    }

    // 2. Current player
    char playerChar;
    if (!(in >> playerChar)) return false;
    currentPlayer = (playerChar == 'G') ? u8"🟩" : u8"⬜";

    // 3. Board rows
    for (int i = 0; i < 8; ++i) {
        std::string row;
        if (!(in >> row) || row.length() != 8) return false;
        for (int j = 0; j < 8; ++j) {
            char c = row[j];
            if (c == 'G') board.setPiece(i, j, u8"🟩");
            else if (c == 'W') board.setPiece(i, j, u8"⬜");
            else if (c == '.') board.setPiece(i, j, "");
            else return false;
        }
    }

    // 4. Elapsed time (optional)
    double savedTime = 0.0;
    if (in >> savedTime) {
        gameTimeStart = std::chrono::steady_clock::now() -
                        std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                            std::chrono::duration<double>(savedTime));
    } else {
        startTimer();
    }

    // 5. Reset undo history to the loaded board
    history.clear();
    historyIndex = -1;
    saveToHistory();

    return true;
}

void Game::showSaveMenu() {
    clearScreen();
    std::cout << BLUE_FG << "   ┌───────────────────────────────────────────────────────┐\n";
    std::cout << "   │" << RESET << "                     Save Game                         " << BLUE_FG << "│\n";
    std::cout << "   ├───────────────────────────────────────────────────────┤\n";
    std::cout << "   │" << RESET << "   Enter filename (e.g., game1.txt):                   " << BLUE_FG << "│\n";
    std::cout << "   └───────────────────────────────────────────────────────┘\n" << RESET;
    std::cout << "   Filename: ";

    std::string filename;
    std::getline(std::cin, filename);

    if (filename.empty()) {
        std::cout << RED_FG << "\n   Empty filename. Save cancelled." << RESET << "\n";
    } else if (saveToFile(filename)) {
        std::cout << GREEN_FG << "\n   Game saved successfully to " << filename << RESET << "\n";
    } else {
        std::cout << RED_FG << "\n   Error saving game!" << RESET << "\n";
    }

    std::cout << "\n   Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool Game::saveGame() {
    return saveToFile("othello_save.txt");
}

bool Game::loadGame() {
    if (loadFromFile("othello_save.txt")) {
        std::cout << GREEN_FG << "Game loaded!\n" << RESET;
        return true;
    }
    std::cout << RED_FG << "No saved game found!\n" << RESET;
    return false;
}

void Game::intro() {
    std::string title = "                 WELCOME TO THE OTHELLO GAME.";
    std::cout << "   ═════════════════════════════════════════════════════════\n";
    for (char c : title) {
        std::cout << "\033[32m" << c << "\033[0m";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    std::cout << "\n   ═════════════════════════════════════════════════════════\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    clearScreen();
}

int Game::menu() {
    std::cout << "\033[34m";
    std::cout << "   ┌───────────────────────────────────────────────────────┐\n";
    std::cout << "   │                       Othello                         │\n";
    std::cout << "   ├───────────────────────────────────────────────────────┤\n";
    std::cout << "   │\033[32m   [0] New Game                                        \033[34m│\n";
    std::cout << "   │\033[32m   [1] Load from othello_save.txt                      \033[34m│\n";
    std::cout << "   │\033[32m   [2] Load from file                                  \033[34m│\n";
    std::cout << "   │\033[32m   [3] Scoreboard                                      \033[34m│\n";
    std::cout << "   │\033[31m   [4] Exit                                            \033[34m│\n";
    std::cout << "   └───────────────────────────────────────────────────────┘\n\033[0m";
    
    int c;
    std::cout << "Choice: ";
    std::cin >> c;
    return c;
}

void Game::choosePiece() {
    char choice;
    while (true) {
        std::cout << BRIGHTBLACK_FG << "Which piece do you want to begin the game? (" 
                  << GREEN_FG << "G " << BRIGHTBLACK_FG << "/" << RESET << " W" 
                  << BRIGHTBLACK_FG << "): " << RESET;
        std::cin >> choice;
        
        if (choice == 'G' || choice == 'g') {
            currentPlayer = u8"🟩";
            break;
        } else if (choice == 'W' || choice == 'w') {
            currentPlayer = u8"⬜";
            break;
        } else {
            clearScreen();
            std::cout << RED_FG << "❌ Invalid choice! Please enter G or W.\n" << RESET;
        }
    }
}

void Game::playSinglePlayer() {
    clearScreen();
    history.clear();
    historyIndex = -1;
    saveToHistory();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << BLUE_FG << "⏱️ Time: " << std::fixed << std::setprecision(1) << gameDurationSeconds() << "s" << RESET << "\n";
        board.print(currentPlayer);
        
        if (currentPlayer == humanPlayer) {
            std::cout << "Your move (" << currentPlayer << "), enter row and column (e.g., 3 4), 'u' to undo, 'r' to redo, 's' to save or 'm' to menu: ";
            std::string input;
            std::getline(std::cin, input);
            
            if (input == "u" || input == "U") {
                undo();
                clearScreen();
                continue;
            }
            if (input == "r" || input == "R") {
                redo();
                clearScreen();
                continue;
            }
            if (input == "s" || input == "S") {
                showSaveMenu();
                clearScreen();
                continue;
            }
            if (input == "m" || input == "M") {
                clearScreen();
                return;
            }

            int r, c;
            if (sscanf(input.c_str(), "%d %d", &r, &c) != 2 || !board.canPlace(r, c, currentPlayer)) {
                clearScreen();
                std::cout << RED_FG << "❌ Invalid move! Try again.\n" << RESET;
                continue;
            }
            
            board.placePiece(r, c, currentPlayer);
            saveToHistory();
        } else {
            std::cout << "AI is thinking...\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            AI ai(board, currentPlayer);
            Move bestMove;
            if (aiMode == 1) {
                 bestMove = ai.findBestMoveGreedy();
            } else {
                bestMove = ai.findBestMoveMinimax(5);
            }
            
            if (bestMove.row != -1) {
                board.placePiece(bestMove.row, bestMove.col, currentPlayer);
                saveToHistory();
            } else {
                std::cout << "AI has no moves!\n";
            }
        }
        
        currentPlayer = (currentPlayer == u8"🟩") ? u8"⬜" : u8"🟩";
        
        if (!board.hasAnyMove(u8"🟩") && !board.hasAnyMove(u8"⬜")) {
            endGame();
            break;
        }
        if (!board.hasAnyMove(currentPlayer)) {
            std::cout << YELLOW_FG << "Player " << currentPlayer << " has no valid moves! Skipping turn.\n" << RESET;
            currentPlayer = (currentPlayer == u8"🟩") ? u8"⬜" : u8"🟩";
        }
        clearScreen();
    }
}

void Game::playTwoPlayer() {
    clearScreen();
    history.clear();
    historyIndex = -1;
    saveToHistory();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << BLUE_FG << "⏱️ Time: " << std::fixed << std::setprecision(1) << gameDurationSeconds() << "s" << RESET << "\n";
        board.print(currentPlayer);
        
        std::cout << "Player " << currentPlayer << ", enter your move (row col), 'u' to undo, 'r' to redo, 's' to save or 'm' to menu: ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "u" || input == "U") {
            undo();
            clearScreen();
            continue;
        }
        if (input == "r" || input == "R") {
            redo();
            clearScreen();
            continue;
        }
        if (input == "s" || input == "S") {
            showSaveMenu();
            clearScreen();
            continue;
        }
        if (input == "m" || input == "M") {
            clearScreen();
            return;
        }
        
        int r, c;
        if (sscanf(input.c_str(), "%d %d", &r, &c) != 2 || !board.canPlace(r, c, currentPlayer)) {
            clearScreen();
            std::cout << RED_FG << "❌ Invalid move! Try again.\n" << RESET;
            continue;
        }
        
        board.placePiece(r, c, currentPlayer);
        saveToHistory();
        
        currentPlayer = (currentPlayer == u8"🟩") ? u8"⬜" : u8"🟩";
        
        if (!board.hasAnyMove(u8"🟩") && !board.hasAnyMove(u8"⬜")) {
            endGame();
            break;
        }
        if (!board.hasAnyMove(currentPlayer)) {
            std::cout << YELLOW_FG << "Player " << currentPlayer << " has no valid moves! Skipping turn.\n" << RESET;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            currentPlayer = (currentPlayer == u8"🟩") ? u8"⬜" : u8"🟩";
        }
        
        clearScreen();
    }
}

void Game::playGame() {
    if (singlePlayer) {
        playSinglePlayer();
    } else {
        playTwoPlayer();
    }
}

void Game::endGame() {
    clearScreen();
    board.print(currentPlayer);
    int green_score = board.count(u8"🟩");
    int white_score = board.count(u8"⬜");
    
    std::cout << "Game Over!\n";
    std::cout << "Scores:\n";
    std::cout << u8"🟩" << ": " << green_score << "\n";
    std::cout << u8"⬜" << ": " << white_score << "\n";
    
    std::string resultStr;
    if (green_score > white_score) {
        std::cout << GREEN_FG << u8"🟩" << " wins!\n" << RESET;
        if (singlePlayer) {
            if (humanPlayer == u8"🟩") resultStr = "Human (Green)";
            else resultStr = "Computer (Green)";
        } else {
            resultStr = "Green (2P)";
        }
    } else if (white_score > green_score) {
        std::cout << "White wins!\n" << RESET;
        if (singlePlayer) {
            if (humanPlayer == u8"⬜") resultStr = "Human (White)";
            else resultStr = "Computer (White)";
        } else {
            resultStr = "White (2P)";
        }
    } else {
        std::cout << YELLOW_FG << "It's a draw!\n" << RESET;
        if (singlePlayer) resultStr = "Draw";
        else resultStr = "Draw (2P)";
    }
    
    saveResult(resultStr, 0, 0.0);
    
    std::cout << "Press Enter to return to the menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    clearScreen();
}

void Game::saveResult(const std::string& winner, int score, double time) {
    std::ofstream file("scoreboard.txt", std::ios::app);
    if (file.is_open()) {
        file << winner << "\n";
        file.close();
    }
}

void Game::loadScores() {
    std::ifstream file("scoreboard.txt");
    scores.clear();
    if (file.is_open()) {
        Score s;
        while (file >> s.winner >> s.score >> s.time) {
            scores.push_back(s);
        }
        file.close();
    }
}

void Game::showScoreboard() {
    clearScreen();
    std::ifstream score("scoreboard.txt");

    if (!score.is_open()) {
        std::cout << "No games recorded yet.\n";
    } else {
        std::string result;
        int gWins = 0, wWins = 0, draws = 0, humanWins = 0, computerWins = 0;

        while (std::getline(score, result)) {
            if (result == "Green (2P)") gWins++;
            else if (result == "White (2P)") wWins++;
            else if (result == "Draw (2P)") draws++;
            else if (result == "Human (Green)") humanWins++;
            else if (result == "Human (White)") humanWins++;
            else if (result == "Computer (Green)") computerWins++;
            else if (result == "Computer (White)") computerWins++;
            else if (result == "Draw") draws++;
        }

        std::cout << "Scoreboard:\n";
        std::cout << "┌─────────────────────┐\n";
        std::cout << "│ Green wins:   " << std::setw(4) << gWins << "  │\n";
        std::cout << "│ White wins:   " << std::setw(4) << wWins << "  │\n";
        std::cout << "│ Draws:        " << std::setw(4) << draws << "  │\n";
        if (humanWins > 0 || computerWins > 0) {
            std::cout << "├─────────────────────┤\n";
            std::cout << "│ Human wins:   " << std::setw(4) << humanWins << "  │\n";
            std::cout << "│ Computer wins: " << std::setw(3) << computerWins << "  │\n";
        }
        std::cout << "└─────────────────────┘\n";

        score.close();
    }
    
    std::cout << "\nPress Enter to return to menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    clearScreen();
}

void Game::saveToHistory() {
    if (historyIndex < (int)history.size() - 1) {
        history.erase(history.begin() + historyIndex + 1, history.end());
    }
    history.push_back(board);
    historyIndex++;
}

void Game::undo() {
    if (historyIndex > 0) {
        historyIndex--;
        board = history[historyIndex];
    } else {
        std::cout << "Cannot undo further!\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Game::redo() {
    if (historyIndex < (int)history.size() - 1) {
        historyIndex++;
        board = history[historyIndex];
    } else {
        std::cout << "Cannot redo further!\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Game::run() {
    intro();
    while (true) {
        int choice = menu();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 0:
                chooseGameMode();
                board.init();
                startTimer();
                playGame();
                break;
            case 1:
                if (loadFromFile("othello_save.txt")) {
                    std::cout << "Loaded from othello_save.txt successfully!\n";
                    playGame();
                } else {
                    std::cout << "Failed to load from othello_save.txt!\n";
                }
                std::this_thread::sleep_for(std::chrono::seconds(2));
                clearScreen();
                break;
            case 2:
                {
                    std::cout << "Enter filename to load: ";
                    std::string filename;
                    std::cin >> filename;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (loadFromFile(filename)) {
                        std::cout << "Loaded from " << filename << " successfully!\n";
                        playGame();
                    } else {
                        std::cout << "Failed to load from " << filename << "!\n";
                    }
                }
                std::this_thread::sleep_for(std::chrono::seconds(2));
                clearScreen();
                break;
            case 3:
                showScoreboard();
                break;
            case 4:
                std::cout << "Exiting...\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                clearScreen();
                break;
        }
    }
}