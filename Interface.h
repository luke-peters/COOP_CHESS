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
//A string containing the last move made
std::string last_move;
//A colour variable containing the colour of the current players turn
Colour player_turn;


public:
//Interface Constructor
interface();

//Simple Set and Get functions
void set_last_move(const std::string & move);
Colour get_player_turn() const;
//Function to switch the colour of each player's turn
void switch_player_turn();

//A map used to convert algebraically inputted x-coordinates to numbers
//(e.g: B converts to 1)
std::map<char, int> letters_map;

//Function to print the game menu, outlining the extra controls and general game rules
void print_menu()const;
//Function that allows a user to undo a move from terminal
void undo_move();

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

//Function that utilises all of the above functions as well as functions to check for check and check mate
//In order to allow a full game of legal chess to be played by two users in the terminal
void play_game();

};