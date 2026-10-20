#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <random>
#include "Four_in_a_row_Classes.h"

using namespace std;

FourInARow_Board::FourInARow_Board() : Board(6, 7) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool FourInARow_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char mark = move->get_symbol();
    int row = find_lowest_row(col);

    if (row != -1) {
        board[row][col] = mark;
        n_moves++;
        return true;
    }
    return false;
}

bool FourInARow_Board::is_win(Player<char>* player) {
    char player_symbol = player->get_symbol();
    return check_four_in_row(player_symbol);
}

bool FourInARow_Board::is_lose(Player<char>* player) {
    char player_symbol = player->get_symbol();
    char opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';
    return check_four_in_row(opponent_symbol);
}

bool FourInARow_Board::is_draw(Player<char>* player) {
    return is_board_full() && !check_four_in_row('X') && !check_four_in_row('O');
}

bool FourInARow_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

int FourInARow_Board::find_lowest_row(int col) const {
    for (int row = 5; row >= 0; row--) {
        if (board[row][col] == blank_symbol) {
            return row;
        }
    }
    return -1;
}

bool FourInARow_Board::check_four_in_row(char player_symbol) const {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 4; col++) {
            if (board[row][col] == player_symbol &&
                board[row][col + 1] == player_symbol &&
                board[row][col + 2] == player_symbol &&
                board[row][col + 3] == player_symbol) {
                return true;
            }
        }
    }

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 7; col++) {
            if (board[row][col] == player_symbol &&
                board[row + 1][col] == player_symbol &&
                board[row + 2][col] == player_symbol &&
                board[row + 3][col] == player_symbol) {
                return true;
            }
        }
    }

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 4; col++) {
            if (board[row][col] == player_symbol &&
                board[row + 1][col + 1] == player_symbol &&
                board[row + 2][col + 2] == player_symbol &&
                board[row + 3][col + 3] == player_symbol) {
                return true;
            }
        }
    }

    for (int row = 0; row < 3; row++) {
        for (int col = 3; col < 7; col++) {
            if (board[row][col] == player_symbol &&
                board[row + 1][col - 1] == player_symbol &&
                board[row + 2][col - 2] == player_symbol &&
                board[row + 3][col - 3] == player_symbol) {
                return true;
            }
        }
    }

    return false;
}

bool FourInARow_Board::is_board_full() const {
    for (int col = 0; col < 7; col++) {
        if (board[0][col] == blank_symbol) {
            return false;
        }
    }
    return true;
}

Player<char>* FourInARow_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* FourInARow_UI::get_move(Player<char>* player) {
    int col;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
            << "), enter your column (0-6): ";
        cin >> col;

        while (col < 0 || col > 6) {
            cout << "Invalid column! Please enter a column between 0 and 6: ";
            cin >> col;
        }
    }
    else {
        FourInARow_Board* game_board = dynamic_cast<FourInARow_Board*>(player->get_board_ptr());
        vector<int> available_columns;
        for (int c = 0; c < 7; c++) {
            if (game_board->find_lowest_row(c) != -1) {
                available_columns.push_back(c);
            }
        }

        if (!available_columns.empty()) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, available_columns.size() - 1);
            int random_index = dis(gen);
            col = available_columns[random_index];
        }
        else {
            col = 0;
        }

        cout << "\nComputer player " << player->get_name() << " chooses column "
            << col << "\n";
    }

    return new Move<char>(-1, col, player->get_symbol());
}

void FourInARow_UI::display_status(FourInARow_Board* board, Player<char>* player1, Player<char>* player2) {
    cout << "\n=== Game Status ===" << endl;
    cout << player1->get_name() << " (X) vs " << player2->get_name() << " (O)" << endl;
    cout << "Moves played: " << board->get_n_moves() << "/42" << endl;
}

void FourInARow_UI::display_results(FourInARow_Board* board, Player<char>* player1, Player<char>* player2) {
    cout << "\n=== Final Results ===" << endl;

    if (board->is_win(player1)) {
        cout << player1->get_name() << " (X) wins!" << endl;
    }
    else if (board->is_win(player2)) {
        cout << player2->get_name() << " (O) wins!" << endl;
    }
    else {
        cout << "It's a draw!" << endl;
    }

    cout << "Total moves: " << board->get_n_moves() << endl;
}

void FourInARow_UI::display_connect_four_board(const vector<vector<char>>& matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();
    cout << "\n  ";
    for (int j = 0; j < cols; ++j)
        cout << " " << j << "  ";
    cout << "\n";
    cout << "  ";
    for (int j = 0; j < cols; ++j)
        cout << "--- ";
    cout << "\n";
    for (int i = 0; i < rows; ++i) {
        cout << "| ";
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << " | ";
        }
        cout << "\n";
        cout << "  ";
        for (int j = 0; j < cols; ++j)
            cout << "--- ";
        cout << "\n";
    }
    cout << "  ";
    for (int j = 0; j < cols; ++j)
        cout << " " << j << "  ";
    cout << "\n\n";
}