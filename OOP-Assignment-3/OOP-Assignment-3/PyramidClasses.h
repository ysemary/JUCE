/*#ifndef PYRAMID_CLASSES_H
#define PYRAMID_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

class PyramidBoard : public Board<char> {
private:
    static const int ROWS = 5;
    static const int COLS = 5;

    vector<vector<bool>> valid_cells;

    void initialize_valid_cells();
    bool is_valid_position(int x, int y) const;
    bool check_three_in_line(char symbol) const;
    bool check_horizontal_win(char symbol) const;
    bool check_vertical_win(char symbol) const;
    bool check_diagonal_win(char symbol) const;

    int evaluate_position(int x, int y) const;

    // Minimax/Backtracking functions
    int minimax(int depth, bool isMaximizing, char symbol, int alpha, int beta, int maxDepth);
    int evaluate_board(char symbol) const;

public:
    PyramidBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    vector<pair<int, int>> get_empty_positions() const;

    // Helper methods
    pair<int, int> get_best_move(char symbol) const;
    PyramidBoard* clone() const;
};

class PyramidUI : public UI<char> {
public:
    PyramidUI() : UI<char>("Welcome to Pyramid Tic-Tac-Toe", 3) {}
    ~PyramidUI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;

    // Add this new method
    Player<char>** setup_players_with_ai();

    // Special display for pyramid board
    void display_board_matrix(const vector<vector<char>>& matrix) const;

private:
    bool is_valid_pyramid_position(int x, int y) const;
};

#endif*/

#ifndef PYRAMID_CLASSES_H
#define PYRAMID_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

class PyramidBoard : public Board<char> {
private:
    static const int ROWS = 5; // Technically only use 0, 1, 2
    static const int COLS = 5;

    // Helper to check geometry
    bool is_valid_position(int x, int y) const;

    // Helper to check lines
    bool check_horizontal_win(char symbol) const;
    bool check_vertical_win(char symbol) const;
    bool check_diagonal_win(char symbol) const;

public:
    PyramidBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    vector<pair<int, int>> get_empty_positions() const;

    // For AI
    PyramidBoard* clone() const;
    int evaluate_board(char symbol) const;
    int minimax(int depth, bool isMaximizing, char symbol, int alpha, int beta);
    pair<int, int> get_best_move(char symbol) const;
};

class PyramidUI : public UI<char> {
public:
    PyramidUI() : UI<char>("Welcome to Pyramid Tic-Tac-Toe", 3) {}
    ~PyramidUI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    Player<char>** setup_players_with_ai();

    void display_board_matrix(const vector<vector<char>>& matrix) const;

private:
    bool is_valid_pyramid_position(int x, int y) const;
};

#endif