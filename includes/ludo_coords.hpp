#pragma once

#include "enumerations.hpp"

#include <utility>
#include <map>
#include <vector>
#include <functional>

struct ludo_coords{	// note - ALL these will remain CONSTANT, not modifying this now to keep it simple
	std::map<coord, Direction> outer_corners;
	std::map<coord, Direction> inner_turns;
	std::map<Colour, std::pair<coord, Direction>> homePath_turns;
	std::map<Colour,coord> start_coords;
	std::map<Colour, Direction> start_dir;
	std::vector<coord> stops;

    Direction turnAtCorner(const coord& curr_coords, const std::map<coord, Direction>&) const;
	ludo_coords();

	friend class game;
};
