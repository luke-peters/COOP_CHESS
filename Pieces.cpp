//Luke Peters: 10204030
//31/05/2021

# include "Pieces.h"

#include <tuple>   
#include <iostream>

//Get and Set functions to access protected members of the class, all const
const char& piece::get_symbol()const
{
    return symbol;
}
const Piece_Type& piece::get_piece_type()const
{
    
    return piece_type;
    
}
const Colour& piece::get_colour()const
{
    return colour;
}
const int& piece::get_move_count()const
{
    return move_count;
}
std::string piece::get_piece_name()const
{   
    std::string piece_name;

    //Switch case function to essentially map piece_type to a string for printing
    switch(this->get_piece_type()){
        case (Piece_Type::pawn):
            piece_name="Pawn";
            break;

        case (Piece_Type::rook):
            piece_name="Rook";
            break;

        case (Piece_Type::bishop):
            piece_name="Bishop";
            break;

        case (Piece_Type::knight):
            piece_name="Knight";
            break;

        case (Piece_Type::queen):
            piece_name="Queen";
            break;

        case (Piece_Type::king):
            piece_name="King";
            break;
    }

    return piece_name;
}
const std::vector<std::tuple<char,char>>& piece::get_move_list() const
{   
    return move_list;
}
const std::vector<std::tuple<char,char>>& piece::get_capture_move_list() const
{   
    return capture_move_list;
}

//Function to change a pieces move count when it makes a move or a move is undo
void piece::edit_move_count(const int &i)
{   
    //If the piece is not a nullptr, it's move count can be altered
    if (this!=nullptr){
        move_count+=i;
    }
}


//Check that a suggested move is to an empty space or results in a capture
bool piece::check_empty_square(std::unique_ptr<piece> squares[8][8],const int& x, const int& y)const
{   
    //Is only true if the position contains a null pointer (no piece)
    if(squares[x][y] == nullptr){
        return true;
    }

    return false;
};
bool piece::check_capture_move(std::unique_ptr<piece> squares[8][8],const int& x_fin, const int& y_fin)const
{ 
    //If the position of the attempted move contains a piece, as long as it is a different colour
    //then a capture move can be achieved
    if(squares[x_fin][y_fin] != nullptr){
        if(squares[x_fin][y_fin]->get_colour() != this->get_colour()){
            return true;
        }
    }
    return false;
};


//Base class constructor, this will be utilised by inherited classes, simply requires the colour and type of piece
//being created
piece::piece(Colour colour, Piece_Type piece_type) : colour{colour},piece_type{piece_type}{};

//PAWN
//########################################################################################
//Utilises the piece constructor
pawn::pawn (Colour colour) : piece(colour,Piece_Type::pawn)
{   
    //Upper vs Lower case symbols depending upon pieces colour
    if (colour==Colour::white){
        symbol='P';
    }
    else{
        symbol='p';
    };
};
//Function to set the pawns possible move list
void pawn::set_move_list(std::unique_ptr<piece> squares[8][8], const std::tuple<char,char>&start)
{   
    //Decouples the coordinate tuple into two integers
    int x_start=std::get<0>(start);
	int y_start=std::get<1>(start);

    //Clears both movelists
    move_list.clear();
    capture_move_list.clear();

    //Create a step size of +1 or -1 in y-dir depending on black or white pawn
    int step_size;
    if(this->get_colour() == Colour::white){
        step_size=1;
    }
    else{
        step_size=-1;
    }


    //Allows a single move forward if square is free
    if(check_empty_square(squares, x_start, y_start+step_size)){
    
        move_list.push_back(std::make_tuple(x_start, y_start+step_size));


        //If this is the pawns first move allows two steps forward
        if(this->get_move_count() == 0 && check_empty_square(squares, x_start, y_start+2*step_size)){
            
            move_list.push_back(std::make_tuple(x_start,(y_start+2*step_size)));

        }
    }

    //Allows a diagonal step of 1 if it's a capture move
    //Also protects against attempting to find a move off side of the board
    for (int side_step = -1; side_step < 2; side_step++){
        if(side_step!=0){
            if(x_start+side_step<8 && x_start+side_step>-1){
                if (check_capture_move(squares, x_start+side_step, y_start+step_size)){
                    move_list.push_back(std::make_tuple(x_start+side_step,(y_start+step_size)));
                    capture_move_list.push_back(std::make_tuple(x_start+side_step,(y_start+step_size)));
                }
            }   
        } 
        
    }

};
//########################################################################################


