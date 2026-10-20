#pragma once

#ifndef INFINITY_CLASSES_H
#define INFINITY_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>
#include <queue>

using namespace std;

class Infinity_Board : public Board<char> {
private:
    char blank_symbol = ' ';
    queue<Move<char>*> moveHistory;  // Store moves in chronological order

    bool check_three_in_row(char symbol) const;
    void remove_oldest_move();

public:
    Infinity_Board();
    ~Infinity_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Infinity-specific methods
    int get_move_count() const { return n_moves; }
    void display_move_history() const;
};

class Infinity_UI : public UI<char> {
public:
    Infinity_UI() : UI<char>("Welcome to FCAI Infinity Tic-Tac-Toe", 3) {}
    ~Infinity_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    void display_results(Board<char>* board, Player<char>* player1, Player<char>* player2);
};

#endif