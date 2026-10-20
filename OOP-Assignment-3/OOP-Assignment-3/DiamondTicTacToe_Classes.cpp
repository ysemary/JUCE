/*#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <algorithm>
#include <random>
#include "DiamondTicTacToe_Classes.h"

using namespace std;

DiamondBoard::DiamondBoard() : Board<char>(5, 5) {
    board = vector<vector<char>>(5, vector<char>(5, blank_symbol));
}

bool DiamondBoard::is_valid_position(int x, int y) const {
    int sum = x + y;
    return x >= 0 && x < rows && y >= 0 && y < columns &&
        sum >= 2 && sum <= 6;
}

vector<vector<pair<int, int>>> DiamondBoard::get_three_lines() const {
    vector<vector<pair<int, int>>> lines;

    // Horizontal lines
    // Top triangle
    lines.push_back({ {0,2}, {1,2}, {2,2} });
    lines.push_back({ {1,1}, {2,1}, {3,1} });
    lines.push_back({ {1,3}, {2,3}, {3,3} });
    lines.push_back({ {2,0}, {3,0}, {4,0} });
    lines.push_back({ {2,4}, {3,4}, {4,4} });

    // Bottom triangle
    lines.push_back({ {2,0}, {2,1}, {2,2} });
    lines.push_back({ {2,2}, {2,3}, {2,4} });
    lines.push_back({ {3,1}, {3,2}, {3,3} });
    lines.push_back({ {4,0}, {4,1}, {4,2} });
    lines.push_back({ {4,2}, {4,3}, {4,4} });

    // Vertical lines 
    lines.push_back({ {0,2}, {1,1}, {2,0} });
    lines.push_back({ {0,2}, {1,3}, {2,4} });
    lines.push_back({ {1,1}, {2,2}, {3,3} });
    lines.push_back({ {1,3}, {2,2}, {3,1} });
    lines.push_back({ {2,0}, {3,1}, {4,2} });
    lines.push_back({ {2,4}, {3,3}, {4,2} });

    // Additional horizontal in center
    lines.push_back({ {1,2}, {2,2}, {3,2} });

    return lines;
}

vector<vector<pair<int, int>>> DiamondBoard::get_four_lines() const {
    vector<vector<pair<int, int>>> lines;

    // Horizontal lines 
    lines.push_back({ {0,2}, {1,2}, {2,2}, {3,2} });
    lines.push_back({ {1,1}, {2,1}, {3,1}, {4,1} });
    lines.push_back({ {1,3}, {2,3}, {3,3}, {4,3} });

    // Vertical lines
    lines.push_back({ {0,2}, {1,1}, {2,0}, {3,1} });
    lines.push_back({ {0,2}, {1,3}, {2,4}, {3,3} });
    lines.push_back({ {1,1}, {2,2}, {3,3}, {4,4} });
    lines.push_back({ {1,3}, {2,2}, {3,1}, {4,0} });

    // Diagonal lines
    lines.push_back({ {1,1}, {2,2}, {3,3}, {4,4} });
    lines.push_back({ {1,3}, {2,2}, {3,1}, {4,0} });
    lines.push_back({ {0,2}, {1,2}, {2,2}, {3,2} });

    return lines;
}

int DiamondBoard::count_in_line(const vector<pair<int, int>>& line, char symbol) const {
    int count = 0;
    for (const auto& pos : line) {
        if (board[pos.first][pos.second] == symbol) {
            count++;
        }
    }
    return count;
}
void DiamondUI::display_board_matrix(const vector<vector<char>>& matrix) const {
    display_diamond_board(matrix);
}

bool DiamondBoard::has_line(const vector<pair<int, int>>& line, char symbol) const {
    for (const auto& pos : line) {
        if (board[pos.first][pos.second] != symbol) {
            return false;
        }
    }
    return true;
}

bool DiamondBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    
    if (!is_valid_position(x, y)) {
        return false;
    }

    if (board[x][y] != blank_symbol) {
        return false;
    }

    board[x][y] = mark;
    n_moves++;
    return true;
}

bool DiamondBoard::is_win(Player<char>* player) {
    char symbol = player->get_symbol();
    auto three_lines = get_three_lines();
    auto four_lines = get_four_lines();
    bool has_three_line = false;
    bool has_four_line = false;
    pair<int, int> shared_pos_three, shared_pos_four;
    
    for (const auto& line : three_lines) {
        if (has_line(line, symbol)) {
            has_three_line = true;
            shared_pos_three = line[1];
            break;
        }
    }

    for (const auto& line : four_lines) {
        if (has_line(line, symbol)) {
            has_four_line = true;
            shared_pos_four = line[1];
            break;
        }
    }

    if (has_three_line && has_four_line) {
        
        for (const auto& three_line : three_lines) {
        
            if (!has_line(three_line, symbol)) continue;

            for (const auto& four_line : four_lines) {
            
                if (!has_line(four_line, symbol)) continue;

                for (const auto& pos1 : three_line) {
                
                    for (const auto& pos2 : four_line) {
                    
                        if (pos1 == pos2) {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool DiamondBoard::is_draw(Player<char>* player) {
    
    if (n_moves >= 13) {
    
        for (int i = 0; i < rows; i++) {
        
            for (int j = 0; j < columns; j++) {
            
                if (is_valid_position(i, j) && board[i][j] == blank_symbol) {
                    return false;
                }
            }
        }
        return !is_win(player);
    }
    return false;
}

bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

vector<DiamondMove> DiamondBoard::get_valid_moves(char symbol) const {
    vector<DiamondMove> moves;

    for (int i = 0; i < rows; i++) {
        
        for (int j = 0; j < columns; j++) {
        
            if (is_valid_position(i, j) && board[i][j] == blank_symbol) {
                moves.push_back(DiamondMove(i, j, symbol));
            }
        }
    }

    return moves;
}

DiamondBoard DiamondBoard::make_move(const DiamondMove& move) const {
    DiamondBoard new_board = *this;
    new_board.update_board(new DiamondMove(move.get_x(), move.get_y(), move.get_symbol()));
    return new_board;
}

int DiamondBoard::evaluate(char player_symbol) const {
    char opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';
    int score = 0;
    auto three_lines = get_three_lines();
    auto four_lines = get_four_lines();

    for (const auto& line : three_lines) {
        int player_count = count_in_line(line, player_symbol);
        int opponent_count = count_in_line(line, opponent_symbol);
        if (player_count == 3) score += 100;
        else if (player_count == 2 && opponent_count == 0) score += 10;
        else if (player_count == 1 && opponent_count == 0) score += 1;
        if (opponent_count == 3) score -= 100;
        else if (opponent_count == 2 && player_count == 0) score -= 10;
        else if (opponent_count == 1 && player_count == 0) score -= 1;
    }

    for (const auto& line : four_lines) {
        int player_count = count_in_line(line, player_symbol);
        int opponent_count = count_in_line(line, opponent_symbol);
        if (player_count == 4) score += 200;
        else if (player_count == 3 && opponent_count == 0) score += 20;
        else if (player_count == 2 && opponent_count == 0) score += 2;
        if (opponent_count == 4) score -= 200;
        else if (opponent_count == 3 && player_count == 0) score -= 20;
        else if (opponent_count == 2 && player_count == 0) score -= 2;
    }

    if (board[2][2] == player_symbol) score += 5;
    if (board[2][2] == opponent_symbol) score -= 5;

    return score;
}

char DiamondBoard::get_opponent(char symbol) const {
    return (symbol == 'X') ? 'O' : 'X';
}

bool DiamondBoard::is_corner(int x, int y) const {
    if (!is_valid_position(x, y)) return false;
    return (x == 0 && y == 2) || (x == 2 && y == 0) || (x == 2 && y == 4) || (x == 4 && y == 2);
}

bool DiamondBoard::is_edge(int x, int y) const {
    if (!is_valid_position(x, y)) return false;
    return (x == 1 && y == 1) || (x == 1 && y == 3) || (x == 3 && y == 1) || (x == 3 && y == 3);
}

bool DiamondBoard::is_center(int x, int y) const {
    return is_valid_position(x, y) && x == 2 && y == 2;
}

AIPlayer::AIPlayer(string name, char symbol, int search_depth)
    : Player<char>(name, symbol, PlayerType::AI), depth(search_depth) {
}

int AIPlayer::minimax(DiamondBoard& board, int depth, bool maximizing,
    int alpha, int beta, char player_symbol) {
    if (depth == 0) {
        return board.evaluate(player_symbol);
    }

    char current_symbol = maximizing ? player_symbol : board.get_opponent(player_symbol);
    auto valid_moves = board.get_valid_moves(current_symbol);

    if (valid_moves.empty()) {
        return board.evaluate(player_symbol);
    }

    if (maximizing) {
        int max_eval = numeric_limits<int>::min();

        for (const auto& move : valid_moves) {
            DiamondBoard new_board = board.make_move(move);
            int eval = minimax(new_board, depth - 1, false, alpha, beta, player_symbol);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break;
        }
        return max_eval;
    }
    else {
        int min_eval = numeric_limits<int>::max();
       
        for (const auto& move : valid_moves) {
            DiamondBoard new_board = board.make_move(move);
            int eval = minimax(new_board, depth - 1, true, alpha, beta, player_symbol);
            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break;
        }
        return min_eval;
    }
}

DiamondMove AIPlayer::get_best_move() {
    DiamondBoard* boardPtr = dynamic_cast<DiamondBoard*>(get_board_ptr());
    
    if (!boardPtr) {
        return DiamondMove(2, 2, get_symbol());
    }

    DiamondBoard board = *boardPtr;
    auto valid_moves = board.get_valid_moves(get_symbol());

    if (valid_moves.empty()) {
        return DiamondMove(2, 2, get_symbol());
    }

    for (const auto& move : valid_moves) {
        
        if (board.is_center(move.get_x(), move.get_y())) {
            return move;
        }
    }

    int best_value = numeric_limits<int>::min();
    DiamondMove best_move = valid_moves[0];

    for (const auto& move : valid_moves) {
        DiamondBoard new_board = board.make_move(move);
        int move_value = minimax(new_board, depth - 1, false,
            numeric_limits<int>::min(),
            numeric_limits<int>::max(),
            get_symbol());

        if (move_value > best_value) {
            best_value = move_value;
            best_move = move;
        }
    }

    return best_move;
}

RandomPlayer::RandomPlayer(string name, char symbol)
    : Player<char>(name, symbol, PlayerType::RANDOM), rng(random_device{}()) {
}

DiamondMove RandomPlayer::get_random_move() {
    DiamondBoard* boardPtr = dynamic_cast<DiamondBoard*>(get_board_ptr());

    if (!boardPtr) {
        return DiamondMove(2, 2, get_symbol());
    }

    DiamondBoard board = *boardPtr;
    auto valid_moves = board.get_valid_moves(get_symbol());

    if (valid_moves.empty()) {
        return DiamondMove(2, 2, get_symbol());
    }

    uniform_int_distribution<size_t> dist(0, valid_moves.size() - 1);
    return valid_moves[dist(rng)];
}

DiamondUI::DiamondUI()
    : UI<char>("Welcome to Diamond Tic-Tac-Toe Game", 3) {
}

PlayerType DiamondUI::get_player_type_choice_extended(string player_label) {
    cout << "\nChoose " << player_label << " type:\n";
    cout << "1. Human\n";
    cout << "2. Computer (Random)\n";
    cout << "3. AI (Smart)\n";

    int choice;
    while (true) {
        cout << "Enter choice (1-3): ";
        cin >> choice;

        if (choice >= 1 && choice <= 3) {
            break;
        }
        cout << "Invalid choice. Please enter 1, 2, or 3.\n";
    }

    switch (choice) {
    case 1: return PlayerType::HUMAN;
    case 2: return PlayerType::RANDOM;
    case 3: return PlayerType::AI;
    default: return PlayerType::HUMAN;
    }
}

Player<char>* DiamondUI::create_player(string& name, char symbol, PlayerType type) {
    cout << "\nCreating ";

    switch (type) {
    case PlayerType::HUMAN:
        cout << "human";
        break;
    case PlayerType::RANDOM:
        cout << "random computer";
        break;
    case PlayerType::AI:
        cout << "AI";
        break;
    default:
        cout << "computer";
    }

    cout << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::AI) {
        return new AIPlayer(name, symbol, 4);
    }
    
    else if (type == PlayerType::RANDOM) {
        return new RandomPlayer(name, symbol);
    }
    
    else {
        return new Player<char>(name, symbol, type);
    }
}

Move<char>* DiamondUI::get_move(Player<char>* player) {
    DiamondBoard* board = dynamic_cast<DiamondBoard*>(player->get_board_ptr());

    if (!board) {
        return new DiamondMove(0, 0, player->get_symbol());
    }

    switch (player->get_type()) {
    case PlayerType::HUMAN: {
        int x, y;
        cout << "\n" << player->get_name() << " (" << player->get_symbol()
            << "), enter your move (row and column, 0-4): ";

        while (true) {
            cin >> x >> y;

            if (x >= 0 && x < 5 && y >= 0 && y < 5) {
                int sum = x + y;
    
                if (sum >= 2 && sum <= 6) {
                
                    if (board->get_board_matrix()[x][y] == ' ') {
                        break;
                    }
                    
                    else {
                        cout << "Position already occupied. Try again: ";
                    }
                }
                
                else {
                    cout << "Invalid position (must be within diamond shape). Try again: ";
                }
            }
            
            else {
                cout << "Invalid coordinates (must be 0-4). Try again: ";
            }
        }
        return new DiamondMove(x, y, player->get_symbol());
    }

    case PlayerType::RANDOM: {
        RandomPlayer* randomPlayer = dynamic_cast<RandomPlayer*>(player);
        if (randomPlayer) {
            DiamondMove move = randomPlayer->get_random_move();
            cout << "\n" << player->get_name() << " (Random) chooses position: "
                << move.get_x() << " " << move.get_y() << endl;
            return new DiamondMove(move.get_x(), move.get_y(), player->get_symbol());
        }
        return new DiamondMove(2, 2, player->get_symbol());
    }

    case PlayerType::AI: {
        AIPlayer* aiPlayer = dynamic_cast<AIPlayer*>(player);
        if (aiPlayer) {
            DiamondMove move = aiPlayer->get_best_move();
            cout << "\n" << player->get_name() << " (AI) chooses position: "
                << move.get_x() << " " << move.get_y() << endl;
            return new DiamondMove(move.get_x(), move.get_y(), player->get_symbol());
        }
        return new DiamondMove(2, 2, player->get_symbol());
    }

    default: {
        cout << "\n" << player->get_name() << " makes a move.\n";
        int x = rand() % 5;
        int y = rand() % 5;
        return new DiamondMove(x, y, player->get_symbol());
    }
    }
}

Player<char>** DiamondUI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    cout << "\n=== Player X Setup ===" << endl;
    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice_extended("Player X");
    players[0] = create_player(nameX, 'X', typeX);
    cout << "\n=== Player O Setup ===" << endl;
    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice_extended("Player O");
    players[1] = create_player(nameO, 'O', typeO);

    return players;
}

void DiamondUI::display_diamond_board(const vector<vector<char>>& matrix) const {
    cout << "\n    ";
    
    for (int j = 0; j < 5; ++j) {
        cout << setw(3) << j;
    }
    cout << "\n   " << string(15, '-') << "\n";

    for (int i = 0; i < 5; ++i) {
        cout << setw(2) << i << " |";
    
        for (int j = 0; j < 5; ++j) {
        
            if (i + j >= 2 && i + j <= 6) {
                cout << setw(2) << matrix[i][j] << "|";
            }
            
            else {
                cout << "  |";
            }
        }
        cout << "\n";
    }
    cout << endl;
}*/