//Sliding Pieces constructor
//########################################################################################
//Utilises the base clearclass constructor
sliding_piece::sliding_piece(Colour colour,Piece_Type piece_type) : piece(colour, piece_type){};
//Function that adds sliding moves given an initial first step, eg (squares,start,0,1) vertically upwards from start position
void sliding_piece::add_sliding_moves(std::unique_ptr<piece> squares[8][8],std::tuple<char,char>start,int x_step, int y_step)
{   
    //Decouples the coordinate tuple into integers
    int x_start{std::get<0>(start)};
	int y_start{std::get<1>(start)};


    //Add empty moves and don't allow moves off the board
    while(squares[x_start+x_step][y_start+y_step] == nullptr && y_start+y_step > -1 && x_start+x_step > -1 && y_start+y_step < 8 && x_start+x_step < 8){
        move_list.push_back(std::make_tuple(x_start+x_step,y_start+y_step));


        //Increases the x and y coordinate depending on the initial first step direction
        if(x_step>0){
            x_step+=1;
        }

        if(x_step<0){
            x_step-=1;
        }

        if(y_step>0){
            y_step+=1;
        }

        if(y_step<0){
            y_step-=1;
        }
    }

    //Once out of empty moves in the given direction check if the next step is a valid capture
    if(y_start+y_step > -1 && x_start+x_step > -1 && y_start+y_step < 8 && x_start+x_step < 8){
        if (check_capture_move(squares,x_start+x_step,y_start+y_step)){
            capture_move_list.push_back(std::make_tuple(x_start+x_step,y_start+y_step));
            move_list.push_back(std::make_tuple(x_start+x_step,y_start+y_step));
        }
    }

    //After out of empty moves or after a capture move a sliding piece can no longer go any further
    //in the selected direction (eg- no jumping over pieces)
}


//ROOK
//########################################################################################
//Rook Constructor
//Utilises the sliding piece constructor which, in turn uses the piece constructor
rook::rook(Colour colour) : sliding_piece(colour,Piece_Type::rook)
{   
    //Upper vs Lower case symbols depending upon pieces colour
    if (colour == Colour::white){
        symbol='R';
    }
    else{
        symbol='r';
    }
};
void rook::set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&start)
{   
    //Clears the rooks move and capture lists
    move_list.clear();
    capture_move_list.clear();
    //Add sliding in all of the legal directions
    add_sliding_moves(squares,start,0,1);
    add_sliding_moves(squares,start,0,-1);
    add_sliding_moves(squares,start,1,0);
    add_sliding_moves(squares,start,-1,0);

};
//########################################################################################

//BISHOP
//########################################################################################
//Utilises the sliding piece constructor which, in turn uses the piece constructor
bishop::bishop(Colour colour) : sliding_piece(colour,Piece_Type::bishop)
{   
    //Upper vs Lower case symbols depending upon pieces colour
    if (colour == Colour::white){
        symbol='B';
    }
    else{
        symbol='b';
    }
};
void bishop::set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&start)
{   
    //Clears move and capture lists
    move_list.clear();
    capture_move_list.clear();
    
    //Adds sliding moves in all of the legal directions
    add_sliding_moves(squares,start,1,1);
    add_sliding_moves(squares,start,1,-1);
    add_sliding_moves(squares,start,-1,-1);
    add_sliding_moves(squares,start,-1,1);

};
//########################################################################################


