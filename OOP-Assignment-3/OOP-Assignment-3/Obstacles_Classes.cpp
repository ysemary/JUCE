#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <random>
#include "Obstacles_Classes.h"

using namespace std;

Obstacles_Board::Obstacles_Board() : Board(6, 6) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool Obstacles_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (is_valid_position(x, y) && board[x][y] == blank_symbol && !is_obstacle(x, y)) {
        board[x][y] = mark;
        n_moves++;

        // Add obstacles after every round (both players have moved)
        if (n_moves % 2 == 0) {
            add_random_obstacles(2);
        }
        return true;
    }
    return false;
}

bool Obstacles_Board::is_win(Player<char>* player) {
    return check_four_in_row(player->get_symbol());
}

bool Obstacles_Board::is_lose(Player<char>* player) {
    char opponent_symbol = (player->get_symbol() == 'X') ? 'O' : 'X';
    return check_four_in_row(opponent_symbol);
}

bool Obstacles_Board::is_draw(Player<char>* player) {
    // Check if board is full (excluding obstacles)
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] == blank_symbol && !is_obstacle(i, j)) {
                return false;
            }
        }
    }
    return !is_win(player) && !is_lose(player);
}

bool Obstacles_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

bool Obstacles_Board::is_valid_position(int x, int y) const {
    return (x >= 0 && x < 6 && y >= 0 && y < 6);
}

bool Obstacles_Board::is_obstacle(int x, int y) const {
    for (const auto& obstacle : obstacles) {
        if (obstacle.first == x && obstacle.second == y) {
            return true;
        }
    }
    return false;
}

void Obstacles_Board::add_random_obstacles(int count) {
    vector<pair<int, int>> available_positions;

    // Find all available positions (blank and not already obstacles)
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] == blank_symbol && !is_obstacle(i, j)) {
                available_positions.push_back({ i, j });
            }
        }
    }

    // If not enough positions, return
    if (available_positions.size() < count) {
        return;
    }

    // Randomly select positions for obstacles
    random_device rd;
    mt19937 gen(rd());
    shuffle(available_positions.begin(), available_positions.end(), gen);

    for (int i = 0; i < count && i < available_positions.size(); i++) {
        obstacles.push_back(available_positions[i]);
    }
}

bool Obstacles_Board::check_four_in_row(char symbol) const {
    // Check horizontal
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol &&
                board[i][j + 1] == symbol &&
                board[i][j + 2] == symbol &&
                board[i][j + 3] == symbol) {
                return true;
            }
        }
    }

    // Check vertical
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] == symbol &&
                board[i + 1][j] == symbol &&
                board[i + 2][j] == symbol &&
                board[i + 3][j] == symbol) {
                return true;
            }
        }
    }

    // Check diagonal (top-left to bottom-right)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == symbol &&
                board[i + 1][j + 1] == symbol &&
                board[i + 2][j + 2] == symbol &&
                board[i + 3][j + 3] == symbol) {
                return true;
            }
        }
    }

    // Check diagonal (top-right to bottom-left)
    for (int i = 0; i < 3; i++) {
        for (int j = 3; j < 6; j++) {
            if (board[i][j] == symbol &&
                board[i + 1][j - 1] == symbol &&
                board[i + 2][j - 2] == symbol &&
                board[i + 3][j - 3] == symbol) {
                return true;
            }
        }
    }

    return false;
}

Player<char>* Obstacles_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Obstacles_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
            << "), enter your move (row and column 0-5): ";
        cin >> x >> y;

        while (x < 0 || x > 5 || y < 0 || y > 5) {
            cout << "Invalid move! Please enter row and column between 0 and 5: ";
            cin >> x >> y;
        }
    }
    else {
        // Computer player - random move
        Board<char>* game_board = player->get_board_ptr();
        auto board_matrix = game_board->get_board_matrix();
        Obstacles_Board* obstacles_board = dynamic_cast<Obstacles_Board*>(game_board);

        vector<pair<int, int>> empty_cells;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                if (board_matrix[i][j] == ' ' &&
                    (obstacles_board && !obstacles_board->is_obstacle(i, j))) {
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

void Obstacles_UI::display_obstacles_board(const vector<vector<char>>& matrix,
    const vector<pair<int, int>>& obstacles) const {
    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();

    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

    for (int i = 0; i < rows; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < cols; ++j) {
            // Check if this position is an obstacle
            bool is_obstacle = false;
            for (const auto& obstacle : obstacles) {
                if (obstacle.first == i && obstacle.second == j) {
                    is_obstacle = true;
                    break;
                }
            }

            if (is_obstacle) {
                cout << setw(cell_width) << "#" << " |";
            }
            else {
                cout << setw(cell_width) << matrix[i][j] << " |";
            }
        }
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    cout << endl;
}

void Obstacles_UI::display_results(Board<char>* board, Player<char>* player1, Player<char>* player2) {
    cout << "\n=== Final Results ===" << endl;

    if (board->is_win(player1)) {
        cout << player1->get_name() << " wins!" << endl;
    }
    else if (board->is_win(player2)) {
        cout << player2->get_name() << " wins!" << endl;
    }
    else if (board->is_draw(player1)) {
        cout << "It's a draw!" << endl;
    }
    else {
        cout << "Game ended without a clear result." << endl;
    }
}