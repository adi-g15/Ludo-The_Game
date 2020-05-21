#include<map>
#include<vector>
#include<functional>
#include "enumerations.hpp"

// class ludo_coords{
// public:
// 	static std::vector<std::pair<int,int>> stops;
// 	static std::map<std::pair<int,int>, direction> corners, inner_turns;
// 	static std::map<std::pair<int,int>, direction> home_turns;
// 	static std::map<colours,std::pair<int,int>> start_coords;

// 	static void InitCoords(void);	//NOTE - This must be called to be able to use the coords
// 	static std::pair<int,int> get_startCoords(colours);
//     static direction turnAtCorner(const std::pair<int,int>& curr_coords, const std::map<std::pair<int,int>, direction> &cornerCoord);

// };


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

	void InitCoords(void);	//NOTE - This must be called to be able to use the coords
    direction turnAtCorner(const std::pair<int,int>& curr_coords, const std::map<std::pair<int,int>, direction>&) const;
	ludo_coords();
	
	//ERROR - With 'static', the functions give 'umdefined references' linker errors
	//QUESTION - And, without 'static', the data members showed MULTIPLE DECLARATIONS (WHY ??)
};

//FUTURE - Shift to std::unordered_map from std::map