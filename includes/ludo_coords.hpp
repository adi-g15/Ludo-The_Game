#include "enumerations.hpp"

#include <map>
#include <vector>
#include <functional>

class ludo_coords{
public:
	std::map<_coord, direction> outer_corners;
	std::map<_coord, direction> inner_turns;
	std::map<_colour, std::pair<_coord, direction>> home_turns;
	std::map<_colour,_coord> start_coords;
	std::map<_colour, direction> start_dir;
	std::vector<_coord> stops;

	friend class game;
public:

	void InitCoords(void);
    direction turnAtCorner(const _coord& curr_coords, const std::map<_coord, direction>&) const;
	ludo_coords();
};