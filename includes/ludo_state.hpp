#pragma once

#include "enumerations.hpp"
#include "ludo_coords.hpp"
#include "forward_decl.hpp"

/*@brief It 'will' be a class to store the 'states' of the game at that time, for which the state is created.
		1. I thought of this kind of feature to be highly helpful in saving, loading, resetting the game, and only this much data as available in a ludo_state object will be required.
		2. It was also thought of being highly helpful, to the AI algorithm i was trying to develop -
			Learning(Will likely not be implemented as of now) - a knowledge of the previous gameplay, ie. all previous states fo the game
			Prediction - Knowledge of 'future' possible states will help (for eg. to move 6 for unlock or to 'chase')
	Left case... What if an already safe goti moves +5 to a STOP, while there's a goti, who is ahead of enemy by just 2 units (Couldn't it be saved? Will it need memory of past profit numbers? )
		3. Can also be used, to have the AI thinker, as a separate class, and pass the ludo_state at that time, to object of that class

	@worst_part - Likely i wont be developing it further, the earlier is a brief of the idea.
				   If you like it, and succeed in developing this, first of all I will be very thankful, secondly, you keep all the AI credits! :D
		*/

#include <set>

class state_goti{
	const _colour gotiColour;
	Direction currDir;
	coord coords;
	~state_goti();

public:
	state_goti(_colour, Direction, coord);

	friend class state_box;
	friend class ludo_state;

	friend class thinker;
};

class state_box{
	std::vector<state_goti*> inBoxGotis;
public:
	Box type;
	bool appendGoti(state_goti*);
	state_goti* removeGoti(state_goti*);	//It doesn't delete the goti pointer, delete it yourself, from returned pointer or use this pointer to append somewhere else
	bool areOpponentsPresent(_colour) const;

	friend class ludo_state;
	friend class thinker;

	~state_box();
};

//Highly dependent on 'currGoti' and 'currPlayer', and that is intentional, since it is to be used mainly for recovering/saving a 'state' of an object
class ludo_state{
	static unsigned int used_count;

	std::vector<std::vector<state_box>> board;

	std::vector<coord> lockedPositions;
	std::map<_colour, std::vector<coord>> movingColours;	//DONT UPDATE THIS, EVEN IF UPDATED THE BOARD, SINCE IT WILL LATER BE USED TO RESET THE BOARD

	Player currPlayer;
	_colour currColour;

	void update(const game*);
	state_box& getBox(const coord&);
	const state_box& getBox(const coord&) const;

public:
	explicit ludo_state(const game*);
	bool isInSync(const game*);
	void resetBoard();	//Uses 'org_movingColours' to reset the board
	size_t getNumLocks();

	friend class thinker;
};
