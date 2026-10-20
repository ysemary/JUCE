#include "FourByFourClasses.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

// FourByFourBoard Implementation
FourByFourBoard::FourByFourBoard() : Board(4, 4) {
    initialize_starting_positions();

    // Initialize board with starting positions
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = ' ';
        }
    }

    // Place starting tokens
    for (const auto& pos : starting_positions_X) {
        board[pos.first][pos.second] = 'X';
    }
    for (const auto& pos : starting_positions_O) {
        board[pos.first][pos.second] = 'O';
    }
}

void FourByFourBoard::initialize_starting_positions() {
    starting_positions_X = { {0, 1}, {0, 3}, {3, 0}, {3, 2} };
    starting_positions_O = { {0, 0}, {0, 2}, {3, 1}, {3, 3} };
}

bool FourByFourBoard::update_board(Move<char>* move) {
    int to_x = move->get_x();
    int to_y = move->get_y();
    char symbol = move->get_symbol();

    if (to_x >= 0 && to_x < 4 && to_y >= 0 && to_y < 4) {
        if (board[to_x][to_y] == ' ') {
            // Find a token of the player that can move to this position
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (board[i][j] == symbol) {
                        // Check if it's adjacent (horizontal or vertical)
                        if ((abs(i - to_x) == 1 && j == to_y) || (abs(j - to_y) == 1 && i == to_x)) {
                            board[i][j] = ' ';
                            board[to_x][to_y] = symbol;
                            n_moves++;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

vector<pair<pair<int, int>, pair<int, int>>> FourByFourBoard::get_all_valid_moves(char symbol) const {
    vector<pair<pair<int, int>, pair<int, int>>> moves;

    // For each of player's tokens, find all adjacent empty cells
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == symbol) {
                // Check all four directions
                if (i > 0 && board[i - 1][j] == ' ')
                    moves.push_back({ {i, j}, {i - 1, j} });
                if (i < 3 && board[i + 1][j] == ' ')
                    moves.push_back({ {i, j}, {i + 1, j} });
                if (j > 0 && board[i][j - 1] == ' ')
                    moves.push_back({ {i, j}, {i, j - 1} });
                if (j < 3 && board[i][j + 1] == ' ')
                    moves.push_back({ {i, j}, {i, j + 1} });
            }
        }
    }
    return moves;
}

bool FourByFourBoard::make_move(pair<int, int> from, pair<int, int> to, char symbol) {
    if (from.first < 0 || from.first >= 4 || from.second < 0 || from.second >= 4) return false;
    if (to.first < 0 || to.first >= 4 || to.second < 0 || to.second >= 4) return false;

    if (board[from.first][from.second] == symbol && board[to.first][to.second] == ' ') {
        if ((abs(from.first - to.first) == 1 && from.second == to.second) ||
            (abs(from.second - to.second) == 1 && from.first == to.first)) {
            board[from.first][from.second] = ' ';
            board[to.first][to.second] = symbol;
            return true;
        }
    }
    return false;
}

FourByFourBoard* FourByFourBoard::clone() const {
    FourByFourBoard* new_board = new FourByFourBoard();

    // Copy the board state
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            new_board->board[i][j] = board[i][j];
        }
    }
    new_board->n_moves = n_moves;
    return new_board;
}
// Add this function definition in FourByFourClasses.cpp
Player<char>** FourByFourUI::setup_players_with_ai() {
    Player<char>** players = new Player<char>*[2];

    // Options including AI
    vector<string> type_options = { "Human", "Computer", "AI" };

    // Player X
    cout << "\n=== Setting up Player X ===\n";
    string nameX;
    cout << "Enter Player X name: ";
    cin.ignore();  // Clear any leftover newline
    getline(cin, nameX);

    cout << "\nChoose Player X type:\n";
    for (size_t i = 0; i < type_options.size(); ++i)
        cout << i + 1 << ". " << type_options[i] << "\n";

    int choiceX;
    cin >> choiceX;

    PlayerType typeX;
    if (choiceX == 1) typeX = PlayerType::HUMAN;
    else if (choiceX == 2) typeX = PlayerType::COMPUTER;
    else typeX = PlayerType::AI;  // choiceX == 3

    players[0] = new Player<char>(nameX, 'X', typeX);

    // Player O
    cout << "\n=== Setting up Player O ===\n";
    string nameO;
    cout << "Enter Player O name: ";
    cin.ignore();  // Clear newline from previous input
    getline(cin, nameO);

    cout << "\nChoose Player O type:\n";
    for (size_t i = 0; i < type_options.size(); ++i)
        cout << i + 1 << ". " << type_options[i] << "\n";

    int choiceO;
    cin >> choiceO;

    PlayerType typeO;
    if (choiceO == 1) typeO = PlayerType::HUMAN;
    else if (choiceO == 2) typeO = PlayerType::COMPUTER;
    else typeO = PlayerType::AI;  // choiceO == 3

    players[1] = new Player<char>(nameO, 'O', typeO);

    return players;
}

