/*    #pragma once

#ifndef DIAMOND_TIC_TAC_TOE_CLASSES_H
#define DIAMOND_TIC_TAC_TOE_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <algorithm>
#include <random>
#include <limits>

using namespace std;

class DiamondMove : public Move<char> {
public:
    DiamondMove(int x, int y, char symbol) : Move<char>(x, y, symbol) {}
    ~DiamondMove() {}
};

class DiamondBoard : public Board<char> {
private:
    char blank_symbol = ' ';
    bool is_valid_position(int x, int y) const;
    vector<vector<pair<int, int>>> get_three_lines() const;
    vector<vector<pair<int, int>>> get_four_lines() const;
    int count_in_line(const vector<pair<int, int>>& line, char symbol) const;
    bool has_line(const vector<pair<int, int>>& line, char symbol) const;

public:
    DiamondBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>*) override { return false; };
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    vector<DiamondMove> get_valid_moves(char symbol) const;
    DiamondBoard make_move(const DiamondMove& move) const;
    int evaluate(char player_symbol) const;
    char get_opponent(char symbol) const;
    bool is_corner(int x, int y) const;
    bool is_edge(int x, int y) const;
    bool is_center(int x, int y) const;
};

class AIPlayer : public Player<char> {
private:
    int depth;
    int minimax(DiamondBoard& board, int depth, bool maximizing, int alpha, int beta, char player_symbol);

public:
    AIPlayer(string name, char symbol, int search_depth = 4);
    DiamondMove get_best_move();
    ~AIPlayer() {}
};

class RandomPlayer : public Player<char> {
private:
    mt19937 rng;

public:
    RandomPlayer(string name, char symbol);
    DiamondMove get_random_move();
    ~RandomPlayer() {}
};

class DiamondUI : public UI<char> {
public:
    DiamondUI();
    ~DiamondUI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    virtual Move<char>* get_move(Player<char>* player) override;
    virtual Player<char>** setup_players() override;
    void display_board_matrix(const vector<vector<char>>& matrix) const;

private:
    void display_diamond_board(const vector<vector<char>>& matrix) const;
    PlayerType get_player_type_choice_extended(string player_label);
};

#endif*/

#ifndef DIAMOND_TIC_TAC_TOE_CLASSES_H
#define DIAMOND_TIC_TAC_TOE_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

// Enum to track line direction for the specific rule "different directions"
enum class LineDirection {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL
};

class DiamondMove : public Move<char> {
public:
    DiamondMove(int x, int y, char symbol) : Move<char>(x, y, symbol) {}
    ~DiamondMove() {}
};

class DiamondBoard : public Board<char> {
private:
    char blank_symbol = ' ';

    // Check Manhattan distance for diamond shape |x-2| + |y-2| <= 2
    bool is_valid_position(int x, int y) const;

    // Helper to get lines with their directions
    vector<pair<vector<pair<int, int>>, LineDirection>> get_lines_of_size(int size) const;

    // Check if a specific set of coordinates has the player's symbol
    bool has_line(const vector<pair<int, int>>& line, char symbol) const;

public:
    DiamondBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>*) override { return false; };
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    vector<DiamondMove> get_valid_moves(char symbol) const;
    DiamondBoard make_move(const DiamondMove& move) const;

    int evaluate(char player_symbol) const;
    char get_opponent(char symbol) const;
};

class AIPlayer : public Player<char> {
private:
    int depth;
    int minimax(DiamondBoard& board, int depth, bool maximizing, int alpha, int beta, char player_symbol);

public:
    AIPlayer(string name, char symbol, int search_depth = 4);
    DiamondMove get_best_move();
    ~AIPlayer() {}
};

class RandomPlayer : public Player<char> {
private:
    // No specific member variables needed for random logic beyond std rand
public:
    RandomPlayer(string name, char symbol);
    DiamondMove get_random_move();
    ~RandomPlayer() {}
};

class DiamondUI : public UI<char> {
public:
    DiamondUI();
    ~DiamondUI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    virtual Move<char>* get_move(Player<char>* player) override;
    virtual Player<char>** setup_players() override;
    void display_board_matrix(const vector<vector<char>>& matrix) const;

private:
    PlayerType get_player_type_choice_extended(string player_label);
};

#endif