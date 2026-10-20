#include "word_tictactoe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>

using namespace std;

// WordMove Implementation
WordMove::WordMove(int x, int y, char symbol) : Move<char>(x, y, symbol) {}

// WordBoard Implementation
WordBoard::WordBoard() : Board<char>(3, 3) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = empty;
        }
    }
    load_dictionary();
}

void WordBoard::load_dictionary() {
    ifstream file("dic.txt");
    string word;

    if (!file.is_open()) {
        cout << "Warning: Could not open dic.txt file. Using default words.\n";
        dictionary = { "CAT", "DOG", "SUN", "CAR", "BOX", "TEA", "BED", "EGG" };
        return;
    }

    while (file >> word) {
        if (word.length() == 3) {
            transform(word.begin(), word.end(), word.begin(), ::toupper);
            dictionary.insert(word);
        }
    }
    file.close();
    cout << "Loaded " << dictionary.size() << " valid 3-letter words.\n";
}

bool WordBoard::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        cout << "Invalid position! Use 0-2 for rows and columns.\n";
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

bool WordBoard::is_valid_word(const string& word) {
    string upper_word = word;
    transform(upper_word.begin(), upper_word.end(), upper_word.begin(), ::toupper);
    return dictionary.find(upper_word) != dictionary.end();
}

bool WordBoard::is_win(Player<char>* player) {
    // Check rows
    for (int i = 0; i < 3; i++) {
        string word = "";
        word += board[i][0];
        word += board[i][1];
        word += board[i][2];

        if (word[0] != empty && word[1] != empty && word[2] != empty) {
            if (is_valid_word(word)) {
                cout << "Valid word found in row " << i << ": " << word << endl;
                return true;
            }
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        string word = "";
        word += board[0][j];
        word += board[1][j];
        word += board[2][j];

        if (word[0] != empty && word[1] != empty && word[2] != empty) {
            if (is_valid_word(word)) {
                cout << "Valid word found in column " << j << ": " << word << endl;
                return true;
            }
        }
    }

    // Check diagonals
    string diag1 = "";
    diag1 += board[0][0];
    diag1 += board[1][1];
    diag1 += board[2][2];

    if (diag1[0] != empty && diag1[1] != empty && diag1[2] != empty) {
        if (is_valid_word(diag1)) {
            cout << "Valid word found in main diagonal: " << diag1 << endl;
            return true;
        }
    }

    string diag2 = "";
    diag2 += board[0][2];
    diag2 += board[1][1];
    diag2 += board[2][0];

    if (diag2[0] != empty && diag2[1] != empty && diag2[2] != empty) {
        if (is_valid_word(diag2)) {
            cout << "Valid word found in anti-diagonal: " << diag2 << endl;
            return true;
        }
    }

    return false;
}

bool WordBoard::is_lose(Player<char>* player) {
    return false;
}

bool WordBoard::is_draw(Player<char>* player) {
    if (n_moves == 9 && !is_win(player)) {
        return true;
    }
    return false;
}

bool WordBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

bool WordBoard::is_cell_empty(int x, int y) {
    return (x >= 0 && x < 3 && y >= 0 && y < 3) && (board[x][y] == empty);
}

void WordBoard::show_sample_words() {
    cout << "Sample valid 3-letter words: ";
    int count = 0;
    for (const auto& word : dictionary) {
        if (count < 5) {
            cout << word << " ";
            count++;
        }
        else {
            break;
        }
    }
    size_t remaining = (dictionary.size() > 5) ? (dictionary.size() - 5) : 0;
    cout << "... and " << remaining << " more!\n";
}

// WordPlayer Implementation
WordPlayer::WordPlayer(string name, char symbol, PlayerType type)
    : Player<char>(name, symbol, type) {}

// WordUI Implementation
WordUI::WordUI() : UI<char>("Word Tic-Tac-Toe - Form valid 3-letter words!", 3) {}

Move<char>* WordUI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        return get_human_move(player);
    }
    else {
        return get_computer_move(player);
    }
}

Player<char>* WordUI::create_player(string& name, char symbol, PlayerType type) {
    return new WordPlayer(name, symbol, type);
}

Move<char>* WordUI::get_human_move(Player<char>* player) {
    int x, y;
    char letter;
    WordBoard* boardPtr = dynamic_cast<WordBoard*>(player->get_board_ptr());

    cout << player->get_name() << "'s turn.\n";

    while (true) {
        cout << "Enter row (0-2), column (0-2), and letter: ";
        cin >> x >> y >> letter;

        letter = toupper(letter);

        if (x >= 0 && x < 3 && y >= 0 && y < 3) {
            if (boardPtr->is_cell_empty(x, y)) {
                if (isalpha(letter)) {
                    break;
                }
                else {
                    cout << "Invalid letter! Please enter A-Z.\n";
                }
            }
            else {
                cout << "Cell already occupied! Choose another position.\n";
            }
        }
        else {
            cout << "Invalid position! Please enter row and column between 0-2.\n";
        }
    }

    return new WordMove(x, y, letter);
}

