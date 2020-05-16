#include<map>
#include<vector>
#include<functional>
#include "enumerations.hpp"

// class ludo_coords{
// public:
// 	static std::vector<std::pair<int,int>> stops;
// 	static std::map<std::pair<int,int>, direction> outer_corners, inner_turns;
// 	static std::map<std::pair<int,int>, direction> home_turns;
// 	static std::map<colours,std::pair<int,int>> start_coords;

// 	static void InitCoords(void);	//NOTE - This must be called to be able to use the coords
// 	static std::pair<int,int> get_startCoords(colours);
//     static direction dir_to_turn_ifCoord_in_CornerCoordsVec(const std::pair<int,int>& curr_coords, const std::map<std::pair<int,int>, direction> &cornerCoord);

// };


namespace ludo_coords{

	static std::vector<std::pair<int,int>> stops;
	static std::map<std::pair<int,int>, direction> outer_corners, inner_turns;
	static std::map<std::pair<int,int>, direction> home_turns;
	static std::map<colours,std::pair<int,int>> start_coords;

	void InitCoords(void);	//NOTE - This must be called to be able to use the coords
    direction dir_to_turn_ifCoord_in_CornerCoordsVec(const std::pair<int,int>& curr_coords, const std::map<std::pair<int,int>, direction> &cornerCoord);

	//ERROR - With 'static', the functions give 'umdefined references' linker errors
	//QUESTION - And, without 'static', the data members showed MULTIPLE DECLARATIONS (WHY ??)
};

//FUTURE - Shift to std::unordered_map from std::map