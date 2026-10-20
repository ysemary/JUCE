#include "memory_tictactoe.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

// MemoryMove Implementation
MemoryMove::MemoryMove(int x, int y, char real_sym, char display_sym)
    : Move<char>(x, y, display_sym), real_symbol(real_sym), display_symbol(display_sym) {}

char MemoryMove::get_real_symbol() const { return real_symbol; }
char MemoryMove::get_display_symbol() const { return display_symbol; }

// MemoryBoard Implementation
MemoryBoard::MemoryBoard() : Board<char>(3, 3), reveal_row(-1), reveal_col(-1) {
    // Initialize all boards
    hidden_board = vector<vector<char>>(3, vector<char>(3, ' '));
    display_board = vector<vector<char>>(3, vector<char>(3, '.'));
    revealed = vector<vector<bool>>(3, vector<bool>(3, false));

    // Initialize parent board for display
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = '.';  // Show dots initially
        }
    }
}

bool MemoryBoard::update_board(Move<char>* move) {
    MemoryMove* memoryMove = dynamic_cast<MemoryMove*>(move);
    if (!memoryMove) return false;

    int x = memoryMove->get_x();
    int y = memoryMove->get_y();
    char real_symbol = memoryMove->get_real_symbol();

    // Check if position is valid
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        cout << "Invalid position! Use 0-2 for rows and columns.\n";
        return false;
    }

    // Check if cell is empty
    if (hidden_board[x][y] != ' ') {
        cout << "Cell already occupied! Choose another.\n";
        return false;
    }

    // Update hidden board with actual symbol
    hidden_board[x][y] = real_symbol;

    // Temporarily reveal this move
    display_board[x][y] = real_symbol;
    board[x][y] = real_symbol;  // Update parent board

    // Store position to hide after delay
    reveal_row = x;
    reveal_col = y;

    n_moves++;
    return true;
}

bool MemoryBoard::is_win(Player<char>* player) {
    char symbol = player->get_symbol();

    // Check rows on HIDDEN board
    for (int i = 0; i < 3; ++i) {
        if (hidden_board[i][0] == symbol &&
            hidden_board[i][1] == symbol &&
            hidden_board[i][2] == symbol) {
            return true;
        }
    }

    // Check columns on HIDDEN board
    for (int j = 0; j < 3; ++j) {
        if (hidden_board[0][j] == symbol &&
            hidden_board[1][j] == symbol &&
            hidden_board[2][j] == symbol) {
            return true;
        }
    }

    // Check diagonals on HIDDEN board
    if (hidden_board[0][0] == symbol &&
        hidden_board[1][1] == symbol &&
        hidden_board[2][2] == symbol) {
        return true;
    }
    if (hidden_board[0][2] == symbol &&
        hidden_board[1][1] == symbol &&
        hidden_board[2][0] == symbol) {
        return true;
    }

    return false;
}

bool MemoryBoard::is_lose(Player<char>* player) {
    return false;
}

bool MemoryBoard::is_draw(Player<char>* player) {
    if (n_moves == 9 && !is_win(player)) {
        return true;
    }
    return false;
}

bool MemoryBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

bool MemoryBoard::is_cell_empty(int x, int y) const {
    if (x < 0 || x >= 3 || y < 0 || y >= 3) return false;
    return hidden_board[x][y] == ' ';
}

char MemoryBoard::get_hidden_cell(int x, int y) const {
    if (x < 0 || x >= 3 || y < 0 || y >= 3) return ' ';
    return hidden_board[x][y];
}

char MemoryBoard::get_display_cell(int x, int y) const {
    if (x < 0 || x >= 3 || y < 0 || y >= 3) return '.';
    return display_board[x][y];
}

void MemoryBoard::reveal_cell(int x, int y) {
    if (x < 0 || x >= 3 || y < 0 || y >= 3) return;
    revealed[x][y] = true;
    display_board[x][y] = hidden_board[x][y];
}

void MemoryBoard::hide_all_cells() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!revealed[i][j]) {
                display_board[i][j] = '.';
            }
        }
    }
}

void MemoryBoard::update_display_board() {
    // Hide the last revealed cell
    if (reveal_row != -1 && reveal_col != -1) {
        display_board[reveal_row][reveal_col] = '.';
        board[reveal_row][reveal_col] = '.';  // Update parent board
        reveal_row = -1;
        reveal_col = -1;
    }

    // Update parent board with current display
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = display_board[i][j];
        }
    }
}

vector<vector<char>> MemoryBoard::get_display_matrix() const {
    return display_board;
}

