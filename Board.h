# include <memory>
# include "Pieces.h"
#include <algorithm> 
# pragma once
#include <tuple>   

//Class called chessboard
class chessboard
{
private:
    //Set up board as 8x8 array of unique pointers to abstract base class piece
    //All unique pointers are nullptrs by default
    std::unique_ptr<piece> squares[8][8];
    //A vector of unique ptrs to base class piece, to store taken pieces
    std::vector<std::unique_ptr<piece>> graveyard{};
    //A vector of unique ptrs to base class piece, to store promoted pawns
    std::vector<std::unique_ptr<piece>> promoted_pawns{};

    //Void function to assign the unique ptrs contained in the board array to new pieces
    void set_start_board();

    //Tracks whether the last move made was a capture
    bool was_last_move_capture;
    //Tracks whether the last move made was a pawn promotion
    bool was_last_move_promotion;


public:

    //Chessboard Constructor which utilises the set start board function
    //to fill the squares array with unique pointers to the relevant pieces
    chessboard(){this->set_start_board();};

    //Chessboard Destructor
    ~chessboard(){};

    //Function to return the coordinate tuple of the selected colours's king position
    const std::tuple<char,char> get_king_position(const Colour& turn) const;

    //Function to return the symbol of the piece being pointed to at the chosen board coordinate
    char get_symbol_board(int x, int y) const;

    //Function to print the board array 'squares' to the terminal with the symbols of pieces
    //being pointed to at each coordinate
    void print_board() const;

    //Function to print the taken pieces to the terimnal
    void print_graveyard() const;

    //Function to move a unique pointer to a pieces within the board array
    //These moves are limited to legal ones and allow for captures
    //Function returns true if a move was made and false otherwise
    //Takes in two tuples, the start x and y and final x and y
    //Also takes in the colour that should be being moved 
    bool move_piece(const Colour& turn, const std::tuple<char, char>& start, const std::tuple<char, char>& fin);

    //Function to perform an illegal move, utilised in an undo move function when a pawn may have to move backwards
    //Or a piece must be returned from the graveyard or promoted pawns vector
    void illegal_undo_move(const std::tuple<char, char>& start,const std::tuple<char, char>& fin);

    //Function that checks if the selected colours king is currently in check
    bool is_check(const Colour &turn);

    //Function that checks if the selected colours king is currently in check mate
    bool is_check_mate(const Colour &turn);

    //Function that checks if any of the selected colours pawns are valid for promotion
    bool is_pawn_promotion(const Colour & turn);

    //Function that promotes a pawn of the inputted colour to the inputted piece type
    //Should only be called when is pawn promotion returns true
    void promote_pawn(const Piece_Type piece_choice,const Colour& turn);

};