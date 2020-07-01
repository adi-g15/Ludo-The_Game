#pragma once

#include<functional>
#include "enumerations.hpp"

class ludo_goti{
private:
	const colours gotiColour;
	direction curr_direction;
	coordinate curr_coords;

public:
	coordinate getCoords() const;
    direction get_curr_direction() const;
    colours get_gotiColour(void) const;

	friend class game;
	friend class ludo_box;

	bool operator==(const ludo_goti& goti);

	ludo_goti() = delete;
	ludo_goti(colours, const std::pair<int,int>&, direction = NO_TURN);
};