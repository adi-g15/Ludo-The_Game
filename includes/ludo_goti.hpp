#pragma once

#include "enumerations.hpp"

#include <functional>

class ludo_goti{
private:
	const _colour gotiColour;
	Direction curr_direction;
	coord curr_coords;

public:
	coord getCoords() const;
    Direction get_curr_direction() const;
    _colour get_gotiColour() const;

	friend class game;
	friend class ludo_box;

	bool operator==(const ludo_goti& goti);

	ludo_goti() = delete;
	ludo_goti(_colour, const coord&, Direction = Direction::NO_TURN);
};