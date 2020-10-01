#include "enumerations.hpp"

#include <utility>
#include <map>
#include <vector>
#include <functional>

class ludo_coords{
public:
	std::map<_coord, Direction> outer_corners;
	std::map<_coord, Direction> inner_turns;
	std::map<_colour, std::pair<_coord, Direction>> home_turns;
	std::map<_colour,_coord> start_coords;
	std::map<_colour, Direction> start_dir;
	std::vector<_coord> stops;

	friend class game;
public:
	void InitCoords();
    Direction turnAtCorner(const _coord& curr_coords, const std::map<_coord, Direction>&) const;
	ludo_coords();
};