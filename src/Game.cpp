#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>
#include <random>
#include <vector>
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
    
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    randomGenerator = std::mt19937(seed);
}

//I love this function:
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
    
    out << (currentPlayer == u8"🟩" ? 'G' : 'W') << "\n";
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::string piece = board.getPiece(i, j);
            if (piece == u8"🟩") out << 'G';
            else if (piece == u8"⬜") out << 'W';
            else out << '.';
        }
        out << "\n";
    }
    out.close();
    return true;
}

bool Game::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;
    
    char player;
    in >> player;
    currentPlayer = (player == 'G') ? u8"🟩" : u8"⬜";
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char c;
            in >> c;
            if (c == 'G') board.setPiece(i, j, u8"🟩");
            else if (c == 'W') board.setPiece(i, j, u8"⬜");
            else board.setPiece(i, j, "");
        }
    }
    in.close();
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
    std::cin >> filename;
    
    if (saveToFile(filename)) {
        std::cout << GREEN_FG << "\n   Game saved successfully to " << filename << RESET << "\n";
    } else {
        std::cout << RED_FG << "\n   Error saving game!" << RESET << "\n";
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

bool Game::saveGame() {
    std::ofstream out("othello_save.txt");
    if (!out.is_open()) {
        return false;
    }
    out << currentPlayer << "\n";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            out << board.getPiece(i, j) << " ";
        }
        out << "\n";
    }
    out.close();
    std::cout << GREEN_FG << "Game saved!\n" << RESET;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
}

bool Game::loadGame() {
    std::ifstream in("othello_save.txt");
    if (!in.is_open()) {
        std::cout << RED_FG << "No saved game found!\n" << RESET;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return false;
    }
    in >> currentPlayer;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::string piece;
            in >> piece;
            board.setPiece(i, j, piece);
        }
    }
    in.close();
    std::cout << GREEN_FG << "Game loaded!\n" << RESET;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
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
    for (int i = 0; i < 11; i++) {
        std::cout << "   │";
        if (i == 2) std::cout << "                       Othello                         │";
        else if (i == 4) std::cout << "\033[32m   [0] New Game                                        \033[34m│";
        else if (i == 5) std::cout << "\033[32m   [1] Load from save.txt                              \033[34m│";
        else if (i == 6) std::cout << "\033[32m   [2] Load from file                                  \033[34m│";
        else if (i == 7) std::cout << "\033[32m   [3] Scoreboard                                      \033[34m│";
        else if (i == 8) std::cout << "\033[31m   [4] Exit                                            \033[34m│";
        else std::cout << "                                                       │";
        std::cout << "\n";
    }
    std::cout << "   └───────────────────────────────────────────────────────┘\n\033[0m";
    
    int c;
    std::cin >> c;
    return c;
}

void Game::choosePiece() {
    char choice;

    piece_choice:
        std::cout << BRIGHTBLACK_FG << "Which piece do you want to begin the game? (" 
                  << GREEN_FG << "G " << BRIGHTBLACK_FG << "/" << RESET << " W" 
                  << BRIGHTBLACK_FG << "): " << RESET;
        
        std::cin >> choice;
        
        if (choice == 'G' || choice == 'g') {
            currentPlayer = u8"🟩";
        }
        else if (choice == 'W' || choice == 'w') {
            currentPlayer = u8"⬜";
        }
        else {
            clearScreen();
            std::cout << RED_FG << "❌ Invalid choice! Please enter G or W.\n" << RESET;
            goto piece_choice;
        }
}

void Game::getComputerMove(int& row, int& col) {
    std::vector<int> validRows;
    std::vector<int> validCols;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board.canPlace(i, j, currentPlayer)) {
                validRows.push_back(i);
                validCols.push_back(j);
            }
        }
    }
    
    if (validRows.size() > 0) {
        std::uniform_int_distribution<int> distribution(0, validRows.size() - 1);
        int index = distribution(randomGenerator);
        row = validRows[index];
        col = validCols[index];
    } else {
        row = -1;
        col = -1;
    }
}

