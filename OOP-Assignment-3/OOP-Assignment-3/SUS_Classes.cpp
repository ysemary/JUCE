#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <random>
#include "SUS_Classes.h"

using namespace std;

SUS_Board::SUS_Board() : Board(3, 3) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = blank_symbol;
        }
    }
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= 3 || y < 0 || y >= 3) && board[x][y] == blank_symbol) {
        board[x][y] = mark;
        n_moves++;
        check_sequences(x, y, mark);
        return true;
    }
    return false;
}

bool SUS_Board::is_win(Player<char>* player) {
    char player_symbol = player->get_symbol();
    char opponent_symbol = (player_symbol == 'S') ? 'U' : 'S';
    return get_player_sequences(player_symbol) > get_player_sequences(opponent_symbol);
}

bool SUS_Board::is_lose(Player<char>* player) {
    char player_symbol = player->get_symbol();
    char opponent_symbol = (player_symbol == 'S') ? 'U' : 'S';
    return get_player_sequences(player_symbol) < get_player_sequences(opponent_symbol);
}

bool SUS_Board::is_draw(Player<char>* player) {
    return (n_moves == 9) && (get_player_sequences('S') == get_player_sequences('U'));
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return n_moves == 9;
}

int SUS_Board::get_player_sequences(char player_symbol) const {
    if (player_symbol == 'S') {
        return player1_sequences.size();
    }
    else {
        return player2_sequences.size();
    }
}

vector<Sequence> SUS_Board::get_player_sequences_list(char player_symbol) const {
    if (player_symbol == 'S') {
        return player1_sequences;
    }
    else {
        return player2_sequences;
    }
}

void SUS_Board::check_sequences(int x, int y, char symbol) {
    vector<vector<pair<int, int>>> patterns = {
        // Rows
        {{0,0}, {0,1}, {0,2}}, {{1,0}, {1,1}, {1,2}}, {{2,0}, {2,1}, {2,2}},
        // Columns  
        {{0,0}, {1,0}, {2,0}}, {{0,1}, {1,1}, {2,1}}, {{0,2}, {1,2}, {2,2}},
        // Diagonals
        {{0,0}, {1,1}, {2,2}}, {{0,2}, {1,1}, {2,0}}
    };

    for (const auto& pattern : patterns) {
        if (board[pattern[0].first][pattern[0].second] == 'S' &&
            board[pattern[1].first][pattern[1].second] == 'U' &&
            board[pattern[2].first][pattern[2].second] == 'S') {

            Sequence seq{ pattern[0], pattern[1], pattern[2], symbol };
            vector<Sequence>& target_list = (symbol == 'S') ? player1_sequences : player2_sequences;

            if (!is_sequence_duplicate(seq, target_list)) {
                target_list.push_back(seq);
            }
        }
    }
}

bool SUS_Board::is_valid_sequence(const vector<pair<int, int>>& seq, char symbol) const {
    if (seq.size() != 3) return false;

    return (board[seq[0].first][seq[0].second] == 'S' &&
        board[seq[1].first][seq[1].second] == 'U' &&
        board[seq[2].first][seq[2].second] == 'S');
}

bool SUS_Board::is_sequence_duplicate(const Sequence& seq, const vector<Sequence>& sequences_list) const {
    for (const auto& existing_seq : sequences_list) {
        if (existing_seq.pos1 == seq.pos1 &&
            existing_seq.pos2 == seq.pos2 &&
            existing_seq.pos3 == seq.pos3) {
            return true;
        }
    }
    return false;
}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer") << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}
Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << " (" << player->get_symbol() << "), enter your move (row and column 0-2): ";
        cin >> x >> y;

        while (x < 0 || x > 2 || y < 0 || y > 2) {
            cout << "Invalid move! Please enter row and column between 0 and 2: ";
            cin >> x >> y;
        }
    }
    else {
        Board<char>* game_board = player->get_board_ptr();
        auto board_matrix = game_board->get_board_matrix();
        vector<pair<int, int>> empty_cells;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board_matrix[i][j] == '.') {
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

        cout << "\nComputer player " << player->get_name() << " chooses position (" << x << ", " << y << ")\n";
    }

    return new Move<char>(x, y, player->get_symbol());
}
void SUS_UI::display_status(SUS_Board* board, Player<char>* player1, Player<char>* player2) {
    cout << "\n=== Game Status ===" << endl;
    cout << player1->get_name() << " (S): " << board->get_player_sequences('S') << " sequences" << endl;
    cout << player2->get_name() << " (U): " << board->get_player_sequences('U') << " sequences" << endl;
    cout << "Moves played: " << board->get_n_moves() << "/9" << endl;
}

void SUS_UI::display_results(SUS_Board* board, Player<char>* player1, Player<char>* player2) {
    int player1_score = board->get_player_sequences('S');
    int player2_score = board->get_player_sequences('U');

    cout << "\n=== Final Results ===" << endl;
    cout << player1->get_name() << " (S): " << player1_score << " sequences" << endl;
    cout << player2->get_name() << " (U): " << player2_score << " sequences" << endl;

    if (player1_score > player2_score) {
        cout << player1->get_name() << " wins!" << endl;
    }
    else if (player2_score > player1_score) {
        cout << player2->get_name() << " wins!" << endl;
    }
    else {
        cout << "It's a draw!" << endl;
    }

    cout << "\nSequences completed by " << player1->get_name() << ":" << endl;
    auto sequences1 = board->get_player_sequences_list('S');
    for (const auto& seq : sequences1) {
        cout << "S-U-S at positions: (" << seq.pos1.first << "," << seq.pos1.second << "), " << "(" << seq.pos2.first << "," << seq.pos2.second << "), " << "(" << seq.pos3.first << "," << seq.pos3.second << ")" << endl;
    }

    cout << "\nSequences completed by " << player2->get_name() << ":" << endl;
    auto sequences2 = board->get_player_sequences_list('U');
    for (const auto& seq : sequences2) {
        cout << "S-U-S at positions: (" << seq.pos1.first << "," << seq.pos1.second << "), " << "(" << seq.pos2.first << "," << seq.pos2.second << "), " << "(" << seq.pos3.first << "," << seq.pos3.second << ")" << endl;
    }
}