#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include "DiamondTicTacToe_Classes.h"

using namespace std;

DiamondBoard::DiamondBoard() : Board<char>(5, 5) {
    // Initialize standard board but we will only treat specific cells as valid
    board = vector<vector<char>>(5, vector<char>(5, blank_symbol));
}

bool DiamondBoard::is_valid_position(int x, int y) const {
    if (x < 0 || x >= 5 || y < 0 || y >= 5) return false;
    // Manhattan distance from center (2,2) must be <= 2
    return (abs(x - 2) + abs(y - 2)) <= 2;
}

vector<pair<vector<pair<int, int>>, LineDirection>> DiamondBoard::get_lines_of_size(int size) const {
    vector<pair<vector<pair<int, int>>, LineDirection>> lines;

    if (size == 4) {
        // In a 5x5 diamond (radius 2), only the center Row (2) and center Col (2) have length 5.
        // Therefore, lines of length 4 can ONLY exist in Row 2 or Col 2.

        // Horizontal Row 2: (2,0)-(2,3) and (2,1)-(2,4)
        lines.push_back({ {{2,0}, {2,1}, {2,2}, {2,3}}, LineDirection::HORIZONTAL });
        lines.push_back({ {{2,1}, {2,2}, {2,3}, {2,4}}, LineDirection::HORIZONTAL });

        // Vertical Col 2: (0,2)-(3,2) and (1,2)-(4,2)
        lines.push_back({ {{0,2}, {1,2}, {2,2}, {3,2}}, LineDirection::VERTICAL });
        lines.push_back({ {{1,2}, {2,2}, {3,2}, {4,2}}, LineDirection::VERTICAL });
    }
    else if (size == 3) {
        // -- HORIZONTAL --
        // Row 1: (1,1)-(1,3)
        lines.push_back({ {{1,1}, {1,2}, {1,3}}, LineDirection::HORIZONTAL });
        // Row 2: (2,0)-(2,2), (2,1)-(2,3), (2,2)-(2,4)
        lines.push_back({ {{2,0}, {2,1}, {2,2}}, LineDirection::HORIZONTAL });
        lines.push_back({ {{2,1}, {2,2}, {2,3}}, LineDirection::HORIZONTAL });
        lines.push_back({ {{2,2}, {2,3}, {2,4}}, LineDirection::HORIZONTAL });
        // Row 3: (3,1)-(3,3)
        lines.push_back({ {{3,1}, {3,2}, {3,3}}, LineDirection::HORIZONTAL });

        // -- VERTICAL --
        // Col 1: (1,1)-(3,1)
        lines.push_back({ {{1,1}, {2,1}, {3,1}}, LineDirection::VERTICAL });
        // Col 2: (0,2)-(2,2), (1,2)-(3,2), (2,2)-(4,2)
        lines.push_back({ {{0,2}, {1,2}, {2,2}}, LineDirection::VERTICAL });
        lines.push_back({ {{1,2}, {2,2}, {3,2}}, LineDirection::VERTICAL });
        lines.push_back({ {{2,2}, {3,2}, {4,2}}, LineDirection::VERTICAL });
        // Col 3: (1,3)-(3,3)
        lines.push_back({ {{1,3}, {2,3}, {3,3}}, LineDirection::VERTICAL });

        // -- DIAGONAL --
        // Main diagonals crossing center
        lines.push_back({ {{0,2}, {1,3}, {2,4}}, LineDirection::DIAGONAL });
        lines.push_back({ {{0,2}, {1,1}, {2,0}}, LineDirection::DIAGONAL });
        lines.push_back({ {{4,2}, {3,3}, {2,4}}, LineDirection::DIAGONAL });
        lines.push_back({ {{4,2}, {3,1}, {2,0}}, LineDirection::DIAGONAL });
        // Inner diamond diagonals
        lines.push_back({ {{1,2}, {2,3}, {3,2}}, LineDirection::DIAGONAL });
        lines.push_back({ {{1,2}, {2,1}, {3,2}}, LineDirection::DIAGONAL });
    }
    return lines;
}

