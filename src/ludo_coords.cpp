#include "ludo_coords.hpp"
#include <utility>

//COORD_CLASS_DEFINTIONS START
void ludo_coords::InitCoords(){

		outer_corners = {
			{ {0,6}, Direction::EAST },
			{ {0,8}, Direction::SOUTH },
			{ {6,0}, Direction::EAST },
			{ {6,14}, Direction::SOUTH },
			{ {8,0}, Direction::NORTH },
			{ {8,14}, Direction::WEST },
			{ {14,6}, Direction::NORTH },
			{ {14,8}, Direction::WEST }
		};

		inner_turns = {
			{ {9,6}, Direction::WEST },
			{ {6,5}, Direction::NORTH },
			{ {8,9}, Direction::SOUTH },
			{ {5,8}, Direction::EAST }
		};

		start_coords = {
			{ _colour::LAAL, {13,6} },
			{ _colour::HARA, {6,1} },
			{ _colour::PEELA, {1,8} },
			{ _colour::NEELA, {8,13} }
		};

		start_dir = {
			{ _colour::LAAL, Direction::NORTH },
			{ _colour::HARA, Direction::EAST },
			{ _colour::PEELA, Direction::SOUTH },
			{ _colour::NEELA, Direction::WEST }
		};

		stops = { {2,6}, {6,12}, {12,8}, {8,2} };
		for (auto &&key_valPair : start_coords){	//Copying start_coords to stops, since their positions are stops too
			stops.push_back(key_valPair.second);
		}

		home_turns = {
			{ _colour::LAAL, { {14,7}, Direction::NORTH}},
			{ _colour::HARA, { {7,0}, Direction::EAST}},
			{ _colour::PEELA, { {0,7}, Direction::SOUTH }},
			{ _colour::NEELA, { {7,14}, Direction::WEST }}
		};
}

Direction ludo_coords::turnAtCorner(const _coord& curr_coords, const std::map<_coord, Direction>& corners_vec) const{
	for ( auto corner : corners_vec ){
		if(curr_coords == corner.first){
			return corner.second;
		}
	}
	return Direction::NO_TURN;
}

ludo_coords::ludo_coords(){
	InitCoords();
}
//COORD_CLASS_DEFINTIONS END