//KNIGHT
//########################################################################################
//Utilises the piece constructor
knight::knight(Colour colour) : piece(colour,Piece_Type::knight)
{   
    //Upper vs Lower case symbols depending upon pieces colour
    if (colour == Colour::white){
        symbol='N';
    }
    else{
        symbol='n';
    }
};
void knight::set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&start)
{   
    //Decouples pieces initial coordinares tuple into integeres
    int x_start=std::get<0>(start);
	int y_start=std::get<1>(start);

    //Clears move and caprute list
    move_list.clear();
    capture_move_list.clear();

    //Loops over the maximum and minimum steps in the x and y direction for the knight
    for (int x_step {-2}; x_step < 3; x_step++){
        for (int y_step {-2}; y_step < 3; y_step++){
            
            //Only add moves if the x_step * y_step=2 or -2
            //Thus only allowing legal moves for the knight, some combination of 2 and 1
            if(x_step*y_step==2 || x_step*y_step==-2){
                int x_fin{x_start+x_step};
                int y_fin{y_start+y_step};

                //Maintains that no moves outside of the board are checked
                if(x_fin>-1 && x_fin<8 && y_fin>-1 && y_fin<8){

                    //Pushes back to only move list or move and capture list based on the content of the target position
                    if (check_empty_square(squares, x_fin, y_fin)){
                        move_list.push_back(std::make_tuple(x_fin,y_fin));
                        
                    }

                    if (check_capture_move(squares, x_fin, y_fin)){
                        move_list.push_back(std::make_tuple(x_fin,y_fin));
                        capture_move_list.push_back(std::make_tuple(x_fin,y_fin));
                    }
                }
            }
        }
    }
}

//########################################################################################

//QUEEN
//########################################################################################
//Utilises the sliding piece constructor which, in turn uses the piece constructor
queen::queen(Colour colour) : sliding_piece(colour,Piece_Type::queen)
{   
    //Upper vs Lower case symbols depending upon pieces colour
    if (colour == Colour::white){
        symbol='Q';
    }
    else{
        symbol='q';
    }
};
void queen::set_move_list(std::unique_ptr<piece> squares[8][8], const std::tuple<char,char>&start)
{   //Clears move and capture lists
    move_list.clear();
    capture_move_list.clear();

    //Adds sliding moves in all legal directions for the queen
    add_sliding_moves(squares,start,0,1);
    add_sliding_moves(squares,start,0,-1);
    add_sliding_moves(squares,start,1,0);
    add_sliding_moves(squares,start,-1,0);

    add_sliding_moves(squares,start,1,1);
    add_sliding_moves(squares,start,1,-1);
    add_sliding_moves(squares,start,-1,-1);
    add_sliding_moves(squares,start,-1,1);
    
};
//########################################################################################


//KING
//########################################################################################
//Utilises the piece constructor
king::king(Colour colour) : piece(colour,Piece_Type::king)
{   
    //Upper vs Lower case symbols depending upon pieces colour
    if (colour == Colour::white){
        symbol='K';
    }
    else{
        symbol='k';
    }
};
void king::set_move_list(std::unique_ptr<piece> squares[8][8],const std::tuple<char,char>&start)
{   
    //Decouples initial coordinate tuple into integeres
    int x_start=std::get<0>(start);
	int y_start=std::get<1>(start);

    //Clears move and capture lists
    move_list.clear();
    capture_move_list.clear();

    //loops from 1 to -1 in x and y directions
    for (int x_step {-1}; x_step < 2; x_step++){
        for (int y_step {-1}; y_step < 2; y_step++){

            //Initialises the final position
            int x_fin{x_start+x_step};
            int y_fin{y_start+y_step};

            //Stops checks off of the board
            //Adds to move or move and cap depending on content of final position
            if(x_fin>-1 && x_fin<8 && y_fin>-1 && y_fin<8){
                if (check_empty_square(squares, x_fin, y_fin)){
                    move_list.push_back(std::make_tuple(x_fin,y_fin));
                    
                }

                if (check_capture_move(squares, x_fin, y_fin)){
                    move_list.push_back(std::make_tuple(x_fin,y_fin));
                    capture_move_list.push_back(std::make_tuple(x_fin,y_fin));
                }
            }
        }
    }
}
//########################################################################################