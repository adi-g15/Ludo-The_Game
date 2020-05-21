#pragma once

#define CUSTOM_LUDO_GOTI 1

// #ifndef CUSTOM_ENUMERATIONS
//     #include "enumerations.hpp"
// #endif
// #ifndef TMP
//     #include "global.hpp"
// #endif

// #ifndef CUSTOM_SIMPLETUPLE
//     #include "simpleTuple.hpp"
// #endif

// #ifndef _GLIBCXX_FUNCTIONAL
//     #include<functional>
// #endif

#include "enumerations.hpp"
#include<functional>

class ludo_goti{
protected:
	colours gotiColour;
	direction curr_direction;
	std::pair<int,int> curr_coords;

public:
	const std::pair<int,int> getCoords() const;
    direction get_curr_direction() const;
    colours get_gotiColour(void) const;

	//TODO - Create a balance b/w friends, and private public
	friend class game;
	friend class ludo_box;
	// bool friend ludo_box::removeGoti(std::shared_ptr<ludo_goti>& toBeRemoved);

	bool operator==(const ludo_goti& goti);

	ludo_goti() = delete;
	ludo_goti(colours) = delete;
	ludo_goti(colours, const std::pair<int,int>&, direction = NO_TURN);
};

//FUTURE - May use these below different classes, combined with std::variant(), BUT in that case, 'override'(if possible) the gotiColour as static for them

//Commented these out, after adding a constructor for the ludo_goti class

// class laalGoti : public ludo_goti{
// 	laalGoti();
// };
// class hariGoti : public ludo_goti{
// 	hariGoti();
// };
// class peeliGoti : public ludo_goti{
// 	peeliGoti();
// };
// class neeliGoti : public ludo_goti{
// 	neeliGoti();
// };


//  FUTURE   //
//[FUTURE] - Likely chose between having coordinates, or pointer to ludo_goti(consider the NULL checks)
//[DONE][FUTURE] - Improve security of this function, else coordinates being private wont be useful... Solution used... made moveGoti a friend function
