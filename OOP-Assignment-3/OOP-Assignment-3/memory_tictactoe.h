#ifndef MEMORY_TICTACTOE_H
#define MEMORY_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>

using namespace std;

// Forward declarations
class MemoryMove;
class MemoryBoard;
class MemoryPlayer;
class MemoryUI;

// Move Class - tracks both the real symbol and what's displayed
class MemoryMove : public Move<char> {
private:
    char real_symbol;  // The actual symbol (X or O)
    char display_symbol; // What's shown (usually '.' for hidden)

public:
    MemoryMove(int x, int y, char real_sym, char display_sym);
    char get_real_symbol() const;
    char get_display_symbol() const;
};

// Board Class - keeps track of hidden and revealed states
class MemoryBoard : public Board<char> {
private:
    vector<vector<char>> hidden_board;  // Actual board with X/O
    vector<vector<char>> display_board; // What players see
    vector<vector<bool>> revealed;      // Which cells are revealed
    int reveal_row, reveal_col;         // Position to reveal after move

public:
    MemoryBoard();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Helper methods
    bool is_cell_empty(int x, int y) const;
    char get_hidden_cell(int x, int y) const;
    char get_display_cell(int x, int y) const;
    void reveal_cell(int x, int y);
    void hide_all_cells();
    void update_display_board();

    vector<vector<char>> get_display_matrix() const;

    // For cheating/debugging
    void show_hidden_board() const;
};

// Player Class
class MemoryPlayer : public Player<char> {
public:
    MemoryPlayer(string name, char symbol, PlayerType type);
};

// UI Class - No AI
class MemoryUI : public UI<char> {
private:
    Move<char>* get_human_move(Player<char>* player);
    void display_memory_board(const MemoryBoard* board) const;

public:
    MemoryUI();
    void display_instructions() const;
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

// Game runner function
void run_memory_tictactoe();

#endif