/*#include "PyramidClasses.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// PyramidBoard Implementation
PyramidBoard::PyramidBoard() : Board(5, 5) {
    initialize_valid_cells();

    // Initialize all cells
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (is_valid_position(i, j)) {
                board[i][j] = ' ';
            }
            else {
                board[i][j] = '.'; // Mark invalid cells
            }
        }
    }
}

void PyramidBoard::initialize_valid_cells() {
    valid_cells.resize(5, vector<bool>(5, false));

    // Pyramid pattern
    valid_cells[0][2] = true;     // Top
    valid_cells[1][1] = true;     // Row 1
    valid_cells[1][3] = true;
    valid_cells[2][0] = true;     // Middle row
    valid_cells[2][2] = true;
    valid_cells[2][4] = true;
    valid_cells[3][1] = true;     // Row 3
    valid_cells[3][3] = true;
    valid_cells[4][2] = true;     // Bottom
}

bool PyramidBoard::is_valid_position(int x, int y) const {
    if (x < 0 || x >= 5 || y < 0 || y >= 5) return false;
    return valid_cells[x][y];
}

bool PyramidBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (is_valid_position(x, y) && board[x][y] == ' ') {
        board[x][y] = symbol;
        n_moves++;
        return true;
    }
    return false;
}

vector<pair<int, int>> PyramidBoard::get_empty_positions() const {
    vector<pair<int, int>> positions;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (is_valid_position(i, j) && board[i][j] == ' ') {
                positions.push_back({ i, j });
            }
        }
    }

    return positions;
}

int PyramidBoard::evaluate_position(int x, int y) const {
    // Higher score for better positions
    if (x == 2 && y == 2) return 5;  // Center is best
    if ((x == 0 && y == 2) || (x == 4 && y == 2) ||
        (x == 2 && y == 0) || (x == 2 && y == 4)) return 3;  // Center of edges
    return 1;  // Other positions
}

PyramidBoard* PyramidBoard::clone() const {
    PyramidBoard* new_board = new PyramidBoard();

    // Copy the board state
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            new_board->board[i][j] = board[i][j];
        }
    }
    new_board->n_moves = n_moves;
    new_board->valid_cells = valid_cells;

    return new_board;
}

int PyramidBoard::evaluate_board(char symbol) const {
    char opponent = (symbol == 'X') ? 'O' : 'X';

    // Terminal states
    if (check_three_in_line(symbol)) return 1000;
    if (check_three_in_line(opponent)) return -1000;

    int score = 0;

    // Position-based evaluation
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (is_valid_position(i, j)) {
                if (board[i][j] == symbol) {
                    score += evaluate_position(i, j);
                }
                else if (board[i][j] == opponent) {
                    score -= evaluate_position(i, j);
                }
            }
        }
    }

    return score;
}

int PyramidBoard::minimax(int depth, bool isMaximizing, char symbol, int alpha, int beta, int maxDepth) {
    char opponent = (symbol == 'X') ? 'O' : 'X';

    // Terminal state evaluations
    if (check_three_in_line(symbol)) return 1000 - depth;
    if (check_three_in_line(opponent)) return -1000 + depth;

    // Check for draw
    auto empty_positions = get_empty_positions();
    if (empty_positions.empty() || depth == maxDepth) {
        return evaluate_board(symbol);
    }

    if (isMaximizing) {
        int maxEval = -10000;
        for (const auto& pos : empty_positions) {
            PyramidBoard* nextBoard = this->clone();
            nextBoard->board[pos.first][pos.second] = symbol;
            nextBoard->n_moves++;

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
        for (const auto& pos : empty_positions) {
            PyramidBoard* nextBoard = this->clone();
            nextBoard->board[pos.first][pos.second] = opponent;
            nextBoard->n_moves++;

            int eval = nextBoard->minimax(depth + 1, true, symbol, alpha, beta, maxDepth);
            delete nextBoard;

            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break; // Alpha-beta pruning
        }
        return minEval;
    }
}

pair<int, int> PyramidBoard::get_best_move(char symbol) const {
    auto empty_positions = get_empty_positions();
    if (empty_positions.empty()) return { -1, -1 };

    int bestValue = -10000;
    pair<int, int> bestPosition = empty_positions[0];

    // Adjust depth based on available positions
    int maxDepth = min(6, (int)empty_positions.size());

    for (const auto& pos : empty_positions) {
        PyramidBoard* nextBoard = this->clone();
        nextBoard->board[pos.first][pos.second] = symbol;
        nextBoard->n_moves++;

        int moveValue = nextBoard->minimax(0, false, symbol, -10000, 10000, maxDepth);
        delete nextBoard;

        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestPosition = pos;
        }
    }

    return bestPosition;
}

bool PyramidBoard::check_three_in_line(char symbol) const {
    return check_horizontal_win(symbol) ||
        check_vertical_win(symbol) ||
        check_diagonal_win(symbol);
}

bool PyramidBoard::check_horizontal_win(char symbol) const {
    // Only middle row can have horizontal win
    return (board[2][0] == symbol && board[2][2] == symbol && board[2][4] == symbol);
}

bool PyramidBoard::check_vertical_win(char symbol) const {
    // Only middle column can have vertical win
    return (board[0][2] == symbol && board[2][2] == symbol && board[4][2] == symbol);
}

bool PyramidBoard::check_diagonal_win(char symbol) const {
    // Check main diagonals
    if (board[0][2] == symbol && board[2][0] == symbol && board[4][2] == symbol)
        return true;

    if (board[0][2] == symbol && board[2][4] == symbol && board[4][2] == symbol)
        return true;

    // Check smaller diagonals
    if (board[1][1] == symbol && board[2][2] == symbol && board[3][1] == symbol)
        return true;

    if (board[1][3] == symbol && board[2][2] == symbol && board[3][3] == symbol)
        return true;

    return false;
}

bool PyramidBoard::is_win(Player<char>* player) {
    return check_three_in_line(player->get_symbol());
}

bool PyramidBoard::is_lose(Player<char>* player) {
    char opponent_symbol = (player->get_symbol() == 'X') ? 'O' : 'X';
    return check_three_in_line(opponent_symbol);
}

bool PyramidBoard::is_draw(Player<char>* player) {
    // Check if all valid cells are filled
    return get_empty_positions().empty() && !is_win(player) && !is_lose(player);
}

bool PyramidBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_lose(player) || is_draw(player);
}

// PyramidUI Implementation
Player<char>* PyramidUI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* PyramidUI::get_move(Player<char>* player) {
    PyramidBoard* board = dynamic_cast<PyramidBoard*>(player->get_board_ptr());

    if (player->get_type() == PlayerType::HUMAN) {
        display_board_matrix(board->get_board_matrix());
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
            << "), enter your move:\n";

        int x, y;
        cout << "Enter row (0-4): ";
        cin >> x;
        cout << "Enter column (0-4): ";
        cin >> y;

        while (!is_valid_pyramid_position(x, y) ||
            board->get_board_matrix()[x][y] != ' ') {
            cout << "Invalid position! Please enter a valid empty pyramid position.\n";
            cout << "Enter row (0-4): ";
            cin >> x;
            cout << "Enter column (0-4): ";
            cin >> y;
        }

        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        // Random computer player
        auto valid_positions = board->get_empty_positions();

        if (!valid_positions.empty()) {
            srand(time(0));
            int idx = rand() % valid_positions.size();

            auto pos = valid_positions[idx];
            cout << "\nComputer player " << player->get_name()
                << " places at (" << pos.first << ", " << pos.second << ")\n";

            return new Move<char>(pos.first, pos.second, player->get_symbol());
        }

        return new Move<char>(0, 2, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::AI) {
        cout << "\nAI player " << player->get_name() << " is thinking...\n";

        auto best_move = board->get_best_move(player->get_symbol());

        if (best_move.first != -1) {
            cout << "AI places at (" << best_move.first << ", " << best_move.second << ")\n";
            return new Move<char>(best_move.first, best_move.second, player->get_symbol());
        }

        return new Move<char>(0, 2, player->get_symbol());
    }

    return new Move<char>(0, 2, player->get_symbol());
}

bool PyramidUI::is_valid_pyramid_position(int x, int y) const {
    if (x < 0 || x >= 5 || y < 0 || y >= 5) return false;

    // Pyramid valid positions:
    if (x == 0 && y == 2) return true;
    if (x == 1 && (y == 1 || y == 3)) return true;
    if (x == 2 && (y == 0 || y == 2 || y == 4)) return true;
    if (x == 3 && (y == 1 || y == 3)) return true;
    if (x == 4 && y == 2) return true;

    return false;
}
Player<char>** PyramidUI::setup_players_with_ai() {
    Player<char>** players = new Player<char>*[2];

    // Options including AI
    vector<string> type_options = { "Human", "Computer", "AI" };

    // Player X setup
    cout << "\n=== Setting up Player X ===\n";
    string nameX;
    cout << "Enter Player X name: ";
    cin.ignore();  // Clear newline from previous input
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

    players[0] = create_player(nameX, 'X', typeX);

    // Player O setup
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

    players[1] = create_player(nameO, 'O', typeO);

    return players;
}
void PyramidUI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\nPyramid Board:\n\n";

    // Display pyramid shape
    cout << "      " << matrix[0][2] << "\n\n";
    cout << "    " << matrix[1][1] << "   " << matrix[1][3] << "\n\n";
    cout << "  " << matrix[2][0] << "   " << matrix[2][2] << "   " << matrix[2][4] << "\n\n";
    cout << "    " << matrix[3][1] << "   " << matrix[3][3] << "\n\n";
    cout << "      " << matrix[4][2] << "\n";

    cout << "\nCoordinates:\n";
    cout << "Top (0,2)\n";
    cout << "Row 1: (1,1), (1,3)\n";
    cout << "Center Row: (2,0), (2,2), (2,4)\n";
    cout << "Row 3: (3,1), (3,3)\n";
    cout << "Bottom (4,2)\n\n";
    }*/
