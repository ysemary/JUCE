#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <random>
#include <queue>
#include "Infinity_Classes.h"

using namespace std;

Infinity_Board::Infinity_Board() : Board(3, 3) {
    // Initialize board with blank symbols
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

Infinity_Board::~Infinity_Board() {
    // Clean up move history
    while (!moveHistory.empty()) {
        delete moveHistory.front();
        moveHistory.pop();
    }
}

bool Infinity_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= 3 || y < 0 || y >= 3) && board[x][y] == blank_symbol) {
        // Store a copy of the move in history
        moveHistory.push(new Move<char>(x, y, mark));

        // Update the board
        board[x][y] = mark;
        n_moves++;

        // Remove oldest move after every 3 moves (starting from move 3)
        if (n_moves >= 3 && n_moves % 3 == 0) {
            remove_oldest_move();
        }
        return true;
    }
    return false;
}

void Infinity_Board::remove_oldest_move() {
    if (!moveHistory.empty()) {
        Move<char>* oldestMove = moveHistory.front();
        moveHistory.pop();

        int x = oldestMove->get_x();
        int y = oldestMove->get_y();

        // Remove the oldest mark from the board
        board[x][y] = blank_symbol;

        delete oldestMove;
    }
}

bool Infinity_Board::is_win(Player<char>* player) {
    return check_three_in_row(player->get_symbol());
}

bool Infinity_Board::is_lose(Player<char>* player) {
    char opponent_symbol = (player->get_symbol() == 'X') ? 'O' : 'X';
    return check_three_in_row(opponent_symbol);
}

bool Infinity_Board::is_draw(Player<char>* player) {
    // In Infinity Tic-Tac-Toe, draws are rare but possible if no one gets 3 in a row
    // and the game continues indefinitely. We'll consider it a draw if no win after a large number of moves
    return n_moves >= 50 && !is_win(player) && !is_lose(player);
}

bool Infinity_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

bool Infinity_Board::check_three_in_row(char symbol) const {
    // Check horizontal
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) {
            return true;
        }
    }

    // Check vertical
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
        return true;
    }
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
        return true;
    }

    return false;
}

void Infinity_Board::display_move_history() const {
    cout << "Move history (oldest to newest): ";
    queue<Move<char>*> temp = moveHistory;
    while (!temp.empty()) {
        Move<char>* move = temp.front();
        cout << "(" << move->get_x() << "," << move->get_y() << ":" << move->get_symbol() << ") ";
        temp.pop();
    }
    cout << endl;
}

Player<char>* Infinity_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Infinity_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
            << "), enter your move (row and column 0-2): ";
        cin >> x >> y;

        while (x < 0 || x > 2 || y < 0 || y > 2) {
            cout << "Invalid move! Please enter row and column between 0 and 2: ";
            cin >> x >> y;
        }
    }
    else {
        // Computer player - random move
        Board<char>* game_board = player->get_board_ptr();
        auto board_matrix = game_board->get_board_matrix();

        vector<pair<int, int>> empty_cells;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == ' ') {
                    empty_cells.push_back({ i, j });
                }
            }
        }

        if (!empty_cells.empty()) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, empty_cells.size() - 1);
            int random_index = dis(gen);
            x = empty_cells[random_index].first;
            y = empty_cells[random_index].second;
        }
        else {
            x = 0; y = 0;
        }

        cout << "\nComputer player " << player->get_name()
            << " chooses position (" << x << ", " << y << ")\n";
    }

    return new Move<char>(x, y, player->get_symbol());
}

void Infinity_UI::display_results(Board<char>* board, Player<char>* player1, Player<char>* player2) {
    cout << "\n=== Final Results ===" << endl;

    if (board->is_win(player1)) {
        cout << player1->get_name() << " wins!" << endl;
    }
    else if (board->is_win(player2)) {
        cout << player2->get_name() << " wins!" << endl;
    }
    else if (board->is_draw(player1)) {
        cout << "It's a draw after many moves!" << endl;
    }
    else {
        cout << "Game ended without a clear result." << endl;
    }

    Infinity_Board* infinity_board = dynamic_cast<Infinity_Board*>(board);
    if (infinity_board) {
        cout << "Total moves made: " << infinity_board->get_move_count() << endl;
    }
}