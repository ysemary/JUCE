#include "Numerical Tic-Tac-Toe.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Move Implementation
NumericalMove::NumericalMove(int x, int y, int symbol) : Move<int>(x, y, symbol) {}

// Board Implementation
NumericalBoard::NumericalBoard() : Board<int>(3, 3),
available_odds({ 1, 3, 5, 7, 9 }),
available_evens({ 2, 4, 6, 8 }) {

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = 0;
        }
    }
}

bool NumericalBoard::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int value = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        cout << "Invalid position! Use 0-2 for rows and columns.\n";
        return false;
    }

    if (board[x][y] != 0) {
        cout << "Cell already occupied! Choose another.\n";
        return false;
    }

    bool is_odd = (value % 2 == 1);

    if (n_moves % 2 == 0) { // Player 1's turn (odd numbers)
        if (!is_odd) {
            cout << "Player 1 must use odd numbers!\n";
            return false;
        }
        auto it = find(available_odds.begin(), available_odds.end(), value);
        if (it == available_odds.end()) {
            cout << "Number already used or invalid! Available odds: ";
            for (int num : available_odds) cout << num << " ";
            cout << endl;
            return false;
        }

        available_odds.erase(it);
        used_odds.push_back(value);
    }
    else { // Player 2's turn (even numbers)
        if (is_odd) {
            cout << "Player 2 must use even numbers!\n";
            return false;
        }
        auto it = find(available_evens.begin(), available_evens.end(), value);
        if (it == available_evens.end()) {
            cout << "Number already used or invalid! Available evens: ";
            for (int num : available_evens) cout << num << " ";
            cout << endl;
            return false;
        }

        available_evens.erase(it);
        used_evens.push_back(value);
    }

    board[x][y] = value;
    n_moves++;
    return true;
}

bool NumericalBoard::is_win(Player<int>* player) {
    int target_sum = 15;

    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] + board[i][1] + board[i][2] == target_sum &&
            board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] + board[1][j] + board[2][j] == target_sum &&
            board[0][j] != 0 && board[1][j] != 0 && board[2][j] != 0) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] + board[1][1] + board[2][2] == target_sum &&
        board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0) {
        return true;
    }
    if (board[0][2] + board[1][1] + board[2][0] == target_sum &&
        board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0) {
        return true;
    }

    return false;
}

bool NumericalBoard::is_lose(Player<int>* player) {
    return false;
}

bool NumericalBoard::is_draw(Player<int>* player) {
    if (n_moves == 9 && !is_win(player)) {
        return true;
    }
    return false;
}

bool NumericalBoard::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

vector<int> NumericalBoard::get_available_numbers(bool is_odd_player) {
    return is_odd_player ? available_odds : available_evens;
}

bool NumericalBoard::is_cell_empty(int x, int y) {
    return (x >= 0 && x < 3 && y >= 0 && y < 3) && (board[x][y] == 0);
}

// Player Implementation
NumericalPlayer::NumericalPlayer(string name, int symbol, PlayerType type)
    : Player<int>(name, symbol, type) {}

// UI Implementation
NumericalUI::NumericalUI() : UI<int>("Numerical Tic-Tac-Toe - Human vs Human (sum to 15)", 4) {}

Move<int>* NumericalUI::get_move(Player<int>* player) {
    return get_human_move(player); // AI removed for this mode
}

Player<int>* NumericalUI::create_player(string& name, int symbol, PlayerType type) {
    return new NumericalPlayer(name, symbol, type);
}

Player<int>** NumericalUI::setup_players() {
    Player<int>** players = new Player<int>*[2];

    string nameX = get_player_name("Player X");
    string nameO = get_player_name("Player O");

    players[0] = create_player(nameX, static_cast<int>('X'), PlayerType::HUMAN);
    players[1] = create_player(nameO, static_cast<int>('O'), PlayerType::HUMAN);
    return players;
}

void NumericalUI::display_numerical_board(const vector<vector<int>>& matrix) const {
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
            if (matrix[i][j] == 0) {
                cout << setw(cell_width) << "." << " |";
            }
            else {
                cout << setw(cell_width) << matrix[i][j] << " |";
            }
        }
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    cout << endl;
}

Move<int>* NumericalUI::get_human_move(Player<int>* player) {
    int x, y, value;
    NumericalBoard* boardPtr = dynamic_cast<NumericalBoard*>(player->get_board_ptr());

    cout << player->get_name() << "'s turn (" << player->get_symbol() << ").\n";

    bool is_odd_player = (player->get_symbol() == 'X');
    auto available_nums = boardPtr->get_available_numbers(is_odd_player);

    cout << "Available numbers: ";
    for (int num : available_nums) {
        cout << num << " ";
    }
    cout << "\n";

    while (true) {
        cout << "Enter row (0-2), column (0-2), and number: ";
        cin >> x >> y >> value;

        if (x >= 0 && x < 3 && y >= 0 && y < 3) {
            if (boardPtr->is_cell_empty(x, y)) {
                break;
            }
            else {
                cout << "Cell already occupied! Choose another position.\n";
            }
        }
        else {
            cout << "Invalid position! Please enter row and column between 0-2.\n";
        }
    }

    return new NumericalMove(x, y, value);
}

// Game runner implementation
void run_numerical_tictactoe() {
    NumericalBoard* board = new NumericalBoard();
    NumericalUI* ui = new NumericalUI();

    Player<int>** players = ui->setup_players();

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    ui->display_numerical_board(board->get_board_matrix());

    GameManager<int> gameManager(board, players, ui);
    gameManager.run();

    delete players[0];
    delete players[1];
    delete[] players;
    delete ui;
    delete board;
}