#include "PyramidClasses.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// === PYRAMID BOARD IMPLEMENTATION ===

PyramidBoard::PyramidBoard() : Board(3, 5) { // Use 3 rows to represent the pyramid (0,1,2)
    // Initialize board with spaces
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            board[i][j] = ' ';
        }
    }
}

bool PyramidBoard::is_valid_position(int x, int y) const {
    if (x < 0 || x > 2 || y < 0 || y > 4) return false;

    // Pyramid Geometry Rule:
    // Row 0: 1 cell at center (0, 2)
    // Row 1: 3 cells at (1, 1), (1, 2), (1, 3)
    // Row 2: 5 cells at (2, 0) to (2, 4)

    if (x == 0 && y == 2) return true;
    if (x == 1 && (y >= 1 && y <= 3)) return true;
    if (x == 2) return true; // All columns valid in base row

    return false;
}

bool PyramidBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();

    if (!is_valid_position(x, y) || board[x][y] != ' ') return false;

    board[x][y] = move->get_symbol();
    n_moves++;
    return true;
}

bool PyramidBoard::check_horizontal_win(char symbol) const {
    // Row 1 (needs 3 consecutive): (1,1), (1,2), (1,3)
    if (board[1][1] == symbol && board[1][2] == symbol && board[1][3] == symbol) return true;

    // Row 2 (needs 3 consecutive):
    // (2,0)-(2,2), (2,1)-(2,3), (2,2)-(2,4)
    if (board[2][0] == symbol && board[2][1] == symbol && board[2][2] == symbol) return true;
    if (board[2][1] == symbol && board[2][2] == symbol && board[2][3] == symbol) return true;
    if (board[2][2] == symbol && board[2][3] == symbol && board[2][4] == symbol) return true;

    return false;
}

