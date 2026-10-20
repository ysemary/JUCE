#ifndef WORD_TICTACTOE_H
#define WORD_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

using namespace std;

// Forward declarations
class WordMove;
class WordBoard;
class WordPlayer;
class WordUI;

// Move Class
class WordMove : public Move<char> {
public:
    WordMove(int x, int y, char symbol);
};

// Board Class
class WordBoard : public Board<char> {
private:
    char empty = ' ';
    set<string> dictionary;

    void load_dictionary();

public:
    WordBoard();
    bool update_board(Move<char>* move) override;
    bool is_valid_word(const string& word);
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    bool is_cell_empty(int x, int y);
    void show_sample_words();
    const set<string>& get_dictionary() const { return dictionary; }
};

// Player Class
class WordPlayer : public Player<char> {
public:
    WordPlayer(string name, char symbol, PlayerType type);
};

// UI Class
class WordUI : public UI<char> {
private:
    Move<char>* get_human_move(Player<char>* player);
    Move<char>* get_computer_move(Player<char>* player);

public:
    WordUI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    void display_instructions();
};

// Game runner function
void run_word_tictactoe();

#endif