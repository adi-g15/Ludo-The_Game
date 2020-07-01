#pragma once
#include<utility>

typedef std::pair<int, int> coordinate;

enum colours{
	UnknownColour,
	ColourLAAL,
	ColourHARA,
	ColourPEELA,
	ColourNEELA
};

enum player{
	PlayerNotDefined,
	Player1,
	Player2,
	Player3,
	Player4
};

enum direction{
	NO_TURN,
	NORTH,
    EAST,
	WEST,
	SOUTH
};

enum profitType{
	NORMAL_MOVE = +1,
	REACH_HOME = +2,
	REACH_STOP = +3,
	ATTACK = +4,
	UNLOCK = +5,	/*TODO - Can't it cause cases where,
						1.Moving '6' will be seen better than unlocking(which is only +5)
						2. Moving '6' would have been better for 'following opponent', which can lead to an attack in few steps, instead of UNLOCK?
						 *Those two, can be stopping each other, because 1 may sometimes solve 2, but for solving 1, i increased the value of UNLOCK to be +5 (only slightly less than +6), so that +6 isn't wasted in moving everytime, but this is a problem for 2*/
					//THINK - Maybe some algo, that 'scans the state' of whole board, be more helpful, if such can be devised in time
	CROSSES_ENEMY = -3
};

enum BOX_TYPE{
	_boxNORMAL,
	_boxLOCK,
	_boxSTOP,
	_boxHOMEPATH,
	_boxHOME_END,
	_boxHOMEAREA,
	_boxUNUSABLE
};

enum RobotKind{
	randomRobo,
	thinkerRobo
};