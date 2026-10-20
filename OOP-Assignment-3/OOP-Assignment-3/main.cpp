#include <iostream>
#include "Numerical Tic-Tac-Toe.h"
#include "BoardGame_Classes.h"
#include "five_by_five_tictactoe.h"
#include "word_tictactoe.h"
#include "SUS_Classes.h"
#include "Four_in_a_row_Classes.h"
#include "DiamondTicTacToe_Classes.h"
#include "Obstacles_classes.h"
#include "Infinity_Classes.h"
#include "ultimate_tictactoe.h"
#include "memory_tictactoe.h"
#include "MisèreTicTacToe_Classes.h"
#include "PyramidClasses.h"
#include "FourByFourClasses.h"

using namespace std;

void playSUSGame() {
    cout << "Starting S-U-S Sequence Game...\n" << endl;
    SUS_UI* ui = new SUS_UI();
    SUS_Board* board = new SUS_Board();
    Player<char>** players = ui->setup_players();
    string player1_name = players[0]->get_name();
    string player2_name = players[1]->get_name();
    PlayerType player1_type = players[0]->get_type();
    PlayerType player2_type = players[1]->get_type();
    delete players[0];
    delete players[1];
    delete[] players;
    players = new Player<char>*[2];
    players[0] = ui->create_player(player1_name, 'S', player1_type);
    players[1] = ui->create_player(player2_name, 'U', player2_type);
    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);
    GameManager<char> gameManager(board, players, ui);
    gameManager.run();
    ui->display_results(board, players[0], players[1]);
    delete players[0];
    delete players[1];
    delete[] players;
    delete board;
    delete ui;
}

void playFourInARowGame() {
    cout << "Starting Four-in-a-Row Game...\n" << endl;


    FourInARow_UI* ui = new FourInARow_UI();
    FourInARow_Board* board = new FourInARow_Board();


    Player<char>** players = ui->setup_players();


    string player1_name = players[0]->get_name();
    string player2_name = players[1]->get_name();
    PlayerType player1_type = players[0]->get_type();
    PlayerType player2_type = players[1]->get_type();


    delete players[0];
    delete players[1];
    delete[] players;


    players = new Player<char>*[2];
    players[0] = ui->create_player(player1_name, 'X', player1_type);
    players[1] = ui->create_player(player2_name, 'O', player2_type);


    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);


    ui->display_connect_four_board(board->get_board_matrix());


    GameManager<char> gameManager(board, players, ui);
    gameManager.run();


    ui->display_results(board, players[0], players[1]);

    delete players[0];
    delete players[1];
    delete[] players;
    delete board;
    delete ui;
}

void run_misere_tictactoe() {
    MisereTicTacToe_UI* ui = new MisereTicTacToe_UI();
    MisereTicTacToe_Board* board = new MisereTicTacToe_Board();
    Player<char>** players = ui->setup_players();
    string name1 = players[0]->get_name();
    string name2 = players[1]->get_name();
    PlayerType t1 = players[0]->get_type();
    PlayerType t2 = players[1]->get_type();

    delete players[0];
    delete players[1];
    delete[] players;

    players = new Player<char>*[2];
    players[0] = ui->create_player(name1, 'X', t1);
    players[1] = ui->create_player(name2, 'O', t2);
    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    GameManager<char> game(board, players, ui);
    game.run();

    bool p1_lost = board->is_lose(players[0]);
    bool p2_lost = board->is_lose(players[1]);
    bool p1_won = board->is_win(players[0]);
    bool p2_won = board->is_win(players[1]);
    bool is_draw = board->is_draw(players[0]);

    cout << "\n================================================" << endl;
    cout << "  GAME RESULTS:" << endl;
    cout << "================================================\n" << endl;

    if (p1_lost) {
        cout << name1 << " (" << players[0]->get_symbol() << ") made THREE-IN-A-ROW and LOSES!" << endl;
        cout << "WINNER: " << name2 << " (" << players[1]->get_symbol() << ")!" << endl;
    }
    else if (p2_lost) {
        cout << name2 << " (" << players[1]->get_symbol() << ") made THREE-IN-A-ROW and LOSES!" << endl;
        cout << "WINNER: " << name1 << " (" << players[0]->get_symbol() << ")!" << endl;
    }
    else if (p1_won) {
        cout << name1 << " (" << players[0]->get_symbol() << ") WINS by forcing opponent to make three-in-a-row!" << endl;
    }
    else if (p2_won) {
        cout << name2 << " (" << players[1]->get_symbol() << ") WINS by forcing opponent to make three-in-a-row!" << endl;
    }
    else if (is_draw) {
        cout << "It's a DRAW! Board is full with no three-in-a-row." << endl;
    }
    cout << "================================================\n" << endl;

    delete players[0];
    delete players[1];
    delete[] players;
    delete board;
    delete ui;
}
void playDiamondTicTacToe() {
    cout << "Starting Diamond Tic-Tac-Toe Game...\n" << endl;

    DiamondUI* ui = new DiamondUI();
    DiamondBoard* board = new DiamondBoard();

    Player<char>** players = ui->setup_players();
    string player1_name = players[0]->get_name();
    string player2_name = players[1]->get_name();
    PlayerType player1_type = players[0]->get_type();
    PlayerType player2_type = players[1]->get_type();

    delete players[0];
    delete players[1];
    delete[] players;

    players = new Player<char>*[2];
    players[0] = ui->create_player(player1_name, 'X', player1_type);
    players[1] = ui->create_player(player2_name, 'O', player2_type);

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    // Display initial board
    ui->display_board_matrix(board->get_board_matrix());

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    // Display final results
    cout << "\n================================================" << endl;
    cout << "  GAME RESULTS:" << endl;
    cout << "================================================\n" << endl;

    if (board->is_win(players[0])) {
        cout << player1_name << " (" << players[0]->get_symbol() << ") WINS!" << endl;
    }
    else if (board->is_win(players[1])) {
        cout << player2_name << " (" << players[1]->get_symbol() << ") WINS!" << endl;
    }
    else if (board->is_draw(players[0])) {
        cout << "It's a DRAW!" << endl;
    }
    cout << "================================================\n" << endl;

    delete players[0];
    delete players[1];
    delete[] players;
    delete board;
    delete ui;
}

