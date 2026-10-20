#pragma once

#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

struct Sequence {
    pair<int, int> pos1;
    pair<int, int> pos2;
    pair<int, int> pos3;
    char player_symbol;
};

class SUS_Board : public Board<char> {
private:
    char blank_symbol = ' ';
    vector<Sequence> player1_sequences;
    vector<Sequence> player2_sequences;

public:
    SUS_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    int get_n_moves() const { return n_moves; }
    int get_player_sequences(char player_symbol) const;
    vector<Sequence> get_player_sequences_list(char player_symbol) const;
    void check_sequences(int x, int y, char symbol);
    bool is_valid_sequence(const vector<pair<int, int>>& seq, char symbol) const;
    bool is_sequence_duplicate(const Sequence& seq, const vector<Sequence>& sequences_list) const;
};

class SUS_UI : public UI<char> {
public:
    SUS_UI() : UI<char>("Welcome to FCAI S-U-S Sequence Game", 3) {}
    ~SUS_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    void display_status(SUS_Board* board, Player<char>* player1, Player<char>* player2);
    void display_results(SUS_Board* board, Player<char>* player1, Player<char>* player2);
};

#endif