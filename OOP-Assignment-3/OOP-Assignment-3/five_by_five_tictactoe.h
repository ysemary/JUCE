#ifndef FIVE_BY_FIVE_TICTACTOE_H
#define FIVE_BY_FIVE_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

// Forward declarations
class FiveByFiveMove;
class FiveByFiveBoard;
class FiveByFivePlayer;
class FiveByFiveUI;

// Move Class
class FiveByFiveMove : public Move<char> {
public:
    FiveByFiveMove(int x, int y, char symbol);
};

// Board Class
class FiveByFiveBoard : public Board<char> {
private:
    char empty = '.';

public:
    FiveByFiveBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    bool is_cell_empty(int x, int y);
    int count_sequences(char symbol);
};

// Player Class
class FiveByFivePlayer : public Player<char> {
public:
    FiveByFivePlayer(string name, char symbol, PlayerType type);
};

// UI Class
class FiveByFiveUI : public UI<char> {
private:
    Move<char>* get_human_move(Player<char>* player);
    Move<char>* get_computer_move(Player<char>* player);

public:
    FiveByFiveUI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    void display_five_by_five_board(const vector<vector<char>>& matrix) const;
};

// Game runner function
void run_five_by_five_tictactoe();

#endif