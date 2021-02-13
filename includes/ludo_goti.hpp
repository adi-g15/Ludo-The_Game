#pragma once

#include "enumerations.hpp"

#include <functional>

class ludo_goti{
private:
	const Colour gotiColour;
	Direction curr_direction;
	coord curr_coords;

public:
	coord get_coords() const;
    Direction get_curr_direction() const;
    Colour get_goti_colour() const;

	friend class game;
	friend class ludo_box;

	bool operator==(const ludo_goti& goti);

	ludo_goti(Colour, const coord&, Direction = Direction::NO_TURN);
};