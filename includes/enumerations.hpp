#pragma once

#include <algorithm>
#include <vector>
#include <ostream>

struct _coord{
	typedef long _type;
	_type n_row, n_col;

	_coord& operator=(const _coord& c) { this->n_row = c.n_row; this->n_col = c.n_col; return *this; }
	bool operator==(const _coord& c) const{ return this->n_row == c.n_row && this->n_col == c.n_col; }
	bool operator<(const _coord& c) const{ if( this->n_row < c.n_row ) return true; else return this->n_col < c.n_col; }
	_coord() : _coord(_type{}, _type{}){}
	_coord(_type x, _type y) : n_row(x), n_col(y){}
	_coord(const _coord& c) : _coord(c.n_row, c.n_col){}
	_coord(const std::pair<_type, _type>& p): _coord(p.first, p.second){}
};

enum class _colour{
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

struct ProfitType{
	static constexpr int NORMAL_MOVE = +1;
	static constexpr int REACH_HOME = +2;
	static constexpr int REACH_STOP = +3;
	static constexpr int ATTACK = +4;
	static constexpr int UNLOCK = +5;	/*TODO - Can't it cause cases where,
					1.Moving '6' will be seen better than unlocking(which is only +5)
					2. Moving '6' would have been better for 'following opponent', which can lead to an attack in few steps, instead of UNLOCK?
						*Those two, can be stopping each other, because 1 may sometimes solve 2, but for solving 1, i increased the value of UNLOCK to be +5 (only slightly less than +6), so that +6 isn't wasted in moving everytime, but this is a problem for 2*/
						//THINK - Maybe some algo, that 'scans the state' of whole board, be more helpful, if such can be devised in time
	static constexpr int CROSSES_ENEMY = -3;
};

enum class Box{
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
	auto nextColour = [](_colour &c, std::vector<_colour>& order) -> void{
		auto iter = std::find(order.begin(), order.end(), c);

		if( iter != order.end() && ++iter != order.end() )	c = *iter;
		else c = order.front();
	};
	auto nextPlayer = [](Player &p) -> bool{
		bool repeated{ false };	//returns true if, player_1 returned, since player_4 was passed
		repeated = p == Player::_4;

		switch( p ){
			case Player::_1: p = Player::_2; break;
			case Player::_2: p = Player::_3; break;
			case Player::_3: p = Player::_4; break;

			default: p = Player::_1;
		}
		return repeated;
	};
}
