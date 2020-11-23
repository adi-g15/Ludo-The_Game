#pragma once

#include "enumerations.hpp"

#include <utility>
#include <map>
#include <vector>
#include <functional>

class ludo_coords{
public:
	std::map<coord, Direction> outer_corners;
	std::map<coord, Direction> inner_turns;
	std::map<_colour, std::pair<coord, Direction>> homePath_turns;
	std::map<_colour,coord> start_coords;
	std::map<_colour, Direction> start_dir;
	std::vector<coord> stops;

	friend class game;
public:
	void InitCoords();
    Direction turnAtCorner(const coord& curr_coords, const std::map<coord, Direction>&) const;
	ludo_coords();
};