int FourByFourBoard::evaluate_board(char symbol) const {
    char opponent = (symbol == 'X') ? 'O' : 'X';

    // Terminal states
    if (check_three_in_line(symbol)) return 1000;
    if (check_three_in_line(opponent)) return -1000;

    int score = 0;

    // Heuristic evaluation
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == symbol) {
                // Center control bonus
                if ((i == 1 || i == 2) && (j == 1 || j == 2)) score += 3;

                // Mobility bonus
                if (i > 0 && board[i - 1][j] == ' ') score += 1;
                if (i < 3 && board[i + 1][j] == ' ') score += 1;
                if (j > 0 && board[i][j - 1] == ' ') score += 1;
                if (j < 3 && board[i][j + 1] == ' ') score += 1;
            }
            else if (board[i][j] == opponent) {
                // Penalize opponent's center control
                if ((i == 1 || i == 2) && (j == 1 || j == 2)) score -= 3;
            }
        }
    }

    return score;
}

int FourByFourBoard::minimax(int depth, bool isMaximizing, char symbol, int alpha, int beta, int maxDepth) {
    char opponent = (symbol == 'X') ? 'O' : 'X';

    // Terminal state evaluations
    if (check_three_in_line(symbol)) return 1000 - depth;
    if (check_three_in_line(opponent)) return -1000 + depth;

    // Check for draw (no valid moves for current player)
    auto moves = get_all_valid_moves(isMaximizing ? symbol : opponent);
    if (moves.empty() || depth == maxDepth) {
        return evaluate_board(symbol);
    }

    if (isMaximizing) {
        int maxEval = -10000;
        for (const auto& move : moves) {
            FourByFourBoard* nextBoard = this->clone();
            nextBoard->make_move(move.first, move.second, symbol);
            int eval = nextBoard->minimax(depth + 1, false, symbol, alpha, beta, maxDepth);
            delete nextBoard;

            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return maxEval;
    }
    else {
        int minEval = 10000;
        for (const auto& move : moves) {
            FourByFourBoard* nextBoard = this->clone();
            nextBoard->make_move(move.first, move.second, opponent);
            int eval = nextBoard->minimax(depth + 1, true, symbol, alpha, beta, maxDepth);
            delete nextBoard;

            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return minEval;
    }
}

pair<pair<int, int>, pair<int, int>> FourByFourBoard::get_best_move(char symbol) const {
    auto moves = get_all_valid_moves(symbol);
    if (moves.empty()) return { {-1, -1}, {-1, -1} };

    int bestValue = -10000;
    pair<pair<int, int>, pair<int, int>> bestMove = moves[0];
    char opponent = (symbol == 'X') ? 'O' : 'X';

    // Adjust depth based on game stage
    int maxDepth = 4; // Can be increased for stronger AI

    for (const auto& move : moves) {
        FourByFourBoard* nextBoard = this->clone();
        nextBoard->make_move(move.first, move.second, symbol);

        int moveValue = nextBoard->minimax(0, false, symbol, -10000, 10000, maxDepth);
        delete nextBoard;

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
    }

    return bestMove;
}

bool FourByFourBoard::check_three_in_line(char symbol) const {
    // Check rows
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            if (board[i][j] == symbol && board[i][j + 1] == symbol && board[i][j + 2] == symbol) {
                return true;
            }
        }
    }

    // Check columns
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 2; i++) {
            if (board[i][j] == symbol && board[i + 1][j] == symbol && board[i + 2][j] == symbol) {
                return true;
            }
        }
    }

    // Check main diagonals
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (board[i][j] == symbol && board[i + 1][j + 1] == symbol && board[i + 2][j + 2] == symbol) {
                return true;
            }
        }
    }

    // Check anti-diagonals
    for (int i = 0; i < 2; i++) {
        for (int j = 2; j < 4; j++) {
            if (board[i][j] == symbol && board[i + 1][j - 1] == symbol && board[i + 2][j - 2] == symbol) {
                return true;
            }
        }
    }

    return false;
}

