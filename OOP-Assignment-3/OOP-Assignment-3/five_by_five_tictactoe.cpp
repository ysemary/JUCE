#include "five_by_five_tictactoe.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Move Implementation
FiveByFiveMove::FiveByFiveMove(int x, int y, char symbol) : Move<char>(x, y, symbol) {}

// Board Implementation
FiveByFiveBoard::FiveByFiveBoard() : Board<char>(5, 5) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = empty;
        }
    }
}

bool FiveByFiveBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= 5 || y < 0 || y >= 5) {
        cout << "Invalid position! Use 0-4 for rows and columns.\n";
        return false;
    }

    if (board[x][y] != empty) {
        cout << "Cell already occupied! Choose another.\n";
        return false;
    }

    board[x][y] = symbol;
    n_moves++;
    return true;
}

bool FiveByFiveBoard::is_win(Player<char>* player) {
    char symbol = player->get_symbol();

    // Check horizontal
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (board[i][j] == symbol &&
                board[i][j + 1] == symbol &&
                board[i][j + 2] == symbol) {
                return true;
            }
        }
    }

    // Check vertical
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i <= 2; ++i) {
            if (board[i][j] == symbol &&
                board[i + 1][j] == symbol &&
                board[i + 2][j] == symbol) {
                return true;
            }
        }
    }

    // Check diagonal (topleft to bottomright)
    for (int i = 0; i <= 2; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (board[i][j] == symbol &&
                board[i + 1][j + 1] == symbol &&
                board[i + 2][j + 2] == symbol) {
                return true;
            }
        }
    }

    // Check diagonal (topright to bottomleft)
    for (int i = 0; i <= 2; ++i) {
        for (int j = 2; j < 5; ++j) {
            if (board[i][j] == symbol &&
                board[i + 1][j - 1] == symbol &&
                board[i + 2][j - 2] == symbol) {
                return true;
            }
        }
    }

    return false;
}

bool FiveByFiveBoard::is_lose(Player<char>* player) {
    return false;
}

bool FiveByFiveBoard::is_draw(Player<char>* player) {
    if (n_moves == 24 && !is_win(player)) {
        return true;
    }
    return false;
}

bool FiveByFiveBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

bool FiveByFiveBoard::is_cell_empty(int x, int y) {
    return (x >= 0 && x < 5 && y >= 0 && y < 5) && (board[x][y] == empty);
}

int FiveByFiveBoard::count_sequences(char symbol) {
    int count = 0;

    // Count horizontal 
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (board[i][j] == symbol &&
                board[i][j + 1] == symbol &&
                board[i][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Count vertical  
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i <= 2; ++i) {
            if (board[i][j] == symbol &&
                board[i + 1][j] == symbol &&
                board[i + 2][j] == symbol) {
                count++;
            }
        }
    }

    // Count diagonal 
    for (int i = 0; i <= 2; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (board[i][j] == symbol &&
                board[i + 1][j + 1] == symbol &&
                board[i + 2][j + 2] == symbol) {
                count++;
            }
        }
    }

    for (int i = 0; i <= 2; ++i) {
        for (int j = 2; j < 5; ++j) {
            if (board[i][j] == symbol &&
                board[i + 1][j - 1] == symbol &&
                board[i + 2][j - 2] == symbol) {
                count++;
            }
        }
    }

    return count;
}

// Player Implementation
FiveByFivePlayer::FiveByFivePlayer(string name, char symbol, PlayerType type)
    : Player<char>(name, symbol, type) {}

// UI Implementation
FiveByFiveUI::FiveByFiveUI() : UI<char>("5x5 Tic Tac Toe - Get the most 3-in-a-row sequences!", 3) {}

Move<char>* FiveByFiveUI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        return get_human_move(player);
    }
    else {
        return get_computer_move(player);
    }
}

Player<char>* FiveByFiveUI::create_player(string& name, char symbol, PlayerType type) {
    return new FiveByFivePlayer(name, symbol, type);
}

void FiveByFiveUI::display_five_by_five_board(const vector<vector<char>>& matrix) const {
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
            cout << setw(cell_width) << matrix[i][j] << " |";
        }
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    cout << endl;
}

