//Luke Peters: 10204030
//31/05/2021

//Header file for interface class 
//This class contains methods to:
//allow a user to input moves from terminal using file and rank coordinates and also undo moves to any depth
//allow a user to select which piece they want to promot a pawn to
//tell the user if they are inputting valid moves for a piece or if they are attempting to move into check
//quit the game if checkmate is achieved and print out information regarding the game
//time each players move and stop and start chess clocks as appropriate
//print out and enumerate all possible legal moves for any board position, this also differentiates between normal, capture, check and checkmate moves
//inform user of extra controls and features available to them

# pragma once

# include "Pieces.h"
# include "Board.h"

# include <iostream>
#include <tuple>
#include <map>

class interface
{
private:
//A unique pointer to a chessboard
std::unique_ptr<chessboard>x;

//A simple boolean to check if the user has asked to quit the game
bool quit_check{false};
//A simple boolean to check if the user has asked to quit the game
bool undo_check{false};

//MOVE DATA
/////////////////////////////////////////////////
//A vector containing all previous completed moves
std::vector<std::string> completed_moves;

//Chess clock for each player in seconds
double white_chess_clock;
double black_chess_clock;

bool is_clock_on{false};

//A string containing the last move made
std::string last_move;
//A colour variable containing the colour of the current players turn
Colour player_turn;

public:
//Interface Constructor
interface();

//Add time to chess clock, move_time must be in seconds
void edit_chess_clock(const double &move_time);


//Simple Set and Get functions
void set_last_move(const std::string & move);
Colour get_player_turn() const;
//Function to switch the colour of each player's turn
void switch_player_turn();

//A map used to convert algebraically inputted x-coordinates to numbers
//(e.g: B converts to 1)
std::map<char, int> letters_map;
std::map<int, char> reverse_letters_map;

//Function that prints an introductory message
void print_intro_message()const;
//Function to print the game menu, outlining the extra controls and general game rules
void print_menu()const;
//Function to print the relecant players chess clock
void print_chess_clocks()const;
//
bool print_move_list_for_selected();
//
bool print_all_moves();

//Function to print the game menu, outlining the extra controls and general game rules
// bool print_move_list();

//Function that allows a user to undo a move from terminal
bool undo_move();

//Function to test if an input is formatted correctly
bool is_input_format_valid(const std::string& str)const;

//Function that allows the conversion of an istream to an actual move on the board
//Returns true if the move is made and false if the istream is incorrect or an illegal move is attempted
bool input_move(std::istream& is);
//Function that utilises the input move function but outputs to the terminal and forces the player to
//input some valid and legal move
void player_move();

//Function that allows a player to select the piece they would like to promote a pawn to
void player_pawn_promotion();

//Secondary interface that allows the user to get help if needed
bool helper();

//Function that utilises all of the above functions as well as functions to check for check and check mate
//In order to allow a full game of legal chess to be played by two users in the terminal
void play_game();

};