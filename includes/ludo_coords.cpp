#include "ludo_coords.hpp"
#include<utility>

//COORD_CLASS_DEFINTIONS START
void ludo_coords::InitCoords(){

		outer_corners = {
			{ {0,6}, EAST },
			{ {0,8}, SOUTH },
			{ {6,0}, EAST },
			{ {6,14}, SOUTH },
			{ {8,0}, NORTH },
			{ {8,14}, WEST },
			{ {14,6}, NORTH },
			{ {14,8}, WEST }
		};

		inner_turns = {
			{ {9,6}, WEST },
			{ {6,5}, NORTH },
			{ {8,9}, SOUTH },
			{ {5,8}, EAST }
		};
			
		start_coords = {
			{ ColourLAAL, {13,6} },
			{ ColourHARA, {6,1} },
			{ ColourPEELA, {1,8} },
			{ ColourNEELA, {8,13} }
		};

		start_dir = {
			{ ColourLAAL, NORTH },
			{ ColourHARA, EAST },
			{ ColourPEELA, SOUTH },
			{ ColourNEELA, WEST }
		};

		stops = { {2,6}, {6,12}, {12,8}, {8,2} };
		for (auto &&key_valPair : start_coords){	//Copying start_coords to stops, since their positions are stops too
			stops.push_back(key_valPair.second);
		}

		home_turns = {
			{ ColourLAAL, { {14,7}, NORTH}},
			{ ColourHARA, { {7,0}, EAST}},
			{ ColourPEELA, { {0,7}, SOUTH }},
			{ ColourNEELA, { {7,14}, WEST }}
		};
}

direction ludo_coords::turnAtCorner(const std::pair<int,int>& curr_coords, const std::map<std::pair<int,int>, direction>& corners_vec) const{
	for ( auto corner : corners_vec ){
		if(curr_coords == corner.first){
			return corner.second;
		}
	}
	return NO_TURN;
}

ludo_coords::ludo_coords(){
	InitCoords();
}
//COORD_CLASS_DEFINTIONS END