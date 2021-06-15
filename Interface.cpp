//Luke Peters: 10204030
//31/05/2021

# include "interface.h"
# include "Pieces.h"
# include "Board.h"

# include <iostream>
# include <tuple>
# include <string>
# include <map>
# include <vector>
# include <cctype>
# include <sstream>
# include <chrono>
# include <iomanip>
# include <algorithm>


interface::interface(){
    //Initialises the map
    letters_map = {{'A', 0},{'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}};
    reverse_letters_map = {{0, 'A'},{1, 'B'}, {2, 'C'}, {3, 'D'}, {4, 'E'}, {5, 'F'}, {6, 'G'}, {7, 'H'}};
    //Initialises the unique pointer x to point a new chess board utilising the chessboard constructor
    //which in turn creates the 8x8 array 'square' containing unique pointers to newly created pieces
    x.reset(new chessboard());
    //Sets the first player turn to white
    player_turn = Colour::white;
};

void interface::print_intro_message()const
{
    std::cout<<"\nWELCOME TO C++ OOP CHESS"<<std::endl;
    std::cout<<"--------------------------"<<std::endl;
    std::cout<<"Created by Luke Peters (Student ID: 10204030)\n"<<std::endl;
    std::cout<<"1.You will not be allowed to make illegal chess moves"<<std::endl;
    std::cout<<"2.You will be informed if your are placed in check"<<std::endl;
    std::cout<<"3.You will not be allowed to make a move that results in you being in check"<<std::endl;
    std::cout<<"4.The game will end when checkmate is detected"<<std::endl;
    std::cout<<"5.Pawn promotion is supported\n"<<std::endl;
    std::cout<<"6.Undo moves are supported\n"<<std::endl;
    std::cout<<"7.You can get advice on your move using the built in helper\n"<<std::endl;
    std::cout<<"8.There are extra features available such as chess clocks\n"<<std::endl;

    std::cout<<"Extras"<<std::endl;

    
    std::cout<<"ENTER:   'menu'   DURING YOUR TURN TO VIEW EXTRA CONTROLS\n"<<std::endl;
    std::cout<<"ENTER:   'help'   DURING YOUR TURN TO ACCESS THE HELPER\n"<<std::endl;

    std::string start_check;
    std::cout<<"To start the game enter: 'start'"<<std::endl;
    std::getline(std::cin,start_check);

    while(start_check!="start"){
        std::cout<<"You must enter 'start'"<<std::endl;
        std::getline(std::cin,start_check);
    }
    //Prints Board to start
    x->print_board();

}

void interface::print_menu()const
{   //Prints the menu of chess rules and the extra controls available to the user
    std::cout<<"           CHESS  MENU            "<<::std::endl;
    std::cout<<"----------------------------------"<<::std::endl;
    std::cout<<"\nExtra Controls during gameplay"<<::std::endl;
    std::cout<<"-------------------------------"<<::std::endl;
    std::cout<<"\nTo print the board at any time enter: 'print'\n"<<::std::endl;
    std::cout<<"\nTo view a list of the taken pieces enter: 'taken'\n"<<::std::endl;
    std::cout<<"To undo the previous move enter: 'undo'"<<::std::endl;
    std::cout<<"(You can undo to the start of the game)"<<::std::endl;
    std::cout<<"\nTo view each players running game clock after each move enter: 'clock on' or 'clock off'"<<::std::endl;
    std::cout<<"(Undo moves will not take time off of a players clock)"<<std::endl;
    std::cout<<"\nTo get help with your move enter:'help'\n"<<::std::endl;

    std::string return_check;
    std::cout<<"To return to the game enter: 'exit'"<<std::endl;
    std::getline(std::cin,return_check);

    while(return_check!="exit"){
        std::cout<<"You must enter 'exit'"<<std::endl;
        std::getline(std::cin,return_check);
    }
    //Prints Board to start
    x->print_board();
}

void interface::print_chess_clocks()const{
    
    int seconds_white{0};
    int seconds_black{0};

   
    seconds_white+=white_chess_clock;
    seconds_black+=black_chess_clock;
    

    
    int output_mins_white {seconds_white / 60};
    int output_seconds_white {seconds_white % 60};

    int output_mins_black {seconds_black / 60};
    int output_seconds_black {seconds_black % 60};

    std::cout<<"\n            Game Clocks         "<<std::endl;
    std::cout<<"         White"<<"       Black"<<std::endl;
    std::cout <<"        ["<<std::setfill('0') << std::setw(2) << output_mins_white << ":"
        << std::setfill('0') << std::setw(2) << output_seconds_white<<"]     ["
        << std::setfill('0') << std::setw(2) << output_mins_black << ":"
        << std::setfill('0') << std::setw(2) << output_seconds_black<<"]\n"<<std::endl;
}

bool interface::print_move_list_for_selected(){

    Colour opponent_colour{};
    if(player_turn==Colour::white){
        opponent_colour=Colour::black;
    }
    else{
        opponent_colour=Colour::white;
    }

    std::cout<<"Input the position of piece you'd like help with"<<std::endl;
    std::string piece_position;
    std::getline (std::cin,piece_position);
    //Acts on a sanity check on the form of the inputted move: has to be (char,int,whitespace,char,int)
    if(piece_position.length()!=2 || !std::isdigit(piece_position[1]) || std::isdigit(piece_position[0])||std::islower(piece_position[0])){
        
        std::cout<<"Please input your pieces position correctly (eg:'A5') "<<std::endl;
        return false;
    } 

    //Checks the user has not inputted a coordinate outside  of the board
    if(piece_position[1]-'0'>8 || piece_position[1]-'0'<1){

        std::cout<<"You cannot select pieces outside of the 8x8 board "<<std::endl;
        return false;
    }

    //Seperates the string into the start coordinates and final coordinates
    //We subtract to 0 here to convert from ascii to actual values//
    //We initialise in round brackets as we are not allowed to narrow in curly braces
    //Extra 1 removed from y values to allow for inputs 1-8 instead of 0-7
    //Maps the user input into useable coordinates within the board and piece classes (0-7)
    char x_start(letters_map[piece_position[0]]);
    char y_start((piece_position[1]-'0')-1);

    //Creates a coordinate tuple
    std::tuple<char,char> piece_coordinate (x_start,y_start);

    //Creates a vector of tuples containting all of the moves
    std::vector<std::tuple<char,char>> quasi_legal_move_list{x->get_movelist_at_position(player_turn, piece_coordinate)};
    std::vector<std::tuple<char,char>> capture_move_list{x->get_capture_movelist_at_position(player_turn, piece_coordinate)};

    //If the piece has no legal moves tell the user
    if(quasi_legal_move_list.size()==0){
        std::cout<<"You cannot move from this position"<<std::endl;
        return false;
    }
    

    std::cout<<"LEGAL MOVES"<<std::endl;
    std::cout<<"-----------"<<std::endl;

    //Loops over the vector of moves
    for (int i=0; i < quasi_legal_move_list.size(); i++) {
        
        char final_x{reverse_letters_map[std::get<0>(quasi_legal_move_list[i])]};
        int final_y{std::get<1>(quasi_legal_move_list[i])+1};

        std::string test_move{piece_position+" "+final_x+std::to_string(final_y)};

        //We must the test move data sets as undo will delete the last lines of each
        completed_moves.push_back(test_move);
        x->add_to_is_promotion_list(false);
        set_last_move(test_move);

        x->move_piece(player_turn,piece_coordinate,quasi_legal_move_list[i]);

        // If the move is quasi legal but is a move into check the interface does not allow it
        // Interface only prints out fully legal moves
        if(x->is_check(player_turn)==false){
            
            //If move doesn't cause capture check or check mate just print it
            //Checks if the opponent is in check after the test move
            if(x->is_check(opponent_colour)==true){
                //If check mate tell the user
                if(x->is_check_mate(opponent_colour)==true){
                    std::cout<<test_move<<" (Puts Opponent in Checkmate)"<<std::endl;
                }
                //If check tell the user
                else{
                std::cout<<test_move<<" (Puts Opponent in Check)"<<std::endl;
                }
            }
            //If the move is a capture move tells the user
            if (std::find(capture_move_list.begin(), capture_move_list.end(),quasi_legal_move_list[i]) != capture_move_list.end()){
                std::cout<<test_move<<" (Capture Move)"<<std::endl;
            }
            else{
                std::cout<<test_move<<std::endl;
            }
        }

        //Undo the tested move
        undo_move();

        
    }
    
    return false;
}

bool interface::print_all_moves(){

    int move_count{0};

    Colour opponent_colour{};
    if(player_turn==Colour::white){
        opponent_colour=Colour::black;
    }
    else{
        opponent_colour=Colour::white;
    }

    for(char x_coord{0}; x_coord<8; x_coord++){
        for(int y_coord{0}; y_coord<8; y_coord++){
            
            char initial_x{reverse_letters_map[x_coord]};
            int initial_y{y_coord+1};

            std::string piece_position{initial_x + std::to_string(initial_y)};
            
            //Seperates the string into the start coordinates and final coordinates
            //We subtract to 0 here to convert from ascii to actual values//
            //We initialise in round brackets as we are not allowed to narrow in curly braces
            //Extra 1 removed from y values to allow for inputs 1-8 instead of 0-7
            //Maps the user input into useable coordinates within the board and piece classes (0-7)
            char x_start(letters_map[piece_position[0]]);
            char y_start((piece_position[1]-'0')-1);

            //Creates a coordinate tuple
            std::tuple<char,char> piece_coordinate (x_start,y_start);

            //Creates a vector of tuples containting all of the moves
            std::vector<std::tuple<char,char>> quasi_legal_move_list{x->get_movelist_at_position(player_turn, piece_coordinate)};
            std::vector<std::tuple<char,char>> capture_move_list{x->get_capture_movelist_at_position(player_turn, piece_coordinate)};

            //Loops over the vector of moves
            for (int i=0; i < quasi_legal_move_list.size(); i++) {
                
                char final_x{reverse_letters_map[std::get<0>(quasi_legal_move_list[i])]};
                int final_y{std::get<1>(quasi_legal_move_list[i])+1};

                std::string test_move{piece_position+" "+final_x+std::to_string(final_y)};

                //We must the test move data sets as undo will delete the last lines of each
                completed_moves.push_back(test_move);
                //here we have to check for promotion as is standard for completing any move
                //However if this is promotion nothing happens
                x->add_to_is_promotion_list(false);
                set_last_move(test_move);

                x->move_piece(player_turn,piece_coordinate,quasi_legal_move_list[i]);

                // If the move is quasi legal but is a move into check the interface does not allow it
                // Interface only prints out fully legal moves
                if(x->is_check(player_turn)==false){
                    //If the move doesn't lead to check then it is a legal move
                    move_count+=1;

                    //Checks if the opponent is in check after the test move
                    if(x->is_check(opponent_colour)==true){

                        //If check mate tell the user
                        if(x->is_check_mate(opponent_colour)==true){
                            std::cout<<test_move<<" (Puts Opponent in Checkmate)"<<std::endl;
                        }
                        //If check tell the user
                        else{
                        std::cout<<test_move<<" (Puts Opponent in Check)"<<std::endl;
                        }
                    }
                    //If the move is a capture move tells the user
                    else if (std::find(capture_move_list.begin(), capture_move_list.end(),quasi_legal_move_list[i]) != capture_move_list.end()){
                        std::cout<<test_move<<" (Capture Move)"<<std::endl;
                    }
            
                    //Otherwise just print the legal move
                    else{
                        std::cout<<test_move<<std::endl;
                    }
                    
                }

                //Undo the tested move
                undo_move();    
            }
            
        }
    }

    std::cout<<"There are "<<move_count<<" legal moves available"<<std::endl;
    return false;
}

//Simple get and set functions
void interface::edit_chess_clock(const double &move_time){
    if (player_turn==Colour::white){
        white_chess_clock+=move_time;
    }

    if (player_turn==Colour::black){
        black_chess_clock+=move_time;
    }
}

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

bool interface::undo_move()
{   
    if (completed_moves.size()!=0) {

        std::string previous_move{completed_moves.back()};
        completed_moves.pop_back();
        //Breaks down the last_move string into the start and final moves for use in the illegal undo move
        //Note we use the second coordinates as the start and the first coordinates and the final
        //(eg- the last move is flipped around)
        std::string final_pos {previous_move.substr (0,2)};
        std::string start_pos {previous_move.substr (3,2)};
        

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

        //Sets the running undo_check to true
        undo_check=true;

        //Returns true if undo move succesful
        return true;
    }

    else{
        
        //If the move list size=0 then this is the first move and can't be undone
        std::cout<<"You cannot undo as a first move"<<std::endl;
        
        //Sets the undo check to false as this isn't currently an undo move
        undo_check=false;

        //Returns false as undo move not complete
        return false;
    }

    

    //There are no sanity checks here as the last_move string is only set when a valid legal move is made
}

bool interface::is_input_format_valid(const std::string& str)const
{   
    //series of validation tests for the format of an input to make a move
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

    if(str[0]!='A'&&str[0]!='B'&&str[0]!='C'&&str[0]!='D'&&str[0]!='E'&&str[0]!='F'&&str[0]!='G'&&str[0]!='H'){
        return false;
    }

    if(str[3]!='A'&&str[3]!='B'&&str[3]!='C'&&str[3]!='D'&&str[3]!='E'&&str[3]!='F'&&str[3]!='G'&&str[3]!='H'){
        return false;
    }

    return true;
}

bool interface::helper(){
    std::cout<<"\nWELCOME TO THE HELPER"<<std::endl;
    std::cout<<"-----------------------\n"<<std::endl;
    std::cout<<"To see moves for a selected piece enter:'select piece'"<<std::endl;
    std::cout<<"\nTo see all of your possible moves enter:'all moves'"<<std::endl;
    std::cout<<"\nTo print the board again enter:'print board'"<<std::endl;
    std::cout<<"\nTo return to the game enter:'exit'"<<std::endl;

    //Takes a user input
    std::string user_input;
    std::getline(std::cin,user_input);

    //If the user hasn't exited stay in the helper
    while (user_input!="exit"){

        //Complete correct function for user input
        if(user_input=="select piece"){
            print_move_list_for_selected();
        }
        else if(user_input=="all moves"){
            print_all_moves();
        }
        else if(user_input=="print board"){
            x->print_board();
        }
        std::cout<<"please enter 'select piece','exit','print board' or 'all moves'"<<std::endl;
        std::getline(std::cin,user_input);
    }
    x->print_board();
    return false;
}

bool interface::input_move(std::istream& is){
    
    //Initalise a variable turn purely for ease of coding
    Colour turn{player_turn};

    //Utilises getline function to initialise a string to the istream input
    std::string str;
    std::getline (is,str);

    //Multiple if statements allowing the user to input extra controls and complete the associated functions
    //START
    /////////////////////////////////////////////////////////////////////////

    //Here we return true as and undo move acts as full move for a given turn
    if(str=="undo"){
        //If undo returns true the move has been mad and the players turn is over
        //If undo returns false, due to undo being made at start of game, turn stays the same
        return undo_move();
    }
    else{
        undo_check=false;
    }

    if(str=="quit"){
        quit_check=true;
        return true;
    }

    if(str=="help"){
        return helper();
    }

    if(str=="clock on"){
        is_clock_on=true;
        std::cout<<"Clocks will be printed above the board after each move"<<std::endl;
        return false;
    
    }

    if(str=="clock off"){
        is_clock_on=false;
        std::cout<<"Clocks are turned off"<<std::endl;
        return false;
    }

    //For these extra controls we return false as these do not count as a complted players turn
    if(str=="menu"){
        print_menu();
        return false;
    }

    if(str=="print"){
        x->print_board();
        return false;
    }

    if(str=="taken"){
        x->print_graveyard();
        return false;
    }
    //FINISH
    /////////////////////////////////////////////////////////////////////////


    //Here we assume no extra inputs have been inputted and parse the user input as an attempted chess move 
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
        //Must add the TESTED move into the completed move list
        completed_moves.push_back (str);
        //And then undo the move
        undo_move();
        return false;
    }

    //If a legal move results from the input we add the inputted string to the
    //completed moves vector
    if (is_legal==true){

        completed_moves.push_back (str);
    }
    

    //Returns the results of the is legal variable
    return is_legal;
    
}

void interface::player_move()
{   //Initialise a variable of the player turn for ease of coding
    Colour turn{player_turn};

    //Prints out a command prompt to terminal indicating whose turn it is to move
    if(turn==Colour::white){
        std::cout<<"WHITE PLAYER TURN"<<std::endl;
        std::cout<<"-----------------"<<std::endl;
    }
    if(turn==Colour::black){
        std::cout<<"BLACK PLAYER TURN"<<std::endl;
        std::cout<<"-----------------"<<std::endl;
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
    std::cout<<"YOU HAVE ACHIEVED PROMOTION"<<std::endl;
    std::cout<<"---------------------------\n"<<std::endl;
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
    //Prints starting message
    print_intro_message();

    //Create a while loop allowing user to quit and end the game at any point

    //While the player whose turn it is not in check allow the move sequence to start
    while(x->is_check_mate(player_turn)==false){

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


        //Inform the player if they have been put in check
        if(x->is_check(player_turn) == true){

            std::cout<<"You are in Check"<<std::endl;
            std::cout<<"----------------"<<std::endl;
        }


        //Allow the player to move
        //This function doesn't allow a move into check so players must also move out of check
        //This function also requires a legal move to be made
        player_move();


        //If the user has decided to undo we do not want to check 
        if(undo_check==false){
            //Checks after a move is made if pawn promotion is needed
            if(x->is_pawn_promotion(player_turn)){
                //If pawn promotion is needed allows the player to choose piece to promote to and does this
                player_pawn_promotion();
            }
        }

    

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        double move_time{(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0};
        
        edit_chess_clock(move_time);

        if(quit_check==true){
            break;
        }

        if(is_clock_on==true){
            print_chess_clocks();
        }

        //Prints the board following all made moves
        x->print_board();


        //Switches the turns of the player
        switch_player_turn();

    
    }

    
    if(x->is_check_mate(player_turn)==true){
        //Once check mate has been achieved this outputs the winner of the game and the function ends

        std::cout<<"----------------------------------"<<std::endl;
        std::cout<<"            CHECK MATE            "<<std::endl;
        std::cout<<"----------------------------------"<<std::endl;
        std::cout<<"                                  "<<std::endl;

        if (player_turn==Colour::white){
            std::cout<<"          BLACK HAS WON!          "<<std::endl;
        }

        if (player_turn==Colour::black){
            std::cout<<"          WHITE HAS WON!          "<<std::endl;
        }

        print_chess_clocks();

        std::cout<<"Total Number Of Moves Made:"<<std::endl;
        std::cout<<completed_moves.size()<<std::endl;
    }
    
    std::cout<<"\nTHANKS FOR PLAYING"<<std::endl;

};