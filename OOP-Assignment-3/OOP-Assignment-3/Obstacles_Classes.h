#pragma once

#ifndef OBSTACLES_CLASSES_H
#define OBSTACLES_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>
#include <random>

using namespace std;

class Obstacles_Board : public Board<char> {
private:
    char blank_symbol = ' ';
    char obstacle_symbol = '#';
    vector<pair<int, int>> obstacles;
    bool is_valid_position(int x, int y) const;
    bool check_four_in_row(char symbol) const;

public:
    Obstacles_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Obstacle-specific methods
    void add_random_obstacles(int count = 2);
    bool is_obstacle(int x, int y) const;
    vector<pair<int, int>> get_obstacles() const { return obstacles; }
};

class Obstacles_UI : public UI<char> {
public:
    Obstacles_UI() : UI<char>("Welcome to FCAI Obstacles Tic-Tac-Toe", 3) {}
    ~Obstacles_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    void display_obstacles_board(const vector<vector<char>>& matrix,
        const vector<pair<int, int>>& obstacles) const;
    void display_results(Board<char>* board, Player<char>* player1, Player<char>* player2);
};

#endif