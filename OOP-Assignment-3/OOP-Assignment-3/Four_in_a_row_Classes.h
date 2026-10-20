#pragma once
#pragma once

#ifndef FOUR_IN_A_ROW_CLASSES_H
#define FOUR_IN_A_ROW_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

class FourInARow_Board : public Board<char> {
private:
    char blank_symbol = ' ';
    char player1_symbol = 'X';
    char player2_symbol = 'O';
    vector<pair<int, int>> winning_positions;

public:
    FourInARow_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    int get_n_moves() const { return n_moves; }
    int find_lowest_row(int col) const;
    bool check_four_in_row(char player_symbol) const;
    vector<pair<int, int>> get_winning_positions() const { return winning_positions; }
    bool is_board_full() const;
}; 

class FourInARow_UI : public UI<char> {
public:
    FourInARow_UI() : UI<char>("Welcome to FCAI Four-in-a-Row Game", 3) {}
    ~FourInARow_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    void display_status(FourInARow_Board* board, Player<char>* player1, Player<char>* player2);
    void display_results(FourInARow_Board* board, Player<char>* player1, Player<char>* player2);
    void display_connect_four_board(const vector<vector<char>>& matrix) const;
};

#endif