bool DiamondBoard::has_line(const vector<pair<int, int>>& line, char symbol) const {
    for (const auto& pos : line) {
        if (board[pos.first][pos.second] != symbol) {
            return false;
        }
    }
    return true;
}

bool DiamondBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();

    if (!is_valid_position(x, y)) return false;
    if (board[x][y] != blank_symbol) return false;

    board[x][y] = move->get_symbol();
    n_moves++;
    return true;
}

bool DiamondBoard::is_win(Player<char>* player) {
    char s = player->get_symbol();

    auto lines3 = get_lines_of_size(3);
    auto lines4 = get_lines_of_size(4);

    // We need to find at least one line of 4 and one line of 3
    // such that their directions are different.

    for (const auto& l4 : lines4) {
        if (has_line(l4.first, s)) {
            // Found a line of 4. Now search for a compatible line of 3.
            for (const auto& l3 : lines3) {
                if (has_line(l3.first, s)) {
                    // Check if directions are different
                    if (l4.second != l3.second) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool DiamondBoard::is_draw(Player<char>* player) {
    // Total valid cells in a diamond radius 2 = 13 cells.
    return (n_moves >= 13) && !is_win(player);
}

bool DiamondBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

vector<DiamondMove> DiamondBoard::get_valid_moves(char symbol) const {
    vector<DiamondMove> moves;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (is_valid_position(i, j) && board[i][j] == blank_symbol) {
                moves.push_back(DiamondMove(i, j, symbol));
            }
        }
    }
    return moves;
}

DiamondBoard DiamondBoard::make_move(const DiamondMove& move) const {
    DiamondBoard new_board = *this;
    new_board.update_board(new DiamondMove(move.get_x(), move.get_y(), move.get_symbol()));
    return new_board;
}

// === FIX IS HERE ===
int DiamondBoard::evaluate(char player_symbol) const {
    char opp = get_opponent(player_symbol);

    // We define a local lambda function to check for wins.
    // This allows us to use logic identical to "is_win" but inside a const function,
    // because "get_lines_of_size" and "has_line" are const.
    auto check_win_const = [&](char sym) -> bool {
        auto lines3 = get_lines_of_size(3);
        auto lines4 = get_lines_of_size(4);

        for (const auto& l4 : lines4) {
            if (has_line(l4.first, sym)) {
                for (const auto& l3 : lines3) {
                    if (has_line(l3.first, sym)) {
                        if (l4.second != l3.second) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
        };

    if (check_win_const(player_symbol)) return 1000;
    if (check_win_const(opp)) return -1000;

    return 0;
}

char DiamondBoard::get_opponent(char symbol) const {
    return (symbol == 'X') ? 'O' : 'X';
}

// AI Player Implementation
AIPlayer::AIPlayer(string name, char symbol, int search_depth)
    : Player<char>(name, symbol, PlayerType::AI), depth(search_depth) {}

int AIPlayer::minimax(DiamondBoard& board, int depth, bool maximizing, int alpha, int beta, char player_symbol) {
    // Check terminal states
    char opp = board.get_opponent(player_symbol);

    // Create temp players for check
    Player<char> pMax(name, player_symbol, PlayerType::AI);
    Player<char> pMin("Opp", opp, PlayerType::AI);

    // Note: board here is NOT const, so we can call is_win safely.
    if (board.is_win(&pMax)) return 1000 + depth;
    if (board.is_win(&pMin)) return -1000 - depth;
    if (board.is_draw(&pMax)) return 0;
    if (depth == 0) return board.evaluate(player_symbol);

    char current_sym = maximizing ? player_symbol : opp;
    auto moves = board.get_valid_moves(current_sym);

    if (maximizing) {
        int maxEval = -99999;
        for (auto& mv : moves) {
            DiamondBoard newB = board.make_move(mv);
            int eval = minimax(newB, depth - 1, false, alpha, beta, player_symbol);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    }
    else {
        int minEval = 99999;
        for (auto& mv : moves) {
            DiamondBoard newB = board.make_move(mv);
            int eval = minimax(newB, depth - 1, true, alpha, beta, player_symbol);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

DiamondMove AIPlayer::get_best_move() {
    DiamondBoard* b = dynamic_cast<DiamondBoard*>(boardPtr);
    if (!b) return DiamondMove(2, 2, symbol);

    auto moves = b->get_valid_moves(symbol);
    if (moves.empty()) return DiamondMove(0, 0, symbol);

    DiamondMove bestMove = moves[0];
    int bestVal = -99999;

    // Initial call
    for (auto& mv : moves) {
        DiamondBoard newB = b->make_move(mv);
        int val = minimax(newB, depth, false, -99999, 99999, symbol);
        if (val > bestVal) {
            bestVal = val;
            bestMove = mv;
        }
    }
    return bestMove;
}

// Random Player
RandomPlayer::RandomPlayer(string name, char symbol) : Player<char>(name, symbol, PlayerType::RANDOM) {}

DiamondMove RandomPlayer::get_random_move() {
    DiamondBoard* b = dynamic_cast<DiamondBoard*>(boardPtr);
    auto moves = b->get_valid_moves(symbol);
    if (moves.empty()) return DiamondMove(0, 0, symbol);
    return moves[rand() % moves.size()];
}

// UI Implementation
DiamondUI::DiamondUI() : UI<char>("Welcome to Diamond Tic-Tac-Toe", 3) {}

PlayerType DiamondUI::get_player_type_choice_extended(string player_label) {
    cout << "Choose " << player_label << " type:\n1. Human\n2. Random Computer\n3. Smart AI\nChoice: ";
    int c; cin >> c;
    if (c == 2) return PlayerType::RANDOM;
    if (c == 3) return PlayerType::AI;
    return PlayerType::HUMAN;
}

Player<char>* DiamondUI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::AI) return new AIPlayer(name, symbol);
    if (type == PlayerType::RANDOM) return new RandomPlayer(name, symbol);
    return new Player<char>(name, symbol, type);
}

Player<char>** DiamondUI::setup_players() {
    Player<char>** players = new Player<char>*[2];

    string n1 = get_player_name("Player X");
    PlayerType t1 = get_player_type_choice_extended("Player X");
    players[0] = create_player(n1, 'X', t1);

    string n2 = get_player_name("Player O");
    PlayerType t2 = get_player_type_choice_extended("Player O");
    players[1] = create_player(n2, 'O', t2);

    return players;
}

Move<char>* DiamondUI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::AI) {
        AIPlayer* p = dynamic_cast<AIPlayer*>(player);
        DiamondMove m = p->get_best_move();
        return new DiamondMove(m.get_x(), m.get_y(), player->get_symbol());
    }
    if (player->get_type() == PlayerType::RANDOM) {
        RandomPlayer* p = dynamic_cast<RandomPlayer*>(player);
        DiamondMove m = p->get_random_move();
        return new DiamondMove(m.get_x(), m.get_y(), player->get_symbol());
    }

    int x, y;
    cout << player->get_name() << " (" << player->get_symbol() << ") enter move (r c): ";
    cin >> x >> y;
    return new DiamondMove(x, y, player->get_symbol());
}

void DiamondUI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\n    0   1   2   3   4\n";
    cout << "  ---------------------\n";
    for (int i = 0; i < 5; ++i) {
        cout << i << " |";
        for (int j = 0; j < 5; ++j) {
            if ((abs(i - 2) + abs(j - 2)) <= 2) {
                cout << " " << matrix[i][j] << " |";
            }
            else {
                cout << "   |";
            }
        }
        cout << "\n  ---------------------\n";
    }
    cout << endl;
}