void Game::playSinglePlayer() {
    clearScreen();
    startTimer();
    
    while (true) {
        std::cout << BLUE_FG << "⏱️ Time: " << RESET 
                  << std::fixed << std::setprecision(1) 
                  << gameDurationSeconds() << " seconds" 
                  << "  |  ";
        
        if (currentPlayer == u8"🟩") std::cout << "🟩 Green";
        else std::cout << "⬜ White";
        
        if (currentPlayer == humanPlayer) std::cout << " (YOU)";
        else std::cout << " (COMPUTER)";
        
        std::cout << "\n\n";

        board.showValidMoves(currentPlayer);

        if (!board.hasAnyMove(currentPlayer)) {
            std::string other = currentPlayer == u8"🟩" ? u8"⬜" : u8"🟩";
            if (!board.hasAnyMove(other)) break;
            currentPlayer = other;
            continue;
        }

        if (currentPlayer == humanPlayer) {
            std::cout << "Your move (row col) or 's' to save: ";
            std::string input;
            std::cin >> input;
            
            if (input == "s" || input == "S") {
                showSaveMenu();
                clearScreen();
                continue;
            }
            
            int row = std::stoi(input);
            int col;
            std::cin >> col;

            if (!board.placePiece(row, col, currentPlayer)) {
                std::cout << RED_FG << "Invalid move!" << RESET << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                clearScreen();
                continue;
            }
        } 
        else {
            std::cout << "Computer is thinking";
            std::cout.flush();
            for (int i = 0; i < 3; i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                std::cout << ".";
                std::cout.flush();
            }
            std::cout << "\n";
            
            int row, col;
            getComputerMove(row, col);
            
            if (row == -1) {
                std::cout << "Computer has no move!\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                currentPlayer = humanPlayer;
                continue;
            }
            
            board.placePiece(row, col, currentPlayer);
            std::cout << "Computer placed at (" << row << ", " << col << ")\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        currentPlayer = currentPlayer == u8"🟩" ? u8"⬜" : u8"🟩";
        clearScreen();
    }

    board.print();
    int g = board.count(u8"🟩");
    int w = board.count(u8"⬜");
    
    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "🟩 Green: " << g << "\n";
    std::cout << "⬜ White: " << w << "\n";
    
    if (g > w) {
        if (humanPlayer == u8"🟩") 
            std::cout << GREEN_FG << "🎉 You win!" << RESET << "\n";
        else 
            std::cout << RED_FG << "Computer wins!" << RESET << "\n";
    }
    else if (w > g) {
        if (humanPlayer == u8"⬜") 
            std::cout << GREEN_FG << "🎉 You win!" << RESET << "\n";
        else 
            std::cout << RED_FG << "Computer wins!" << RESET << "\n";
    }
    else {
        std::cout << YELLOW_FG << "Draw!" << RESET << "\n";
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();

    std::ofstream score("scoreboard.txt", std::ios::app);
    if (score.is_open()) {
        if (g > w) {
            if (humanPlayer == u8"🟩") score << "Human (Green)\n";
            else score << "Computer (Green)\n";
        }
        else if (w > g) {
            if (humanPlayer == u8"⬜") score << "Human (White)\n";
            else score << "Computer (White)\n";
        }
        else score << "Draw\n";
        score.close();
    }
}

void Game::playTwoPlayer() {
    clearScreen();
    startTimer();

    while (true) {
        std::cout << BLUE_FG << "⏱️ Time: " << RESET 
                  << std::fixed << std::setprecision(1) 
                  << gameDurationSeconds() << " seconds" 
                  << "  |  " << (currentPlayer == u8"🟩" ? "🟩 Green's turn" : "⬜ White's turn") 
                  << "\n\n";

        board.showValidMoves(currentPlayer);

        if (!board.hasAnyMove(currentPlayer)) {
            std::string other = currentPlayer == u8"🟩" ? u8"⬜" : u8"🟩";
            if (!board.hasAnyMove(other)) break;
            currentPlayer = other;
            continue;
        }

        std::cout << (currentPlayer == u8"🟩" ? "Green" : "White") << " move (row col) or 's' to save: ";
        std::string input;
        std::cin >> input;
        
        if (input == "s" || input == "S") {
            showSaveMenu();
            clearScreen();
            continue;
        }

        int row = std::stoi(input);
        int col;
        std::cin >> col;

        if (!board.placePiece(row, col, currentPlayer)) {
            std::cout << RED_FG << "Invalid move!" << RESET << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            clearScreen();
            continue;
        }

        currentPlayer = currentPlayer == u8"🟩" ? u8"⬜" : u8"🟩";
        clearScreen();
    }

    board.print();
    int g = board.count(u8"🟩");
    int w = board.count(u8"⬜");
    
    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "🟩 Green: " << g << "\n";
    std::cout << "⬜ White: " << w << "\n";
    
    if (g > w) std::cout << GREEN_FG << "Green wins!\n" << RESET;
    else if (w > g) std::cout << GREEN_FG << "White wins!\n" << RESET;
    else std::cout << YELLOW_FG << "Draw!\n" << RESET;

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();

    std::ofstream score("scoreboard.txt", std::ios::app);
    if (score.is_open()) {
        if (g > w) score << "Green (2P)\n";
        else if (w > g) score << "White (2P)\n";
        else score << "Draw (2P)\n";
        score.close();
    }
}

void Game::playGame() {
    if (singlePlayer) {
        playSinglePlayer();
    } else {
        playTwoPlayer();
    }
}

void Game::run() {
    clearScreen();
    intro();

    main_menu:
        while (true) {
            int c = menu();
            clearScreen();

        switch (c) {
            case 0:
                chooseGameMode();
                playGame();
                goto main_menu;
                break;

            case 1:
                if (loadGame()) {
                    singlePlayer = false;
                    playGame();
                    goto main_menu;
                }
                break;

            case 2: {
                std::cout << "\nEnter filename to load: ";
                std::string filename;
                std::cin >> filename;
                if (loadFromFile(filename)) {
                    std::cout << GREEN_FG << "Game loaded from " << filename << RESET << "\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    singlePlayer = false;
                    playGame();
                    goto main_menu;
                } else {
                    std::cout << RED_FG << "Failed to load from " << filename << RESET << "\n";
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }
                break;
            }

            case 3: {
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
                    std::cout << "│ Green wins: " << std::setw(4) << gWins << " │\n";
                    std::cout << "│ White wins: " << std::setw(4) << wWins << " │\n";
                    std::cout << "│ Draws:      " << std::setw(4) << draws << " │\n";
                    if (humanWins > 0 || computerWins > 0) {
                        std::cout << "├─────────────────────┤\n";
                        std::cout << "│ Human wins: " << std::setw(4) << humanWins << " │\n";
                        std::cout << "│ Computer wins: " << std::setw(3) << computerWins << " │\n";
                    }
                    std::cout << "└─────────────────────┘\n";

                    score.close();
                }

                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                clearScreen();
                goto main_menu;
                break;
            }

            case 4:
                std::cout << "\nGoodbye! Thanks for playing.\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                clearScreen();
                return;
            default:
                std::cout << "\nInvalid choice! Please try again.\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                clearScreen();
                goto main_menu;
        }
    }
}