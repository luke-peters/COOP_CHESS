//Luke Peters: 10204030
//31/05/2021

//Header file containing all piece objects each with their own movelist

#pragma once

#include <tuple>   
#include <vector>
#include <memory>
//Globally defined enum type for colour
enum class Colour {none, white, black};
//Globally defined enum type for colour
enum class Piece_Type {none, pawn, rook, bishop, knight, queen, king};


//Abstract base class piece
class piece
{     
protected:
    //The pieces colour
    Colour colour{Colour::none};
    //The type of the piece
    Piece_Type piece_type{Piece_Type::none};
    //Tracks the number of moves the piece has made
    int move_count{0};
    //The pieces symbol for display on the board
    char symbol;
    //Both of these move lists are only updated when the pieces set_move list function is called
    //A vector of character tuples giving the x and y coordinates of all legal moves for the piece
    std::vector<std::tuple<char,char>> move_list{};
    //A vector of character tuples giving the x and y coordinates of all legal capture moves for the piece
    std::vector<std::tuple<char,char>> capture_move_list{};
 

public:
    //Constructor for piece
    piece(Colour colour,Piece_Type piece_type);
    //Pure Virtual destructor for piece
    // virtual ~piece(){};
    
    //Simple and get functions to return protected members
    const Piece_Type& get_piece_type()const;
    const char& get_symbol()const;
    const Colour& get_colour()const;
    const int&  get_move_count()const;
    void edit_move_count(const int&i);
    std::string get_piece_name() const;
    const std::vector<std::tuple<char,char>>& get_move_list() const;
    const std::vector<std::tuple<char,char>>& get_capture_move_list() const;
    

    //Pure virtual set move list for overriding
    virtual void set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&)=0;
    
    //Useful function inherited by all derived classes for repeat useage in set move list functions
    bool check_empty_square(std::unique_ptr<piece> squares[8][8],const int& x, const int& y)const;
    bool check_capture_move(std::unique_ptr<piece> squares[8][8],const int& x_fin, const int& y_fin)const;

  
};

//A derived abstract class called sliding piece
class sliding_piece : public piece{
protected:
    //Vitrual destructor for sliding piece
    // virtual ~sliding_piece(){};
    //Constructor for sliding piece that utilises the piece constructor
    sliding_piece(Colour colour,Piece_Type piece_type);
    //Function to add sliding moves to a derived pieces moved list
    //This is inherited by all dervied sliding pieces and utilised differentl depending on the pieces possible sliding moves 
    void add_sliding_moves(std::unique_ptr<piece> squares[8][8],std::tuple<char,char>start,int x_step, int y_step);
};



//Pawn class derived from abstract base class, piece
class pawn : public piece
{  
private:

public:
    //Pawn Constructor
    pawn(Colour colour);
    //Overriden function to set the pawns move list
    void set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&)override;
    //Pawn Destructor
    ~pawn(){};
};

//Knight class derived from abstract base class, piece
class knight : public piece
{
private:

public:
    //Constructor to create a knight
    knight(Colour colour);
    //Overriden function to set the knights move list
    void set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&)override;
    //Knight Destructor
    ~knight(){};

};

//King class derived from abstract base class, piece
class king : public piece
{
private:

public:
    //Constructor to create a king
    king(Colour colour);
    //Overriden function to set the kings move list
    void set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&)override;
    //King Destructor
    ~king(){};
};

//Rook class derived from abstract sliding piece class
class rook : public sliding_piece
{
private:

public:
    //Constructor to create a rook
    rook(Colour colour);
    //Overriden function to set the rooks move list
    void set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&)override;
    //Rook Destructor
    ~rook(){};
};

//Bishop class derived from abstract sliding piece class
class bishop : public sliding_piece
{
private:

public:
    //Constructor to create a bishop
    bishop(Colour colour);
    //Overriden function to set the bishops move list
    void set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&)override;
    //Bishop Destructor
    ~bishop(){};
};

//Queen class derived from abstract sliding piece class
class queen : public sliding_piece
{
private:

public:
    //Constructor to create a queen
    queen(Colour colour);
    //Overriden function to set the queens move list
    void set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&)override;
    //Queen Destructor
    ~queen(){};
};

