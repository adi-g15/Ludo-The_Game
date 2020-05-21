#include "ludo_goti.hpp"

#include<iostream>
#include "ludo_coords.hpp"

const std::pair<int,int> ludo_goti::getCoords() const{
    if(curr_coords.first == 0 && curr_coords.second == 0){
        std::cerr<<"Coordinates of Goti are corrupt (0,0)\n"<<std::endl;
    }
    return curr_coords;
}

direction ludo_goti::get_curr_direction() const{
    return curr_direction;
}

colours ludo_goti::get_gotiColour(void) const{
    return gotiColour;
}

bool ludo_goti::operator==(const ludo_goti& goti){
    return (this->curr_coords == goti.curr_coords) && (this->gotiColour == goti.gotiColour);
}

ludo_goti::ludo_goti(colours gotiColour,const std::pair<int,int>& coords, direction dir) : curr_coords(coords) {
    this->gotiColour = gotiColour;
    this->curr_direction = NO_TURN;
    this->curr_direction = dir;
}

// laalGoti::laalGoti(){
//     this->gotiColour = ColourLAAL;
//     this->curr_coords = ludo_coords::get_startCoords(ColourNEELA);
// }

// hariGoti::hariGoti(){
//     this->gotiColour = ColourHARA;
//     this->curr_coords = ludo_coords::get_startCoords(ColourNEELA);
// }

// peeliGoti::peeliGoti(){
//     this->gotiColour = ColourPEELA;
//     this->curr_coords = ludo_coords::get_startCoords(ColourNEELA);
// }

// neeliGoti::neeliGoti(){
//     this->gotiColour = ColourNEELA;
//     this->curr_coords = ludo_coords::get_startCoords(ColourNEELA);
// }

//  LEARNT  //
//[LEARNT_KNOW_MORE] - 'abstract' isn't a keyword in C++
//[LEARNT] - 	//public Getters maybe used to ensure that private mamber can't be modified from outside
//ERROR_SOLVED - Showing error : expected identifier before numeric constant std::pair<int,int> coords(0,0);
                    /*LEARNT - We can't call the constructor of a datatype right at declaration, "in a class outside any method"
                            But, we can braces{} instead of () to initialize right at declaration, BUT... by passing a constructed object
                                vector<int> v(5);	//Wrong
                                vector<int> v{vector<int>(5)}; //Right
                                
                            So, first declare then, call their constructors in constructor(See this class's constructor)*/
//LEARNT - We can't define, 'implicitly-"declared" function/constructor', we have to explicitly declare the constructor in the header
