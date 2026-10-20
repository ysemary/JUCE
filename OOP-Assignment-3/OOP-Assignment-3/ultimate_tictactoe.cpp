#include "ultimate_tictactoe.h"
#include <iostream>
#include <iomanip>

using namespace std;

// UltimateMove Implementation
UltimateMove::UltimateMove(int mainRow, int mainCol, int subRow, int subCol, char symbol)
    : Move<char>(subRow, subCol, symbol), mainRow(mainRow), mainCol(mainCol),
    subRow(subRow), subCol(subCol) {
}

int UltimateMove::get_main_row() const { return mainRow; }
int UltimateMove::get_main_col() const { return mainCol; }
int UltimateMove::get_sub_row() const { return subRow; }
int UltimateMove::get_sub_col() const { return subCol; }

// SubBoard Implementation
SubBoard::SubBoard() : Board<char>(3, 3), winner(' ') {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = ' ';
        }
    }
}

bool SubBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        return false;
    }

    if (board[x][y] != ' ') {
        return false;
    }

    board[x][y] = symbol;
    n_moves++;
    return true;
}

bool SubBoard::is_win(Player<char>* player) {
    char symbol = player->get_symbol();

    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) {
            winner = symbol;
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol) {
            winner = symbol;
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) {
        winner = symbol;
        return true;
    }
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) {
        winner = symbol;
        return true;
    }

    return false;
}

bool SubBoard::is_lose(Player<char>* player) {
    return false;
}

bool SubBoard::is_draw(Player<char>* player) {
    if (n_moves == 9 && !is_win(player)) {
        winner = 'D';
        return true;
    }
    return false;
}

bool SubBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

char SubBoard::get_winner() const {
    return winner;
}

void SubBoard::set_winner(char w) {
    winner = w;
}

bool SubBoard::is_full() const {
    return n_moves == 9;
}

// UltimateBoard Implementation
UltimateBoard::UltimateBoard() : Board<char>(3, 3), nextMainRow(-1), nextMainCol(-1) {
    mainBoard = vector<vector<char>>(3, vector<char>(3, ' '));

    subBoards.resize(3);
    for (int i = 0; i < 3; ++i) {
        subBoards[i].resize(3);
        for (int j = 0; j < 3; ++j) {
            subBoards[i][j] = new SubBoard();
        }
    }
}

UltimateBoard::~UltimateBoard() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            delete subBoards[i][j];
        }
    }
}

bool UltimateBoard::update_board(Move<char>* move) {
    UltimateMove* ultimateMove = dynamic_cast<UltimateMove*>(move);
    if (!ultimateMove) return false;

    int mainRow = ultimateMove->get_main_row();
    int mainCol = ultimateMove->get_main_col();
    int subRow = ultimateMove->get_sub_row();
    int subCol = ultimateMove->get_sub_col();
    char symbol = ultimateMove->get_symbol();

    if (!is_valid_move(mainRow, mainCol, subRow, subCol)) {
        return false;
    }

    SubBoard* subBoard = subBoards[mainRow][mainCol];
    Move<char>* subMove = new Move<char>(subRow, subCol, symbol);

    if (!subBoard->update_board(subMove)) {
        delete subMove;
        return false;
    }
    delete subMove;

    Player<char> tempPlayer("temp", symbol, PlayerType::HUMAN);
    if (subBoard->is_win(&tempPlayer)) {
        mainBoard[mainRow][mainCol] = symbol;
    }
    else if (subBoard->is_draw(&tempPlayer)) {
        mainBoard[mainRow][mainCol] = 'D';
    }

    if (mainBoard[subRow][subCol] != ' ') {
        nextMainRow = -1;
        nextMainCol = -1;
    }
    else {
        nextMainRow = subRow;
        nextMainCol = subCol;
    }

    n_moves++;
    return true;
}

bool UltimateBoard::is_valid_move(int mainRow, int mainCol, int subRow, int subCol) {
    if (mainRow < 0 || mainRow >= 3 || mainCol < 0 || mainCol >= 3 ||
        subRow < 0 || subRow >= 3 || subCol < 0 || subCol >= 3) {
        return false;
    }

    if (mainBoard[mainRow][mainCol] != ' ') {
        return false;
    }

    if (nextMainRow != -1 && nextMainCol != -1) {
        if (mainRow != nextMainRow || mainCol != nextMainCol) {
            return false;
        }
    }

    SubBoard* subBoard = subBoards[mainRow][mainCol];
    return subBoard->get_board_matrix()[subRow][subCol] == ' ';
}

bool UltimateBoard::is_win(Player<char>* player) {
    char symbol = player->get_symbol();

    for (int i = 0; i < 3; ++i) {
        if (mainBoard[i][0] == symbol && mainBoard[i][1] == symbol && mainBoard[i][2] == symbol) {
            return true;
        }
    }

    for (int j = 0; j < 3; ++j) {
        if (mainBoard[0][j] == symbol && mainBoard[1][j] == symbol && mainBoard[2][j] == symbol) {
            return true;
        }
    }

    if (mainBoard[0][0] == symbol && mainBoard[1][1] == symbol && mainBoard[2][2] == symbol) {
        return true;
    }
    if (mainBoard[0][2] == symbol && mainBoard[1][1] == symbol && mainBoard[2][0] == symbol) {
        return true;
    }

    return false;
}

