# include<iostream>
# include "Board.h"
# include <algorithm>
# include <memory>
#include <iomanip>
#include <tuple>   

//NO elegant way of doing this
//All uncalled squares already have a nullptr
void chessboard::set_start_board() 
{	
	//Resets unique_ptr at relevant coordinates on the board array
	//to point at a newly created relevant piece

	//Sets white non pawn pieces
	squares[0][0].reset(new rook	{Colour::white});
	squares[1][0].reset(new knight	{Colour::white});
	squares[2][0].reset(new bishop	{Colour::white});
	squares[3][0].reset(new queen	{Colour::white});
	squares[4][0].reset(new king	{Colour::white});
	squares[5][0].reset(new bishop	{Colour::white});
	squares[6][0].reset(new knight	{Colour::white});
	squares[7][0].reset(new rook	{Colour::white});

	//Sets white non pawn pieces
	squares[0][7].reset(new rook	{Colour::black});
	squares[1][7].reset(new knight	{Colour::black});
	squares[2][7].reset(new bishop	{Colour::black});
	squares[3][7].reset(new queen	{Colour::black});
	squares[4][7].reset(new king	{Colour::black});
	squares[5][7].reset(new bishop	{Colour::black});
	squares[6][7].reset(new knight	{Colour::black});
	squares[7][7].reset(new rook	{Colour::black});

	//Sets all pawns
	for (int x = 0; x < 8; x++) {
		squares[x][1].reset(new pawn{Colour::white});
		squares[x][6].reset(new pawn{Colour::black});
	}
	
};

//Gets symbol at position on board
//Differs from the get symbol function in piece class as it handles nullptrs
//For use in the print board function
char chessboard::get_symbol_board(int x, int y)const
{   
	//If a piece present at the coordinate return it's symbol
    if (squares[x][y] != nullptr) {
        return squares[x][y]->get_symbol();

	//If null ptr return a whitespace symbol
    } else {
        return ' ';
    }
}


void chessboard::print_graveyard() const
{	
	std::cout<<"PIECE GRAVEYARD"<<std::endl;
	//For each piece in the graveyard, print it's symbol
	for (unsigned i=0; i<graveyard.size(); i++){
		std::cout<<graveyard.at(i)->get_symbol()<<std::endl;
	}
}

void chessboard::print_board() const
{	

	//Uses get symbol on board function to display the pieces being pointed to at
	//each coordinate in the board array
	std::cout<<"   A   B   C   D   E   F   G   H"<<std::endl;
	for (int y {7}; y > -1; y--) {
		std::cout<<"  --------------------------------"<<std::endl;
		
		for (int x {0}; x < 8; x++) {
			if (x==0){
				std::cout<<y+1<<"| "<<this->get_symbol_board(x,y)<<" ";
			}
			else{
				std::cout<<"| "<<this->get_symbol_board(x,y)<<" ";
			}
			
		};
		std::cout<<"|"<<std::endl;
	};
	std::cout<<"  --------------------------------"<<std::endl;

	std::cout<<"(Enter 'menu' to see menu)\n"<<std::endl;
};

//Moves a piece on the board legally
bool chessboard::move_piece(const Colour& turn, const std::tuple<char, char>& start, const std::tuple<char, char>& fin)
{	
	//Convert tuple of coordinates to seperate integers for ease of coding
	int x_start=std::get<0>(start);
	int y_start=std::get<1>(start);
	int x_fin=std::get<0>(fin);
	int y_fin=std::get<1>(fin); 

	//If not trying to move an empty space or wrong colour
	if(squares[x_start][y_start] != nullptr && squares[x_start][y_start]->get_colour() == turn){

		//Sets the movelist for the selected piece and position
		squares[x_start][y_start]->set_move_list(squares,start);

		//Creates temporary vector using get_move_list
		std::vector<std::tuple<char,char>> all_moves=squares[x_start][y_start]->get_move_list();

		//Checks that the inputted move is in the selected pieces legal move list
		if (std::find(all_moves.begin(), all_moves.end(), fin) != all_moves.end()){

			//If move is legal moves the unique ptr to piece to the final position
			std::swap(squares[x_start][y_start],squares[x_fin][y_fin]);

			//Puts unique ptr to piece in the graveyard if it has just been taken
			if(squares[x_start][y_start]!=nullptr){
				graveyard.push_back(std::move(squares[x_start][y_start]));
				//Sets last move captrue to true
				was_last_move_capture=true;
			}
			else{
				//If move wasn't a capture sets was last move capture to false
				was_last_move_capture=false;
			}


			//Resets the now empty square to a nullptr
			squares[x_start][y_start].reset();

			//Increase the moved pieces move counter by 1
			squares[x_fin][y_fin]->edit_move_count(1);
			
			//Returns true indicating a move has actually been completed
			return true;
		}

		else{

			//If the inputted move coordinate is not in the selected pieces move list returns
			//An error message indicating the move is illegal for the selected piece, and outputs it's name
			std::cout<<"This is an illegal move for the " << squares[x_start][y_start]->get_piece_name()<<std::endl;

			//Returns false as no move was actually made
			return false;
		}
	
	}

	else{
		//If the user selects a nullptr or the wrong colour, outputs an error message indicating thise
		std::cout<<"Please select your own piece"<<std::endl;

		//returns false as no move was actuall made
		return false;
	}

	//Returns false by default
	return false;
};