bool PyramidBoard::check_vertical_win(char symbol) const {
    // Only one vertical line of 3 exists: the center column
    // (0,2), (1,2), (2,2)
    return (board[0][2] == symbol && board[1][2] == symbol && board[2][2] == symbol);
}

bool PyramidBoard::check_diagonal_win(char symbol) const {
    // Diagonals crossing the center or edges
    // Left-to-Right Diagonals:
    if (board[0][2] == symbol && board[1][3] == symbol && board[2][4] == symbol) return true; // (0,2)->(2,4)
    if (board[2][0] == symbol && board[1][1] == symbol && board[0][2] == symbol) return true; // (2,0)->(0,2) wait direction

    // Let's trace lines explicitly visually:
    // / shape: (2,0)-(1,1)-(0,2)
    // \ shape: (0,2)-(1,3)-(2,4)

    if (board[2][0] == symbol && board[1][1] == symbol && board[0][2] == symbol) return true;
    if (board[0][2] == symbol && board[1][3] == symbol && board[2][4] == symbol) return true;

    return false;
}

bool PyramidBoard::is_win(Player<char>* player) {
    char s = player->get_symbol();
    return check_horizontal_win(s) || check_vertical_win(s) || check_diagonal_win(s);
}

bool PyramidBoard::is_lose(Player<char>* player) {
    return false; // Standard tic tac toe logic
}

bool PyramidBoard::is_draw(Player<char>* player) {
    // Total cells = 1 + 3 + 5 = 9.
    return (n_moves >= 9) && !is_win(player);
}

bool PyramidBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

vector<pair<int, int>> PyramidBoard::get_empty_positions() const {
    vector<pair<int, int>> moves;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (is_valid_position(i, j) && board[i][j] == ' ') {
                moves.push_back({ i, j });
            }
        }
    }
    return moves;
}

// AI Helpers
PyramidBoard* PyramidBoard::clone() const {
    PyramidBoard* b = new PyramidBoard();
    b->board = this->board;
    b->n_moves = this->n_moves;
    return b;
}