bool UltimateBoard::is_lose(Player<char>* player) {
    return false;
}

bool UltimateBoard::is_draw(Player<char>* player) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (mainBoard[i][j] == ' ') {
                return false;
            }
        }
    }
    return !is_win(player);
}

bool UltimateBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

void UltimateBoard::set_next_board(int row, int col) {
    nextMainRow = row;
    nextMainCol = col;
}

SubBoard* UltimateBoard::get_sub_board(int row, int col) {
    if (row >= 0 && row < 3 && col >= 0 && col < 3) {
        return subBoards[row][col];
    }
    return nullptr;
}

char UltimateBoard::get_main_cell(int row, int col) {
    if (row >= 0 && row < 3 && col >= 0 && col < 3) {
        return mainBoard[row][col];
    }
    return ' ';
}

bool UltimateBoard::is_sub_board_active(int row, int col) const {
    if (nextMainRow == -1 && nextMainCol == -1) return true;
    return (row == nextMainRow && col == nextMainCol);
}

void UltimateBoard::display_full_board() const {
    cout << "\n=== ULTIMATE TIC TAC TOE ===\n\n";

    for (int mainRow = 0; mainRow < 3; ++mainRow) {
        for (int subRow = 0; subRow < 3; ++subRow) {
            cout << " ";
            for (int mainCol = 0; mainCol < 3; ++mainCol) {
                for (int subCol = 0; subCol < 3; ++subCol) {
                    char cell = subBoards[mainRow][mainCol]->get_board_matrix()[subRow][subCol];
                    cout << (cell == ' ' ? '.' : cell) << " ";
                }
                if (mainCol < 2) cout << "| ";
            }
            cout << endl;
        }
        if (mainRow < 2) {
            cout << " " << string(29, '-') << endl;
        }
    }

    cout << "\nMain Board Status:\n";
    for (int i = 0; i < 3; ++i) {
        cout << " ";
        for (int j = 0; j < 3; ++j) {
            char status = mainBoard[i][j];
            if (status == ' ') {
                cout << (is_sub_board_active(i, j) ? "+" : ".");
            }
            else if (status == 'D') {
                cout << "D";
            }
            else {
                cout << status;
            }
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// UltimatePlayer Implementation
UltimatePlayer::UltimatePlayer(string name, char symbol, PlayerType type)
    : Player<char>(name, symbol, type) {}

// UltimateUI Implementation 
UltimateUI::UltimateUI() : UI<char>("Ultimate Tic Tac Toe - player vs player", 1) {}

Move<char>* UltimateUI::get_move(Player<char>* player) {
    return get_human_move(player);
}

Player<char>* UltimateUI::create_player(string& name, char symbol, PlayerType type) {
    return new UltimatePlayer(name, symbol, type);
}

void UltimateUI::display_current_status(UltimateBoard* board, Player<char>* player) {
    cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";

    UltimateBoard* ultimateBoard = dynamic_cast<UltimateBoard*>(board);
    bool anyBoard = true;
    int nextRow = -1, nextCol = -1;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (ultimateBoard->is_sub_board_active(i, j)) {
                anyBoard = false;
                nextRow = i;
                nextCol = j;
                break;
            }
        }
        if (!anyBoard) break;
    }

    if (anyBoard) {
        cout << "You can play on any available board!\n";
    }
    else {
        cout << "You must play in board (" << nextRow << ", " << nextCol << ")\n";
    }
}

Move<char>* UltimateUI::get_human_move(Player<char>* player) {
    UltimateBoard* boardPtr = dynamic_cast<UltimateBoard*>(player->get_board_ptr());

    display_current_status(boardPtr, player);
    boardPtr->display_full_board();

    int mainRow, mainCol, subRow, subCol;

    while (true) {
        cout << "Enter main board position:\n";
        cout << "  Row (0-2): ";
        cin >> mainRow;
        cout << "  Column (0-2): ";
        cin >> mainCol;

        cout << "Enter position within that board:\n";
        cout << "  Row (0-2): ";
        cin >> subRow;
        cout << "  Column (0-2): ";
        cin >> subCol;

        if (boardPtr->is_valid_move(mainRow, mainCol, subRow, subCol)) {
            break;
        }
        else {
            cout << "\nInvalid move! Please try again.\n";
        }
    }

    cout << "\nMove accepted: Board (" << mainRow << ", " << mainCol
        << ") Position (" << subRow << ", " << subCol << ")\n";

    return new UltimateMove(mainRow, mainCol, subRow, subCol, player->get_symbol());
}

// Game runner
void run_ultimate_tictactoe() {
    cout << "=== ULTIMATE TIC TAC TOE ===\n";
    cout << "Rules:\n";
    cout << "1. 9 small boards in a 3x3 grid\n";
    cout << "2. Win a small board by getting 3 in a row\n";
    cout << "3. Win the game by getting 3 small boards in a row\n";
    cout << "4. Your next board is determined by opponent's last move\n";
    cout << "================================\n\n";

    UltimateBoard* board = new UltimateBoard();
    UltimateUI* ui = new UltimateUI();

    Player<char>** players = ui->setup_players();

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    board->display_full_board();

    delete players[0];
    delete players[1];
    delete[] players;
    delete ui;
    delete board;
}