void MemoryBoard::show_hidden_board() const {
    cout << "\n[DEBUG] Hidden Board:\n";
    for (int i = 0; i < 3; ++i) {
        cout << " ";
        for (int j = 0; j < 3; ++j) {
            cout << (hidden_board[i][j] == ' ' ? '.' : hidden_board[i][j]) << " ";
        }
        cout << endl;
    }
}

// MemoryPlayer Implementation
MemoryPlayer::MemoryPlayer(string name, char symbol, PlayerType type)
    : Player<char>(name, symbol, type) {}

// MemoryUI Implementation
MemoryUI::MemoryUI() : UI<char>("Memory Tic-Tac-Toe - Test Your Memory!", 3) {}

void MemoryUI::display_instructions() const {
    cout << "\n=== MEMORY TIC-TAC-TOE ===\n";
    cout << "Rules:\n";
    cout << "1. Place your mark (X or O) on the board\n";
    cout << "2. After placing, the mark will be HIDDEN!\n";
    cout << "3. You must remember where all marks are\n";
    cout << "4. First to get 3 in a row wins!\n";
    cout << "5. If board fills with no winner, it's a draw\n";
    cout << "===========================\n\n";
}

void MemoryUI::display_memory_board(const MemoryBoard* board) const {
    vector<vector<char>> matrix = board->get_display_matrix();

    cout << "\n    ";
    for (int j = 0; j < 3; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * 3, '-') << "\n";

    for (int i = 0; i < 3; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < 3; ++j) {
            cout << setw(cell_width) << matrix[i][j] << " |";
        }
        cout << "\n   " << string((cell_width + 2) * 3, '-') << "\n";
    }
    cout << endl;
}

Move<char>* MemoryUI::get_move(Player<char>* player) {
    // Only human players for memory game
    return get_human_move(player);
}

Player<char>* MemoryUI::create_player(string& name, char symbol, PlayerType type) {
    return new MemoryPlayer(name, symbol, type);
}

Move<char>* MemoryUI::get_human_move(Player<char>* player) {
    MemoryBoard* boardPtr = dynamic_cast<MemoryBoard*>(player->get_board_ptr());

    cout << player->get_name() << "'s turn (" << player->get_symbol() << ").\n";

    // Display current board (with hidden marks)
    display_memory_board(boardPtr);

    int x, y;

    while (true) {
        cout << "Enter row (0-2) and column (0-2): ";
        cin >> x >> y;

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

    // Create move with real symbol but display as dot
    return new MemoryMove(x, y, player->get_symbol(), '.');
}

// Game runner implementation
void run_memory_tictactoe() {
    MemoryBoard* board = new MemoryBoard();
    MemoryUI* ui = new MemoryUI();

    ui->display_instructions();

    Player<char>** players = ui->setup_players();

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    // Show initial empty board
    ui->display_board_matrix(board->get_board_matrix());

    GameManager<char> gameManager(board, players, ui);

    // Custom game loop to handle memory aspect
    cout << "\n=== GAME START ===\n";

    int currentPlayer = 0;
    bool gameEnded = false;

    while (!gameEnded) {
        Player<char>* player = players[currentPlayer];

        // Get move
        Move<char>* move = ui->get_move(player);

        // Apply move (temporarily reveals it)
        while (!board->update_board(move)) {
            delete move;
            move = ui->get_move(player);
        }

        // Show the move briefly
        cout << "\n" << player->get_name() << " placed at ("
            << move->get_x() << ", " << move->get_y() << ")\n";

        ui->display_board_matrix(board->get_board_matrix());

        cout << "\nRemember this position! Hiding in 3 seconds...\n";
        sleep_for(seconds(3));

        // Hide the move
        board->update_display_board();
        cout << "\n\n\n\n\n\n\n\n\n\n";  // Clear screen effect
        cout << "=== Board Hidden ===\n";
        ui->display_board_matrix(board->get_board_matrix());

        // Check game status
        if (board->is_win(player)) {
            cout << "\n" << player->get_name() << " wins!\n";
            // Reveal full board at the end
            cout << "\nFinal board (revealed):\n";
            board->show_hidden_board();
            gameEnded = true;
        }
        else if (board->is_draw(player)) {
            cout << "\nIt's a draw!\n";
            cout << "\nFinal board (revealed):\n";
            board->show_hidden_board();
            gameEnded = true;
        }

        delete move;

        // Switch player
        currentPlayer = 1 - currentPlayer;
    }

    // Cleanup
    delete players[0];
    delete players[1];
    delete[] players;
    delete ui;
    delete board;
}