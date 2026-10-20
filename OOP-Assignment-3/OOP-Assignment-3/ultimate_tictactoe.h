#ifndef ULTIMATE_TICTACTOE_H
#define ULTIMATE_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

// Forward declarations
class UltimateMove;
class UltimateBoard;
class UltimatePlayer;
class UltimateUI;

// Move Class
class UltimateMove : public Move<char> {
private:
    int mainRow;
    int mainCol;
    int subRow;
    int subCol;

public:
    UltimateMove(int mainRow, int mainCol, int subRow, int subCol, char symbol);

    int get_main_row() const;
    int get_main_col() const;
    int get_sub_row() const;
    int get_sub_col() const;
};

// Sub-board class
class SubBoard : public Board<char> {
private:
    char winner;

public:
    SubBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    char get_winner() const;
    void set_winner(char w);
    bool is_full() const;
};

// Main Ultimate Board
class UltimateBoard : public Board<char> {
private:
    vector<vector<SubBoard*>> subBoards;
    vector<vector<char>> mainBoard;
    int nextMainRow;
    int nextMainCol;

public:
    UltimateBoard();
    ~UltimateBoard();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    bool is_valid_move(int mainRow, int mainCol, int subRow, int subCol);
    void set_next_board(int row, int col);
    SubBoard* get_sub_board(int row, int col);
    char get_main_cell(int row, int col);
    bool is_sub_board_active(int row, int col) const;
    void display_full_board() const;
};

// Player Class
class UltimatePlayer : public Player<char> {
public:
    UltimatePlayer(string name, char symbol, PlayerType type);
};

// UI Class - No AI
class UltimateUI : public UI<char> {
private:
    Move<char>* get_human_move(Player<char>* player);
    void display_current_status(UltimateBoard* board, Player<char>* player);

public:
    UltimateUI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

void run_ultimate_tictactoe();

#endif