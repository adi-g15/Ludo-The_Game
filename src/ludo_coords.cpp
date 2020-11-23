#include "ludo_coords.hpp"
#include <utility>

//COORD_CLASS_DEFINTIONS START
void ludo_coords::InitCoords(){
		outer_corners = {
			{ {0,6}, Direction::LEFT },
			{ {0,8}, Direction::DOWN },
			{ {6,0}, Direction::LEFT },
			{ {6,14}, Direction::DOWN },
			{ {8,0}, Direction::UP },
			{ {8,14}, Direction::RIGHT },
			{ {14,6}, Direction::UP },
			{ {14,8}, Direction::RIGHT }
		};

		inner_turns = {
			{ {9,6}, Direction::RIGHT },
			{ {6,5}, Direction::UP },
			{ {8,9}, Direction::DOWN },
			{ {5,8}, Direction::LEFT }
		};

		start_coords = {
			{ _colour::LAAL, {13,6} },
			{ _colour::HARA, {6,1} },
			{ _colour::PEELA, {1,8} },
			{ _colour::NEELA, {8,13} }
		};

		start_dir = {
			{ _colour::LAAL, Direction::UP },
			{ _colour::HARA, Direction::LEFT },
			{ _colour::PEELA, Direction::DOWN },
			{ _colour::NEELA, Direction::RIGHT }
		};

		stops = { {2,6}, {6,12}, {12,8}, {8,2} };
		for (auto &&key_valPair : start_coords){	//Copying start_coords to stops, since their positions are stops too
			stops.push_back(key_valPair.second);
		}

		homePath_turns = {
			{ _colour::LAAL, { {14,7}, Direction::UP}},
			{ _colour::HARA, { {7,0}, Direction::LEFT}},
			{ _colour::PEELA, { {0,7}, Direction::DOWN }},
			{ _colour::NEELA, { {7,14}, Direction::RIGHT }}
		};
}

Direction ludo_coords::turnAtCorner(const coord& curr_coords, const std::map<coord, Direction>& corners_vec) const{
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