void playObstaclesGame() {
    cout << "Starting Obstacles Tic-Tac-Toe Game...\n" << endl;

    Obstacles_UI* ui = new Obstacles_UI();
    Obstacles_Board* board = new Obstacles_Board();

    Player<char>** players = ui->setup_players();
    string player1_name = players[0]->get_name();
    string player2_name = players[1]->get_name();
    PlayerType player1_type = players[0]->get_type();
    PlayerType player2_type = players[1]->get_type();

    delete players[0];
    delete players[1];
    delete[] players;

    players = new Player<char>*[2];
    players[0] = ui->create_player(player1_name, 'X', player1_type);
    players[1] = ui->create_player(player2_name, 'O', player2_type);

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    // Display initial board
    ui->display_obstacles_board(board->get_board_matrix(), board->get_obstacles());

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    ui->display_results(board, players[0], players[1]);

    delete players[0];
    delete players[1];
    delete[] players;
    delete board;
    delete ui;
}

void playInfinityGame() {
    cout << "Starting Infinity Tic-Tac-Toe Game...\n" << endl;

    Infinity_UI* ui = new Infinity_UI();
    Infinity_Board* board = new Infinity_Board();

    Player<char>** players = ui->setup_players();
    string player1_name = players[0]->get_name();
    string player2_name = players[1]->get_name();
    PlayerType player1_type = players[0]->get_type();
    PlayerType player2_type = players[1]->get_type();

    delete players[0];
    delete players[1];
    delete[] players;

    players = new Player<char>*[2];
    players[0] = ui->create_player(player1_name, 'X', player1_type);
    players[1] = ui->create_player(player2_name, 'O', player2_type);

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    // Display initial board
    ui->display_board_matrix(board->get_board_matrix());

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    ui->display_results(board, players[0], players[1]);

    delete players[0];
    delete players[1];
    delete[] players;
    delete board;
    delete ui;
}
void playFourByFourGame() {
    cout << "Starting 4x4 Tic Tac Toe Game...\n" << endl;

    FourByFourUI* ui = new FourByFourUI();
    FourByFourBoard* board = new FourByFourBoard();

    Player<char>** players = ui->setup_players_with_ai();
    string player1_name = players[0]->get_name();
    string player2_name = players[1]->get_name();
    PlayerType player1_type = players[0]->get_type();
    PlayerType player2_type = players[1]->get_type();

    delete players[0];
    delete players[1];
    delete[] players;

    players = new Player<char>*[2];
    players[0] = ui->create_player(player1_name, 'X', player1_type);
    players[1] = ui->create_player(player2_name, 'O', player2_type);

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    // Display initial board
    ui->display_board_matrix(board->get_board_matrix());

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    // Display results
    bool p1_win = board->is_win(players[0]);
    bool p2_win = board->is_win(players[1]);
    bool is_draw = board->is_draw(players[0]);

    cout << "\n================================================" << endl;
    cout << "  4x4 TIC TAC TOE - GAME RESULTS:" << endl;
    cout << "================================================\n" << endl;

    if (p1_win) {
        cout << "CONGRATULATIONS! " << player1_name << " (" << players[0]->get_symbol()
            << ") WINS by getting three tokens in a row!" << endl;
    }
    else if (p2_win) {
        cout << "CONGRATULATIONS! " << player2_name << " (" << players[1]->get_symbol()
            << ") WINS by getting three tokens in a row!" << endl;
    }
    else if (is_draw) {
        cout << "It's a DRAW! No more valid moves available." << endl;
    }
    cout << "================================================\n" << endl;

    delete players[0];
    delete players[1];
    delete[] players;
    delete board;
    delete ui;
}

