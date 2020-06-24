#include<map>
#include<vector>
#include<functional>
#include "enumerations.hpp"

typedef std::pair<int, int> coordinates;

class ludo_coords{
// private:
public:
	std::map<coordinates, direction> outer_corners;
	std::map<coordinates, direction> inner_turns;
	std::map<colours, std::pair<coordinates, direction>> home_turns;
	std::map<colours,coordinates> start_coords;
	std::map<colours, direction> start_dir;
	std::vector<coordinates> stops;

	friend class game;
public:

	void InitCoords(void);
    direction turnAtCorner(const coordinates& curr_coords, const std::map<coordinates, direction>&) const;
	ludo_coords();	
};