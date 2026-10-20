#ifndef FOURBYFOUR_CLASSES_H
#define FOURBYFOUR_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

class FourByFourBoard : public Board<char> {
private:
    static const int BOARD_SIZE = 4;
    vector<pair<int, int>> starting_positions_X;
    vector<pair<int, int>> starting_positions_O;

    void initialize_starting_positions();
    bool make_move(pair<int, int> from, pair<int, int> to, char symbol);
    bool check_three_in_line(char symbol) const;

    // Minimax/Backtracking functions
    int minimax(int depth, bool isMaximizing, char symbol, int alpha, int beta, int maxDepth);
    int evaluate_board(char symbol) const;

public:
    FourByFourBoard();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Helper methods
    vector<pair<pair<int, int>, pair<int, int>>> get_all_valid_moves(char symbol) const;
    pair<pair<int, int>, pair<int, int>> get_best_move(char symbol) const;
    FourByFourBoard* clone() const;
};

class FourByFourUI : public UI<char> {
public:
    FourByFourUI() : UI<char>("Welcome to 4x4 Tic-Tac-Toe", 3) {}
    ~FourByFourUI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;

    Player<char>** setup_players_with_ai();

    void display_board_matrix(const vector<vector<char>>& matrix) const;
};

#endif