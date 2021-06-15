//Luke Peters: 10204030
//31/05/2021

//Main function
//Simply creates and interface and calls the play game function

# include "Pieces.h"
# include "Board.h"
# include "Interface.h"
# include <iostream>
#include <tuple>
#include <string>

int main()
{   
    //Create a unique pointer to a new interface, this creates a chessboard and all the relevant piece
    std::unique_ptr<interface> interface_A{new interface};
    
    //Call the play function from the interface
    interface_A->play_game();

};