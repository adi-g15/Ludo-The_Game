#include "ludo_coords.hpp"
#include "ludo_goti.hpp"

#include <iostream>

coord ludo_goti::getCoords() const{
    if(curr_coords.y == 0 && curr_coords.x == 0){
        std::cerr<<"Coordinates of Goti are corrupt (0,0)\n"<<std::endl;
    }
    return curr_coords;
}

Direction ludo_goti::get_curr_direction() const{
    return curr_direction;
}

_colour ludo_goti::get_gotiColour() const{
    return gotiColour;
}

bool ludo_goti::operator==(const ludo_goti& goti){
    return (this->curr_coords == goti.curr_coords) && (this->gotiColour == goti.gotiColour);
}

ludo_goti::ludo_goti(_colour gotiColour,const coord& coords, Direction dir) : gotiColour(gotiColour), curr_coords(coords) {
    this->curr_direction = Direction::NO_TURN;
    this->curr_direction = dir;
}