//Moves a piece on the board Illegally, used for undoing moves (e.g-pawns going backwards)
//It takes the initial and final positions in the same sense as the move_piece function 
//So the coordinate tuples inputted into move piece should be reversed when attempting to undo the move
void chessboard::illegal_undo_move(const std::tuple<char, char>& start, const std::tuple<char, char>& fin)
{
	//Convert tuples to integers for ease of coding
	int x_start=std::get<0>(start);
	int y_start=std::get<1>(start);
	int x_fin=std::get<0>(fin);
	int y_fin=std::get<1>(fin);
	
	//If the last move was a promotion
	if(was_last_move_promotion==true){
		//Deletes the newly created promotion piece
		squares[x_start][y_start].reset();
		//Moves the newly promoted pawn into it's position
		squares[x_start][y_start]=std::move(promoted_pawns.back());
		//Deletes the now empty vector element
		promoted_pawns.pop_back();
	}

	//Swaps the unique ptrs to piece at the start and infal position
	std::swap(squares[x_start][y_start],squares[x_fin][y_fin]);

	//As this is an undo type move, it reduces the selected pieces move count by 1
	//This is important for undoing a pawns first move, as it must still be allowed to move two steps afterwards
	squares[x_fin][y_fin]->edit_move_count(-1);

	//If last move was a capture we replace the now empty square with the most recently capture pieced at the end of the graveyard vector
	//and then delete the now nullptr in the graveyard vector
	if(was_last_move_capture==true){
		squares[x_start][y_start]=std::move(graveyard.back());
		graveyard.pop_back();
	}

}


const std::tuple<char,char> chessboard::get_king_position(const Colour& turn)const
{	//Loops over every coordinate on the board array
	for (int x{0}; x < 8; x++){
		for (int y{0}; y < 8; y++){
			//If a coorindate contains a unique pointer to a piece
			if(squares[x][y] != nullptr){
				//If the coordinate contatains a unique pointer to a king of the inputted colour
				if(squares[x][y]->get_piece_type() == Piece_Type::king && squares[x][y]->get_colour()==turn){
					
					//Create and return a coordinate tuple of the kings position
					std::tuple<char,char> king_position{x,y};

					return(king_position);
				}
			}
		}
	}

	//Otherwise return a tuple of some inaccessible coordinate
	//This return should never be met as we always have a king on the board??
	return (std::tuple<char,char> {-1,-1});
}

bool chessboard::is_check(const Colour& turn)
{
	//Creates a coordinate tuple containing the selected colours king position
	std::tuple<char,char> king_position{this->get_king_position(turn)};

	//Loops over every coordinate in the board array
	for (int x{0}; x < 8; x++){
		for (int y{0}; y < 8; y++){

			//If a coordinate contains a unique pointer to a piece of the opposite colour
			if(squares[x][y]!=nullptr && squares[x][y]->get_colour()!=turn){
				
				//Creates a coordinate tuple of this opponent pieces position
				std::tuple<char,char> start{x,y};
				//Sets the legal move lists for the opponent piece 
				squares[x][y]->set_move_list(squares,start);
				//Initialises a vector contating the opponent pieces legal moves
				std::vector<std::tuple<char,char>> selected_opp_moves{squares[x][y]->get_move_list()};

				//If the kings coordinate tuple is one of the oppoonent pieces legal moves then it can be taken
				//hence the king is currently in check and so returns true
				if (std::find(selected_opp_moves.begin(), selected_opp_moves.end(), king_position) != selected_opp_moves.end()){
					return true;
				}

			}
		}
	}
	//If the kings position is not on any opposing pieces legal move list it is not in check so return false
	return false;
}

