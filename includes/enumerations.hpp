#pragma once

#include <algorithm>
#include <vector>
#include <ostream>

#include "util/coord.hpp"

struct coord : util::_coord2D<int>{
	typedef util::_coord2D<int> coord_base;
	typedef int dimen_t;

	coord() : coord(-1, -1){}	// a default constructor is required for use of coord in std::map (since it constructs a coord, when a new index is used)
	coord(const coord_base& c) : coord(c.x, c.y){}

	coord(dimen_t x, dimen_t y) : coord_base(x, y){}
};

enum class Colour{
	LAAL,
	HARA,
	PEELA,
	NEELA
};

enum class Player : short{	//don't tamper with the data type, static_cast<short> is used on player
	NotDefined = 0,
	_1,
	_2,
	_3,
	_4
};

enum class Direction{
	NO_TURN,
	UP,
	LEFT,
	RIGHT,
	DOWN
};

enum ProfitType {
	NORMAL_MOVE = +1,
	REACH_HOME = +2,
	REACH_STOP = +3,
	ATTACK = +4,
	UNLOCK = +5,
	CROSSES_ENEMY = -3

	/*FUTURE IMPROVEMENTS - Can't it cause cases where,
		1.Moving '6' will be seen better than unlocking(which is only +5)
		2. Moving '6' would have been better for 'following opponent', which can lead to an attack in few steps, instead of UNLOCK?
			*Those two, can be stopping each other, because 1 may sometimes solve 2, but for solving 1, i increased the value of UNLOCK to be +5 (only slightly less than +6), so that +6 isn't wasted in moving everytime, but this is a problem for 2

			THINK - Maybe some algo, that 'scans the state' of whole board, be more helpful, if such can be devised in time
	*/
};

enum class BoxType{
	NORMAL,
	LOCK,
	STOP,
	HOMEPATH,
	HOME_END,
	HOMEAREA,
	UNUSABLE
};

enum class RobotKind{
	randomRobo,
	thinkerRobo
};


namespace util_lamdas{	//the bool return values here, can simply be ignored
	static auto nextColour = [](Colour& c, std::vector<Colour>& order) -> void{
		auto iter = std::find(order.begin(), order.end(), c);

		if( iter != order.end() && ++iter != order.end() )	c = *iter;
		else c = order.front();
	};
	static auto nextPlayer = [](Player& p) -> bool{
		bool repeated{ p == Player::_4 };	//returns true if, player_1 returned, since player_4 was passed

		switch( p ){
			case Player::_1: p = Player::_2; break;
			case Player::_2: p = Player::_3; break;
			case Player::_3: p = Player::_4; break;

			default: p = Player::_1;
		}
		return repeated;
	};
}
