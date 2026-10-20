#include <iostream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <utility>
#include <tuple>
#include <cstdlib>  // Added for rand()
#include "MisèreTicTacToe_Classes.h"

using namespace std;

MisereTicTacToe_Board::MisereTicTacToe_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool MisereTicTacToe_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) {
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool MisereTicTacToe_Board::has_three_in_row(char symbol) {
    for (int i = 0; i < rows; ++i) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
            return true;
    }

    for (int i = 0; i < columns; ++i) {
        if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)
            return true;
    }

    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
        return true;

    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
        return true;

    return false;
}

bool MisereTicTacToe_Board::is_win(Player<char>* player) {
    char player_symbol = player->get_symbol();
    char opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';
    return has_three_in_row(opponent_symbol);
}

bool MisereTicTacToe_Board::is_lose(Player<char>* player) {
    char player_symbol = player->get_symbol();
    return has_three_in_row(player_symbol);
}

bool MisereTicTacToe_Board::is_draw(Player<char>* player) {
    if (n_moves == 9) {
        char player_symbol = player->get_symbol();
        char opponent_symbol = (player_symbol == 'X') ? 'O' : 'X';
        return !has_three_in_row(player_symbol) && !has_three_in_row(opponent_symbol);
    }
    return false;
}

bool MisereTicTacToe_Board::game_is_over(Player<char>* player) {
    return is_lose(player) || is_win(player) || n_moves == 9;
}

MisereTicTacToe_UI::MisereTicTacToe_UI() : UI<char>("Welcome to FCAI Inverse X-O Game (Misère Tic-Tac-Toe) by Dr El-Ramly", 3) {}

Player<char>* MisereTicTacToe_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

// Fixed: Now a member function of MisereTicTacToe_UI
bool MisereTicTacToe_UI::createsThreeInRow(const vector<vector<char>>& board, int x, int y, char symbol) {
    vector<vector<char>> tempBoard = board;
    tempBoard[x][y] = symbol;

    for (int i = 0; i < 3; i++) {
        if (tempBoard[i][0] == symbol && tempBoard[i][1] == symbol && tempBoard[i][2] == symbol)
            return true;
    }

    for (int i = 0; i < 3; i++) {
        if (tempBoard[0][i] == symbol && tempBoard[1][i] == symbol && tempBoard[2][i] == symbol)
            return true;
    }

    if (tempBoard[0][0] == symbol && tempBoard[1][1] == symbol && tempBoard[2][2] == symbol)
        return true;

    if (tempBoard[0][2] == symbol && tempBoard[1][1] == symbol && tempBoard[2][0] == symbol)
        return true;

    return false;
}

// Fixed: Now a member function of MisereTicTacToe_UI
bool MisereTicTacToe_UI::forcesOpponentThree(const vector<vector<char>>& board, int x, int y, char aiSymbol, char opponentSymbol) {
    vector<vector<char>> tempBoard = board;
    tempBoard[x][y] = aiSymbol;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tempBoard[i][j] == ' ') {
                tempBoard[i][j] = opponentSymbol;
                if (createsThreeInRow(tempBoard, i, j, opponentSymbol)) {
                    return true;
                }
                tempBoard[i][j] = ' ';
            }
        }
    }
    return false;
}

// Fixed: Now a member function of MisereTicTacToe_UI
bool MisereTicTacToe_UI::isCenter(int x, int y) {
    return x == 1 && y == 1;
}

// Fixed: Now a member function of MisereTicTacToe_UI
bool MisereTicTacToe_UI::isCorner(int x, int y) {
    return (x == 0 && y == 0) || (x == 0 && y == 2) ||
        (x == 2 && y == 0) || (x == 2 && y == 2);
}

// Fixed: Now a member function of MisereTicTacToe_UI
pair<int, int> MisereTicTacToe_UI::getBestMove(const vector<vector<char>>& board, char aiSymbol, char opponentSymbol) {
    vector<pair<int, int>> emptyCells;
    vector<pair<int, int>> safeMoves;
    vector<pair<int, int>> goodMoves;
    vector<pair<int, int>> forcingMoves;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {  // Changed from '.' to ' '
                emptyCells.push_back({ i, j });
            }
        }
    }

    if (emptyCells.size() == 1) {
        return emptyCells[0];
    }

    for (const auto& cell : emptyCells) {
        int x = cell.first;
        int y = cell.second;

        if (forcesOpponentThree(board, x, y, aiSymbol, opponentSymbol)) {
            forcingMoves.push_back(cell);
            continue;
        }

        if (createsThreeInRow(board, x, y, aiSymbol)) {
            continue;
        }

        safeMoves.push_back(cell);

        if (isCenter(x, y) || isCorner(x, y)) {
            goodMoves.push_back(cell);
        }
    }

    if (!forcingMoves.empty()) {
        int idx = rand() % forcingMoves.size();
        return forcingMoves[idx];
    }

    if (!goodMoves.empty()) {
        for (const auto& move : goodMoves) {
            if (isCenter(move.first, move.second)) {
                return move;
            }
        }
        int idx = rand() % goodMoves.size();
        return goodMoves[idx];
    }

    if (!safeMoves.empty()) {
        int idx = rand() % safeMoves.size();
        return safeMoves[idx];
    }

    if (!emptyCells.empty()) {
        int idx = rand() % emptyCells.size();
        return emptyCells[idx];
    }

    return { rand() % 3, rand() % 3 };
}

// Added: Missing implementation
void MisereTicTacToe_UI::display_game_state(Move<char>* move, Board<char>* board,
    Player<char>* current_player, Player<char>* next_player) {

    cout << "\n=== Game State ===" << endl;

    if (move) {
        cout << "Last move: " << current_player->get_name()
            << " placed " << current_player->get_symbol()
            << " at (" << move->get_x() << ", " << move->get_y() << ")" << endl;
    }

    cout << "Current player: " << current_player->get_name()
        << " (" << current_player->get_symbol() << ")" << endl;
    cout << "Next player: " << next_player->get_name()
        << " (" << next_player->get_symbol() << ")" << endl;

    // Display board
    display_board_matrix(board->get_board_matrix());

    cout << "==================\n" << endl;
}

Move<char>* MisereTicTacToe_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlayer " << player->get_name() << " (" << player->get_symbol()
            << "), enter your move (x y) from 0 to 2: ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        Board<char>* boardPtr = player->get_board_ptr();
        if (boardPtr) {
            vector<vector<char>> boardState = boardPtr->get_board_matrix();
            char aiSymbol = player->get_symbol();
            char opponentSymbol = (aiSymbol == 'X') ? 'O' : 'X';

            // Fixed: Now calling member function
            pair<int, int> bestMove = getBestMove(boardState, aiSymbol, opponentSymbol);
            x = bestMove.first;
            y = bestMove.second;

            cout << "\nComputer player " << player->get_name() << " (" << player->get_symbol()
                << ") chooses position (" << x << ", " << y << ")\n";
        }
        else {
            x = rand() % 3;
            y = rand() % 3;
            cout << "\nComputer player " << player->get_name() << " (" << player->get_symbol()
                << ") chooses position (" << x << ", " << y << ")\n";
        }
    }
    return new Move<char>(x, y, player->get_symbol());
}