bool chessboard::is_check_mate(const Colour& turn)
{
	//Requires King is in check before checking for check mate
	if(this->is_check(turn)==true){
		//Loops over all coordinates in board array
		for (int x{0}; x < 8; x++){
			for (int y{0}; y < 8; y++){

				//If position contains unique ptr to piece of same colour we are checking check mate for
				if(squares[x][y]!=nullptr && squares[x][y]->get_colour()==turn){
					//Creates a coordinate tuple 
					std::tuple<char,char> start{x,y};
					//Sets the movelist for the piece being pointed to at the board coordinate
					squares[x][y]->set_move_list(squares,start);
					//Initialises a vector to store all the possible moves for this piece
					std::vector<std::tuple<char,char>> possible_moves{squares[x][y]->get_move_list()};
					//Loops over every possible move in the newly initalisedpossible moves vector
					for (int i=0; i < possible_moves.size(); i++) {
						//Make the possible move for the selected piece
    					this->move_piece(turn,start,possible_moves[i]);
						
						//Now the move has been made check if the King is still in check
						if(this->is_check(turn)!=true){
							//Undo the move
							illegal_undo_move(possible_moves[i],start);
							//If the king isn't in check anymore then there exists a move
							//that takes the king out of check so is checkmate returns false
							return false;
						}

						//Undo the move
						illegal_undo_move(possible_moves[i],start);
					}
				}
			}
		}

		//If there are no moves by the selected colour that take the selected colours king out of check
		//then the king is in checkmate and is checkmate returns true
		return true;

	}

	//Return false if the king isn't in check in the first place
	return false;
	
}

bool chessboard::is_pawn_promotion(const Colour& turn)
{	
	//Initialise an integer the y coordinate of search for a promotion pawn
	int y_pawn_pos;
	
	//The y coordinate "search row" depends on the colour
	//If white the 7th row is searched 
	//If black the 0th row is searched
	if(turn==Colour::white){
		y_pawn_pos=7;
	}

	if(turn==Colour::black){
		y_pawn_pos=0;
	}
	
	//Loops over all x coordinates/ board collumns
	for (int x{0}; x < 8; x++){
		//If there is a piece
		if(squares[x][y_pawn_pos] != nullptr){
			//If the piece type is pawn (we can assume it's the correct colour as pawns can only achieve end rows by travelling forward)
			if (squares[x][y_pawn_pos]->get_piece_type() == Piece_Type::pawn){
				//Set was last move promotion to true and return true
				was_last_move_promotion=true;
				//Essentially, a pawn has been found somewhere on the end rows of the board array, so a promition must occur
				return true;
			}
		}

	}

	//If no pawns present on end rows of the board array, then no promotion needs to happen
	was_last_move_promotion=false;	
	return false;
	
}

void chessboard::promote_pawn(const Piece_Type piece_choice, const Colour& turn)
{	
	//Defines integers to store the "promotion pawn"'s position
	int x_pawn_pos;
	int y_pawn_pos;

	//Once again initialises the y coordinate depending on the pawns colour
	if(turn==Colour::white){
		y_pawn_pos=7;
	}
	if(turn==Colour::black){
		y_pawn_pos=0;
	}

	//Loops over all x_coordinates on the board
	for (int x{0}; x < 8; x++){
		//If there is a piece present
		if(squares[x][7] != nullptr){
			//If it is also a pawn
			if (squares[x][y_pawn_pos]->get_piece_type() == Piece_Type::pawn){
				//Set the x coordinate of the pawn to this value
				x_pawn_pos=x;
			}
		
		}		
	}

	//Push back the unique pointer to the promition pawn to the promoted pawns vector
	promoted_pawns.push_back(std::move(squares[x_pawn_pos][y_pawn_pos]));


	//Reset the empty square left by the promoted pawn to a new piece of the inputted type
	if(piece_choice==Piece_Type::queen){
		squares [x_pawn_pos][y_pawn_pos].reset(new queen {turn});
	}

	if(piece_choice==Piece_Type::rook){
		squares[x_pawn_pos][y_pawn_pos].reset(new rook {turn});
	}

	if(piece_choice==Piece_Type::bishop){
		squares[x_pawn_pos][y_pawn_pos].reset(new bishop {turn});
	}

	if(piece_choice==Piece_Type::knight){
		squares[x_pawn_pos][y_pawn_pos].reset(new knight {turn});
	}

}