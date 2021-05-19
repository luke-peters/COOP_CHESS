# include <iostream>
# include <tuple>
# include <string>
# include <map>
# include "interface.h"
# include "Pieces.h"
# include "Board.h"
# include <vector>
# include <cctype>
#include <sstream>


interface::interface(){
    //Initialises the map
    letters_map = {{'A', 0},{'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}};
    //Initialises the unique pointer x to point a new chess board utilising the chessboard constructor
    //which in turn creates the 8x8 array 'square' containing unique pointers to newly created pieces
    x.reset(new chessboard());
    //Sets the first player turn to white
    player_turn = Colour::white;
};

void interface::print_menu()const
{   //Prints the menu of chess rules and the extra controls available to the user
    std::cout<<"            CHESS            "<<::std::endl;
    std::cout<<"-----------------------------"<<::std::endl;
    std::cout<<"\nExtra Controls"<<::std::endl;
    std::cout<<"--------------"<<::std::endl;
    std::cout<<"\nTo view a list of the taken pieces enter: 'taken'\n"<<::std::endl;
    std::cout<<"To undo the last move made and change to previous player enter: 'undo'"<<::std::endl;
    std::cout<<"[Can only undo the previous move]\n"<<::std::endl;
    std::cout<<"To access a pieces moves list enter 'help'\n"<<::std::endl;
}

//Simple get and set functions
void interface::set_last_move(const std::string& move)
{
    last_move=move;
}
void interface::switch_player_turn()
{   
    //Switches the player turn to the opposite colour depending on which colour is the current turn
    if(player_turn==Colour::white){
        player_turn=Colour::black;
    }
    else{
        player_turn=Colour::white;
    }

}
Colour interface::get_player_turn() const
{
    return player_turn;
}

void interface::undo_move()
{   
    //
    std::cout<<last_move<<std::endl;
    //Breaks down the last_move string into the start and final moves for use in the illegal undo move
    //Note we use the second coordinates as the start and the first coordinates and the final
    //(eg- the last move is flipped around)
    std::string final_pos {last_move.substr (0,2)};
    std::string start_pos {last_move.substr (3,2)};

    //We subtract to 0 here to convert from ascii to actual values?? change this
    //We initialise in round brackets as we are not allowed to narrow in curly braces
    //Extra 1 removed from y values to allow for inputs 1-8 instead of 0-7

    //Maps algebraically notated inputted to array coordinates ranging from 0-7
    char x_start(letters_map[start_pos[0]]);
    char y_start((start_pos[1]-'0')-1);
    char x_fin(letters_map[final_pos[0]]);
    char y_fin((final_pos[1]-'0')-1);

    //Creates start and finish coordinate tuples
    std::tuple<char,char> start (x_start,y_start);
    std::tuple<char,char> fin (x_fin,y_fin);
    
    //Completes the illegal undo move
    x->illegal_undo_move(start,fin);

    //There are no sanity checks here as the last_move string is only set when a valid legal move is made
}

bool interface::is_input_format_valid(const std::string& str)const
{
    if(str.length()!=5){
        return false;
    } 
    
    if(!std::isdigit(str[1]) || !std::isdigit(str[4])){
        return false;
    }
    
    if(std::isdigit(str[0]) || std::isdigit(str[3])){
        return false;
    } 
    
    if(str[2]!=' '){
        return false;
    }
    
    if(std::islower(str[0]) || std::islower(str[3])){
        return false;
    }

    return true;
}

bool interface::input_move(std::istream& is){
    
    //Initalise a variable turn purely for ease of coding
    Colour turn{player_turn};

    //Utilises getline function to initialise a string to the istream input
    std::string str;
    std::getline (is,str);

    //Multiple if statements allowing the user to input extra controls and complete the associated functions

    //Here we return true as and undo move acts as full move for a given turn
    if(str=="undo"){
        undo_move();
        return true;
    }

    //For these extra controls we return false as these do not count as a complted players turn
    if(str=="menu"){
        print_menu();
        return false;
    }
    if(str=="taken"){
        x->print_graveyard();
        return false;
    }

    //Acts on a sanity check on the form of the inputted move: has to be (char,int,whitespace,char,int)
    if(is_input_format_valid(str)==false){
        std::cout<<"Please format your move input correctly, eg: 'C1 H6' "<<std::endl;
        return false;
    }

    
    //Checks the user has not inputted a coordinate outside  of the board
    if(str[1]-'0'>8 || str[1]-'0'<1 || str[4]-'0'<1 || str[4]-'0'>8){
        std::cout<<"Cannot make moves outside of the 8x8 board "<<std::endl;
        return false;
    }

    //Seperates the string into the start coordinates and final coordinates
    std::string start_pos {str.substr (0,2)};
    std::string final_pos {str.substr (3,2)};


    //We subtract to 0 here to convert from ascii to actual values//
    //We initialise in round brackets as we are not allowed to narrow in curly braces

    //Extra 1 removed from y values to allow for inputs 1-8 instead of 0-7
    //Maps the user input into useable coordinates within the board and piece classes (0-7)
    char x_start(letters_map[start_pos[0]]);
    char y_start((start_pos[1]-'0')-1);
    char x_fin(letters_map[final_pos[0]]);
    char y_fin((final_pos[1]-'0')-1);

    //Creates start and final coordinate tuples
    std::tuple<char,char> start (x_start,y_start);
    std::tuple<char,char> fin (x_fin,y_fin);

    //Updates the last move variable
    set_last_move(str);

    //Initialise a variable called islegal which returns true if a legal move can be made
    //Using the users input, and false if not
    bool is_legal {x->move_piece(turn,start,fin)};
    
    //If the move is technically legal but is a move into check the interface does not allow it
    //Here the interface acts as a referree
    if(x->is_check(turn)==true){
        std::cout<<"Cannot move into check"<<std::endl;
        undo_move();
        return false;
    }

    //Returns the results of the is legal variable
    return is_legal;
    
}

void interface::player_move()
{   //Initialise a variable of the player turn for ease of coding
    Colour turn{player_turn};

    //Prints out a command prompt to terminal indicating whose turn it is to move
    if(turn==Colour::white){
        std::cout<<"White's turn to move"<<std::endl;
        std::cout<<"--------------------"<<std::endl;
    }

    if(turn==Colour::black){
        std::cout<<"Black's turn to move"<<std::endl;
        std::cout<<"--------------------"<<std::endl;
    }

    //Initialises a variable to track whether the players move is finished
    bool finished_move{false};

    //While the move isn't finished ask the user to enter a move
    while (finished_move==false){
        //Prompt the user to enter a move
        std::cout<<"Enter Move: "<<std::endl;
        //Set finished move to the true only when a valid legal move not into check is inputted
        finished_move=input_move(std::cin);
    }
};

void interface::player_pawn_promotion()
{   
    //Define a variable to store the choice of promotion piece made by the user
    Piece_Type piece_choice;
    
    //Prompts the user to input a selected piece to promote to
    std::cout<<"Please enter: 'queen', 'rook', 'bishop' or 'knight to select your promotion piece"<<std::endl;

    //Uses getline to read the assign the users input to a string
    std::string str;
    std::getline (std::cin,str);

    //Acts a sanity check on the users input and ouputs an error message until a valide string is inputted
    while (str!="queen" && str!="rook" && str!="bishop" && str!="knight")
    {
        std::cout<<"You've selected an invalid promotion piece \nPlease enter: 'queen', 'rook', 'bishop' or 'knight' to select your promotion piece"<<std::endl;
        std::getline (std::cin,str);
    }
    
    //Promotes the pawn to the relevant piece type selected by the user 
    if(str=="queen"){
        x->promote_pawn(Piece_Type::queen, player_turn);
    }

    if(str=="rook"){
        x->promote_pawn(Piece_Type::rook, player_turn);
    }

    if(str=="bishop"){
        x->promote_pawn(Piece_Type::bishop, player_turn);
    }

    if(str=="knight"){
        x->promote_pawn(Piece_Type::knight, player_turn);
    }
}

void interface::play_game()
{   
    //Prints Board to start
    x->print_board();

    //While the player whose turn it is not in check allow the move sequence to start
    while(x->is_check_mate(player_turn)==false){
    // while(game_over==false){


        //Inform the player if they have been put in check
        if(x->is_check(player_turn) == true){

            std::cout<<"You are in Check"<<std::endl;
            std::cout<<"----------------"<<std::endl;
        }

        //Allow the player to move
        //This function doesn't allow a move into check so players must also move out of check
        //This function also requires a legal move to be made
        player_move();

        //Checks after a move is made if pawn promotion is needed
        if(x->is_pawn_promotion(player_turn)){
            //If pawn promotion is needed allows the player to choose piece to promote to and does this
            player_pawn_promotion();
        }

        //Prints the board following all made moves
        x->print_board();

        //Switches the turns of the player
        switch_player_turn();
    }


    //Once check mate has been achieved this outputs the winner of the game and the function ends
    std::cout<<"----------------------------------"<<std::endl;
    std::cout<<"            CHECK MATE            "<<std::endl;
    std::cout<<"----------------------------------"<<std::endl;
    std::cout<<"                                  "<<std::endl;

    if (player_turn==Colour::white){
        std::cout<<"          Black Has Won!          "<<std::endl;
    }

    if (player_turn==Colour::black){
        std::cout<<"          White Has Won!          "<<std::endl;
    }

};