int PyramidBoard::minimax(int depth, bool isMaximizing, char symbol, int alpha, int beta) {
    char opp = (symbol == 'X') ? 'O' : 'X';

    // Fake player objects for check
    Player<char> pMax("Max", symbol, PlayerType::AI);
    Player<char> pMin("Min", opp, PlayerType::AI);

    if (is_win(&pMax)) return 100 - depth;
    if (is_win(&pMin)) return -100 + depth;
    if (get_empty_positions().empty()) return 0;

    if (isMaximizing) {
        int best = -1000;
        for (auto m : get_empty_positions()) {
            PyramidBoard* newB = clone();
            newB->board[m.first][m.second] = symbol;
            newB->n_moves++;
            best = max(best, newB->minimax(depth + 1, false, symbol, alpha, beta));
            delete newB;
            alpha = max(alpha, best);
            if (beta <= alpha) break;
        }
        return best;
    }
    else {
        int best = 1000;
        for (auto m : get_empty_positions()) {
            PyramidBoard* newB = clone();
            newB->board[m.first][m.second] = opp;
            newB->n_moves++;
            best = min(best, newB->minimax(depth + 1, true, symbol, alpha, beta));
            delete newB;
            beta = min(beta, best);
            if (beta <= alpha) break;
        }
        return best;
    }
}

pair<int, int> PyramidBoard::get_best_move(char symbol) const {
    int bestVal = -1000;
    pair<int, int> bestMove = { -1, -1 };

    for (auto m : get_empty_positions()) {
        PyramidBoard* newB = clone();
        newB->board[m.first][m.second] = symbol;
        newB->n_moves++;

        int moveVal = newB->minimax(0, false, symbol, -1000, 1000);
        delete newB;

        if (moveVal > bestVal) {
            bestMove = m;
            bestVal = moveVal;
        }
    }
    return bestMove;
}

// === PYRAMID UI IMPLEMENTATION ===

bool PyramidUI::is_valid_pyramid_position(int x, int y) const {
    if (x == 0 && y == 2) return true;
    if (x == 1 && (y >= 1 && y <= 3)) return true;
    if (x == 2 && (y >= 0 && y <= 4)) return true;
    return false;
}

Player<char>* PyramidUI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Player<char>** PyramidUI::setup_players_with_ai() {
    Player<char>** players = new Player<char>*[2];

    // Simple console setup
    cout << "Enter Player X Name: ";
    string n1; cin >> n1;
    cout << "Type (1: Human, 2: Random, 3: AI): ";
    int t1; cin >> t1;
    PlayerType pt1 = (t1 == 1) ? PlayerType::HUMAN : (t1 == 2) ? PlayerType::COMPUTER : PlayerType::AI;

    cout << "Enter Player O Name: ";
    string n2; cin >> n2;
    cout << "Type (1: Human, 2: Random, 3: AI): ";
    int t2; cin >> t2;
    PlayerType pt2 = (t2 == 1) ? PlayerType::HUMAN : (t2 == 2) ? PlayerType::COMPUTER : PlayerType::AI;

    players[0] = create_player(n1, 'X', pt1);
    players[1] = create_player(n2, 'O', pt2);

    return players;
}

Move<char>* PyramidUI::get_move(Player<char>* player) {
    PyramidBoard* pb = dynamic_cast<PyramidBoard*>(player->get_board_ptr());

    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << player->get_name() << " (" << player->get_symbol() << ") move (row col): ";
        cin >> x >> y;
        while (!is_valid_pyramid_position(x, y) || pb->get_board_matrix()[x][y] != ' ') {
            cout << "Invalid. Try again (row col): ";
            cin >> x >> y;
        }
        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::AI) {
        pair<int, int> best = pb->get_best_move(player->get_symbol());
        return new Move<char>(best.first, best.second, player->get_symbol());
    }
    else {
        // Random
        auto empty = pb->get_empty_positions();
        if (empty.empty()) return new Move<char>(0, 0, ' ');
        auto m = empty[rand() % empty.size()];
        return new Move<char>(m.first, m.second, player->get_symbol());
    }
}

void PyramidUI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\nPyramid Board:\n";

    // Row 0
    cout << "        | " << matrix[0][2] << " |\n";
    cout << "      ----------- \n";

    // Row 1
    cout << "    | " << matrix[1][1] << " | " << matrix[1][2] << " | " << matrix[1][3] << " |\n";
    cout << "  ------------------- \n";

    // Row 2
    cout << "| " << matrix[2][0] << " | " << matrix[2][1] << " | " << matrix[2][2]
        << " | " << matrix[2][3] << " | " << matrix[2][4] << " |\n";
    cout << "---------------------\n\n";
}