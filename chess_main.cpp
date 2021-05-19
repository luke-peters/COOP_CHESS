# include "Pieces.h"
# include "Board.h"
# include "Interface.h"
# include <iostream>
#include <tuple>
#include <string>

int main()
{   
    //Print out starting rules
    std::cout<<"\nWELCOME TO C++ OOP CHESS"<<std::endl;
    std::cout<<"--------------------------"<<std::endl;
    std::cout<<"Created by Luke Peters (Student ID: 10204030)\n"<<std::endl;
    std::cout<<"1.You will not be allowed to make an illegal chess moves"<<std::endl;
    std::cout<<"2.You will be informed if your are placed into check"<<std::endl;
    std::cout<<"3.You will not be allowed to make a move that results in you being in check"<<std::endl;
    std::cout<<"4.The game will end when checkmate is achieved"<<std::endl;
    std::cout<<"5.Pawn promotion is supported"<<std::endl;
    std::cout<<"You must input your moves in algebraic notation using caps and separated by a space (eg: 'E2 E4')"<<std::endl;
    std::cout<<"At any time during your turn [enter] 'menu' to view extra controls\n\n"<<std::endl;

    //Create a unique pointer to a new interface
    std::unique_ptr<interface> interface_A{new interface};
    //Run play function on the interface
    interface_A->play_game();

};