void playPyramidGame() {
    cout << "Starting Pyramid Tic Tac Toe Game...\n" << endl;

    PyramidUI* ui = new PyramidUI();
    PyramidBoard* board = new PyramidBoard();

    // CHANGE THIS LINE: Use setup_players_with_ai() instead of setup_players()
    Player<char>** players = ui->setup_players_with_ai();

    // Rest of the function remains the same...
    string player1_name = players[0]->get_name();
    string player2_name = players[1]->get_name();
    PlayerType player1_type = players[0]->get_type();
    PlayerType player2_type = players[1]->get_type();

    delete players[0];
    delete players[1];
    delete[] players;

    players = new Player<char>*[2];
    players[0] = ui->create_player(player1_name, 'X', player1_type);
    players[1] = ui->create_player(player2_name, 'O', player2_type);

    players[0]->set_board_ptr(board);
    players[1]->set_board_ptr(board);

    // Display initial board
    ui->display_board_matrix(board->get_board_matrix());

    GameManager<char> gameManager(board, players, ui);
    gameManager.run();

    // Display results
    bool p1_win = board->is_win(players[0]);
    bool p2_win = board->is_win(players[1]);
    bool is_draw = board->is_draw(players[0]);

    cout << "\n================================================" << endl;
    cout << "  PYRAMID TIC TAC TOE - GAME RESULTS:" << endl;
    cout << "================================================\n" << endl;

    if (p1_win) {
        cout << "CONGRATULATIONS! " << player1_name << " (" << players[0]->get_symbol()
            << ") WINS by getting three symbols in a row in the pyramid!" << endl;
    }
    else if (p2_win) {
        cout << "CONGRATULATIONS! " << player2_name << " (" << players[1]->get_symbol()
            << ") WINS by getting three symbols in a row in the pyramid!" << endl;
    }
    else if (is_draw) {
        cout << "It's a DRAW! The pyramid is full with no winner." << endl;
    }
    cout << "================================================\n" << endl;

    delete players[0];
    delete players[1];
    delete[] players;
    delete board;
    delete ui;
}

int main() {
    while (true) {
        cout << "Pick a game from the list below (1 - 13):" << "\n";
        cout << "Game 1: SUS" << "\n";
        cout << "Game 2: Four-in-a-row" << "\n";
        cout << "Game 3: 5 x 5 Tic-Tac-Toe" << "\n";
        cout << "Game 4: Word Tic-Tac-Toe" << "\n";
        cout << "Game 5: Misere Tic-Tac-Toe" << "\n";
        cout << "Game 6: Diamond Tic-Tac-Toe" << "\n";
        cout << "Game 7: 4 x 4 Tic-Tac-Toe" << "\n";
        cout << "Game 8: Pyramid Tic-Tac-Toe" << "\n";
        cout << "Game 9: Numerical Tic-Tac-Toe" << "\n";
        cout << "Game 10: Obstacles Tic-Tac-Toe" << "\n";
        cout << "Game 11: Infinity Tic-Tac-Toe" << "\n";
        cout << "Game 12: Ultimate Tic-Tac-Toe" << "\n";
        cout << "Game 13: Memory Tic-Tac-Toe" << "\n";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            playSUSGame();
            break;
        case 2:
            playFourInARowGame();
            break;
        case 3:
            run_five_by_five_tictactoe();
            break;
        case 4:
            run_word_tictactoe();
            break;
        case 5:
            run_misere_tictactoe();
            break;
        case 6:
            playDiamondTicTacToe();
            break;
        case 7:
            playFourByFourGame();
            break;
        case 8:
            playPyramidGame();
            break;
        case 9:
            run_numerical_tictactoe();
            break;
        case 10:
            playObstaclesGame();
            break;
        case 11:
            playInfinityGame();
            break;
        case 12:
            run_ultimate_tictactoe();
            break;
        case 13:
            run_memory_tictactoe();
            break;
        default:
            cout << "Invalid choice! Please select 1-13." << "\n";
            break;
        }

        char exit;
        cout << "Do you want to exit? (y / n): ";
        cin >> exit;
        if (exit == 'y' || exit == 'Y') break;
    }

    return 0;
}