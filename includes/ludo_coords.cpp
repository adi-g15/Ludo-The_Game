#include "ludo_coords.hpp"
#include<utility>

//COORD_CLASS_DEFINTIONS START
void ludo_coords::InitCoords(){

		stops = std::vector<std::pair<int,int>>({

		});

		outer_corners = std::map<std::pair<int,int>,direction>({
			std::make_pair(std::pair<int,int>(0,6), NORTH),
			std::make_pair(std::pair<int,int>(0,8), EAST),
			std::make_pair(std::pair<int,int>(14,6), WEST),
			std::make_pair(std::pair<int,int>(14,8), SOUTH),
			std::make_pair(std::pair<int,int>(6,0), NORTH),
			std::make_pair(std::pair<int,int>(8,0), WEST),
			std::make_pair(std::pair<int,int>(6,14), EAST),
			std::make_pair(std::pair<int,int>(8,14), SOUTH)
		});
			
		inner_turns = std::map<std::pair<int,int>,direction>({
			std::make_pair(std::pair<int,int>(6,6), WEST),
			std::make_pair(std::pair<int,int>(6,8), NORTH),
			std::make_pair(std::pair<int,int>(8,6), SOUTH),
			std::make_pair(std::pair<int,int>(8,8), EAST)
		});

		start_coords = std::map<colours,std::pair<int,int>>({
			std::make_pair(ColourLAAL, std::pair<int,int>(6,1)),
			std::make_pair(ColourHARA, std::pair<int,int>(1,8)),
			std::make_pair(ColourNEELA, std::pair<int,int>(8,13)),
			std::make_pair(ColourPEELA, std::pair<int,int>(13,6)),
		});
	}

direction ludo_coords::dir_to_turn_ifCoord_in_CornerCoordsVec(const std::pair<int,int>& curr_coords, const std::map<std::pair<int,int>, direction> &corner_vec){
	for ( auto corner : corner_vec ){
		if(curr_coords == corner.first){
			return corner.second;
		}
	}
	return NO_TURN;
}
//COORD_CLASS_DEFINTIONS END