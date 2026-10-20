#pragma once

#ifndef MISERETICTACTOE_CLASSES_H
#define MISERETICTACTOE_CLASSES_H

#include "BoardGame_Classes.h"
#include <utility>

using namespace std;

class MisereTicTacToe_Board : public Board<char> {
private:
    char blank_symbol = ' ';

public:
    MisereTicTacToe_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>* player);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    bool has_three_in_row(char symbol);
};

class MisereTicTacToe_UI : public UI<char> {
public:
    MisereTicTacToe_UI();
    ~MisereTicTacToe_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
    virtual void display_game_state(Move<char>* move, Board<char>* board,
        Player<char>* current_player, Player<char>* next_player);

private:
    pair<int, int> getBestMove(const vector<vector<char>>& board, char aiSymbol, char opponentSymbol);
    bool createsThreeInRow(const vector<vector<char>>& board, int x, int y, char symbol);
    bool forcesOpponentThree(const vector<vector<char>>& board, int x, int y, char aiSymbol, char opponentSymbol);
    bool isCenter(int x, int y);
    bool isCorner(int x, int y);
};

#endif