#ifndef NUMERICAL_TICTACTOE_H
#define NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

// Forward declarations
class NumericalMove;
class NumericalBoard;
class NumericalPlayer;
class NumericalUI;

// Move Class
class NumericalMove : public Move<int> {
public:
    NumericalMove(int x, int y, int symbol);
};

// Board Class
class NumericalBoard : public Board<int> {
private:
    vector<int> used_odds;
    vector<int> used_evens;
    vector<int> available_odds;
    vector<int> available_evens;

public:
    NumericalBoard();
    bool update_board(Move<int>* move) override;
    bool is_win(Player<int>* player) override;
    bool is_lose(Player<int>* player) override;
    bool is_draw(Player<int>* player) override;
    bool game_is_over(Player<int>* player) override;
    vector<int> get_available_numbers(bool is_odd_player);
    bool is_cell_empty(int x, int y);
};

// Player Class
class NumericalPlayer : public Player<int> {
public:
    NumericalPlayer(string name, int symbol, PlayerType type);
};

// UI Class
class NumericalUI : public UI<int> {
private:
    Move<int>* get_human_move(Player<int>* player);

public:
    NumericalUI();
    Move<int>* get_move(Player<int>* player) override;
    Player<int>* create_player(string& name, int symbol, PlayerType type) override;
    void display_numerical_board(const vector<vector<int>>& matrix) const;
    Player<int>** setup_players() override;
};

// Game runner function
void run_numerical_tictactoe();

#endif