Move<char>* WordUI::get_computer_move(Player<char>* player) {
    WordBoard* boardPtr = dynamic_cast<WordBoard*>(player->get_board_ptr());
    auto matrix = boardPtr->get_board_matrix();
    const auto& dictionary = boardPtr->get_dictionary();

    struct Candidate {
        int x = -1, y = -1;
        char letter = 'A';
        int score = -1;
    } best;

    // Precompute all 3-cell lines on the board
    vector<array<pair<int, int>, 3>> lines = {
        // rows
        {{{0, 0}, {0, 1}, {0, 2}}},
        {{{1, 0}, {1, 1}, {1, 2}}},
        {{{2, 0}, {2, 1}, {2, 2}}},
        // columns
        {{{0, 0}, {1, 0}, {2, 0}}},
        {{{0, 1}, {1, 1}, {2, 1}}},
        {{{0, 2}, {1, 2}, {2, 2}}},
        // diagonals
        {{{0, 0}, {1, 1}, {2, 2}}},
        {{{0, 2}, {1, 1}, {2, 0}}}
    };

    auto evaluate_line = [&](const array<pair<int, int>, 3>& line) {
        string current(3, '.');
        for (int i = 0; i < 3; ++i) {
            char cell = matrix[line[i].first][line[i].second];
            current[i] = (cell == ' ') ? '.' : cell;
        }

        for (int empty_idx = 0; empty_idx < 3; ++empty_idx) {
            if (current[empty_idx] != '.') continue;

            for (const auto& word : dictionary) {
                bool fits = true;
                for (int k = 0; k < 3 && fits; ++k) {
                    if (current[k] != '.' && current[k] != word[k]) {
                        fits = false;
                    }
                }
                if (!fits) continue;

                // Score: prioritize immediate wins (all three filled), then setups
                int filled_now = 1; // the empty cell we will fill
                for (int k = 0; k < 3; ++k) {
                    if (current[k] != '.') filled_now++;
                }

                int score = 0;
                if (filled_now == 3) score = 20;          // creates a word right away
                else if (filled_now == 2) score = 8;      // sets up a 2-letter chain
                else score = 3;                           // just a small improvement

                if (score > best.score) {
                    best.score = score;
                    best.x = line[empty_idx].first;
                    best.y = line[empty_idx].second;
                    best.letter = word[empty_idx];
                }
            }
        }
    };

    for (const auto& line : lines) {
        evaluate_line(line);
    }

    if (best.score == -1) {
        // fallback: pick center if empty, otherwise first available spot with a vowel
        vector<pair<int, int>> preferred = { {1, 1}, {0, 0}, {0, 2}, {2, 0}, {2, 2} };
        char vowels[] = { 'A', 'E', 'I', 'O', 'U' };
        for (auto pos : preferred) {
            if (boardPtr->is_cell_empty(pos.first, pos.second)) {
                cout << "Computer chooses position (" << pos.first << ", " << pos.second
                    << ") with letter " << vowels[rand() % 5] << "\n";
                return new WordMove(pos.first, pos.second, vowels[rand() % 5]);
            }
        }
        // absolute fallback
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (boardPtr->is_cell_empty(i, j)) {
                    return new WordMove(i, j, 'A');
                }
            }
        }
    }

    cout << "Computer chooses position (" << best.x << ", " << best.y
        << ") with letter " << best.letter << "\n";
    return new WordMove(best.x, best.y, best.letter);
}

void WordUI::display_instructions() {
    cout << "\n=== HOW TO PLAY ===\n";
    cout << "- Place letters on a 3x3 grid\n";
    cout << "- Form valid 3-letter words horizontally, vertically, or diagonally\n";
    cout << "- First player to form a valid word wins!\n";
    cout << "- Game ends in draw if board fills with no valid words\n";
    cout << "===================\n\n";
}

// Game runner implementation
void run_word_tictactoe() {
    WordBoard* board = new WordBoard();
    WordUI* ui = new WordUI();

    ui->display_instructions();
    board->show_sample_words();

    Player<char>** players = ui->setup_players();

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    delete players[0];
    delete players[1];
    delete[] players;
    delete ui;
    delete board;
}