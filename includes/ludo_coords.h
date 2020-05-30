#include<map>
#include<vector>
#include<functional>
#include "enumerations.hpp"

class ludo_coords{
private:

	std::map<std::pair<int,int>, direction> outer_corners;
	std::map<std::pair<int,int>, direction> inner_turns;
	std::map<colours, std::pair<std::pair<int,int>, direction>> home_turns;
	std::map<colours,std::pair<int,int>> start_coords;
	std::map<colours, direction> start_dir;
	std::vector<std::pair<int,int>> stops;

	friend class game;
public:

	void InitCoords(void);
    direction turnAtCorner(const std::pair<int,int>& curr_coords, const std::map<std::pair<int,int>, direction>&) const;
	ludo_coords();	
};