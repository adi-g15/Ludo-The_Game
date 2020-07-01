#include<map>
#include<vector>
#include<functional>
#include "enumerations.hpp"

class ludo_coords{
public:
	std::map<coordinate, direction> outer_corners;
	std::map<coordinate, direction> inner_turns;
	std::map<colours, std::pair<coordinate, direction>> home_turns;
	std::map<colours,coordinate> start_coords;
	std::map<colours, direction> start_dir;
	std::vector<coordinate> stops;

	friend class game;
public:

	void InitCoords(void);
    direction turnAtCorner(const coordinate& curr_coords, const std::map<coordinate, direction>&) const;
	ludo_coords();	
};