Move<char>* FiveByFiveUI::get_human_move(Player<char>* player) {
    int x, y;
    FiveByFiveBoard* boardPtr = dynamic_cast<FiveByFiveBoard*>(player->get_board_ptr());

    cout << player->get_name() << "'s turn (" << player->get_symbol() << ").\n";

    while (true) {
        cout << "Enter row (0-4) and column (0-4): ";
        cin >> x >> y;

        if (x >= 0 && x < 5 && y >= 0 && y < 5) {
            if (boardPtr->is_cell_empty(x, y)) {
                break;
            }
            else {
                cout << "Cell already occupied! Choose another position.\n";
            }
        }
        else {
            cout << "Invalid position! Please enter row and column between 0-4.\n";
        }
    }

    return new FiveByFiveMove(x, y, player->get_symbol());
}

Move<char>* FiveByFiveUI::get_computer_move(Player<char>* player) {
    FiveByFiveBoard* boardPtr = dynamic_cast<FiveByFiveBoard*>(player->get_board_ptr());
    auto matrix = boardPtr->get_board_matrix();
    char mySymbol = player->get_symbol();
    char oppSymbol = (mySymbol == 'X') ? 'O' : 'X';

    struct Pick { int x = -1, y = -1, score = -1000; };
    Pick best;

    auto within = [](int r, int c) { return r >= 0 && r < 5 && c >= 0 && c < 5; };
    vector<pair<int, int>> dirs = { {1,0},{0,1},{1,1},{1,-1} };

    auto line_score = [&](int sx, int sy, int dx, int dy, char symbol_place, bool defensive) {
        int selfCount = 0, oppCount = 0, emptyCount = 0;
        for (int k = 0; k < 3; ++k) {
            int r = sx + k * dx, c = sy + k * dy;
            char cell = matrix[r][c];
            if (cell == '.') emptyCount++;
            else if (cell == symbol_place) selfCount++;
            else oppCount++;
        }
        if (defensive) {
            if (oppCount == 2 && emptyCount == 1) return 7; // block an immediate threat
            if (oppCount == 1 && emptyCount == 2) return 3;
            return 0;
        }
        if (selfCount == 3) return 12;           // immediate win
        if (selfCount == 2 && emptyCount == 1) return 6;
        if (selfCount == 1 && emptyCount == 2) return 2;
        return 0;
    };

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (!boardPtr->is_cell_empty(i, j)) continue;

            int score = 0;
            // evaluate every line of length 3 that passes through (i,j)
            for (size_t dirIdx = 0; dirIdx < dirs.size(); ++dirIdx) {
                int dx = dirs[dirIdx].first;
                int dy = dirs[dirIdx].second;
                for (int offset = 0; offset < 3; ++offset) {
                    int sx = i - offset * dx;
                    int sy = j - offset * dy;
                    int ex = sx + 2 * dx;
                    int ey = sy + 2 * dy;
                    if (!within(sx, sy) || !within(ex, ey)) continue;

                    // temporarily place my symbol for scoring
                    char original = matrix[i][j];
                    matrix[i][j] = mySymbol;
                    score += line_score(sx, sy, dx, dy, mySymbol, false);
                    matrix[i][j] = original;

                    // defensive look: assume opponent owns the spot
                    score += line_score(sx, sy, dx, dy, oppSymbol, true);
                }
            }

            if (score > best.score) {
                best = { i, j, score };
            }
        }
    }

    if (best.x == -1) {
        return new FiveByFiveMove(0, 0, player->get_symbol());
    }

    cout << "Computer chooses position (" << best.x << ", " << best.y << ")\n";
    return new FiveByFiveMove(best.x, best.y, player->get_symbol());
}

// Game runner 
void run_five_by_five_tictactoe() {
    FiveByFiveBoard* board = new FiveByFiveBoard();
    FiveByFiveUI* ui = new FiveByFiveUI();

    Player<char>** players = ui->setup_players();

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    // After game ends, count sequences for both players
    FiveByFiveBoard* fiveBoard = dynamic_cast<FiveByFiveBoard*>(board);
    int x_sequences = fiveBoard->count_sequences('X');
    int o_sequences = fiveBoard->count_sequences('O');

    cout << "\n=== FINAL SCORE ===" << endl;
    cout << players[0]->get_name() << " (X): " << x_sequences << " sequences" << endl;
    cout << players[1]->get_name() << " (O): " << o_sequences << " sequences" << endl;

    if (x_sequences > o_sequences) {
        cout << players[0]->get_name() << " wins with more sequences!" << endl;
    }
    else if (o_sequences > x_sequences) {
        cout << players[1]->get_name() << " wins with more sequences!" << endl;
    }
    else {
        cout << "It's a tie!" << endl;
    }

    // Cleanup
    delete players[0];
    delete players[1];
    delete[] players;
    delete ui;
    delete board;
}