bool FourByFourBoard::is_win(Player<char>* player) {
    return check_three_in_line(player->get_symbol());
}

bool FourByFourBoard::is_lose(Player<char>* player) {
    char opponent_symbol = (player->get_symbol() == 'X') ? 'O' : 'X';
    return check_three_in_line(opponent_symbol);
}

bool FourByFourBoard::is_draw(Player<char>* player) {
    // Check if no valid moves left for current player
    char symbol = player->get_symbol();
    auto moves = get_all_valid_moves(symbol);

    return moves.empty() && !is_win(player) && !is_lose(player);
}

bool FourByFourBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

// FourByFourUI Implementation
Player<char>* FourByFourUI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* FourByFourUI::get_move(Player<char>* player) {
    FourByFourBoard* board = dynamic_cast<FourByFourBoard*>(player->get_board_ptr());

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
            << "), you need to move one of your tokens.\n";

        // Display current board
        display_board_matrix(board->get_board_matrix());

        // Ask for destination only (source will be found automatically)
        int to_x, to_y;
        cout << "Enter destination row (0-3): ";
        cin >> to_x;
        cout << "Enter destination column (0-3): ";
        cin >> to_y;

        return new Move<char>(to_x, to_y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        // Random computer player
        auto valid_moves = board->get_all_valid_moves(player->get_symbol());

        if (!valid_moves.empty()) {
            srand(time(0));
            int idx = rand() % valid_moves.size();

            auto move = valid_moves[idx];
            cout << "\nComputer player " << player->get_name()
                << " moves from (" << move.first.first << "," << move.first.second
                << ") to (" << move.second.first << "," << move.second.second << ")\n";

            return new Move<char>(move.second.first, move.second.second, player->get_symbol());
        }

        return new Move<char>(0, 0, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::AI) {
        cout << "\nAI player " << player->get_name() << " is thinking...\n";

        auto best_move = board->get_best_move(player->get_symbol());

        if (best_move.first.first != -1) {
            cout << "AI moves from (" << best_move.first.first << "," << best_move.first.second
                << ") to (" << best_move.second.first << "," << best_move.second.second << ")\n";

            return new Move<char>(best_move.second.first, best_move.second.second, player->get_symbol());
        }

        return new Move<char>(0, 0, player->get_symbol());
    }

    return new Move<char>(0, 0, player->get_symbol());
}

void FourByFourUI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\nCurrent Board (4x4):\n";
    cout << "   ";
    for (int j = 0; j < 4; ++j)
        cout << " " << j << "  ";
    cout << "\n   ";
    for (int j = 0; j < 4; ++j)
        cout << "--- ";
    cout << "\n";

    for (int i = 0; i < 4; ++i) {
        cout << i << " |";
        for (int j = 0; j < 4; ++j) {
            char cell = matrix[i][j];
            if (cell == ' ') cell = '.';
            cout << " " << cell << " |";
        }
        cout << "\n";
        cout << "   ";
        for (int j = 0; j < 4; ++j)
            cout << "--- ";
        cout << "\n";
    }
}