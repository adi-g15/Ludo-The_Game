#pragma once

#include "ludo_coords.h"
#include "ludo_box.h"
#include <unordered_map>
#include <set>
#include <functional>
#include <random>
#include "exceptions.h"

#define DEBUG_PRINTBOARD cout<<"\n-------------DEBUG--------------";\
							for(auto &i : board) {\
							std::cout<<"\n|";\
							for(auto &j : i) {\
								std::cout<<(j.inBoxGotis.empty() ? "  " : (j.content+" ") );\
							}\
							std::cout<<'|';\
						 }\
						 cout<<"\n-------------DEBUG--------------"<<endl;

#define DEBUG_NUMERICALBOARD cout<<"\n-------------DEBUG--------------";\
							for(auto &i : board) {\
							std::cout<<"\n|";\
							for(auto &j : i) {\
								std::cout<<(j.inBoxGotis.size() == 0 ? ' ' : static_cast<char>(48 + j.inBoxGotis.size()) ) <<' '; \
							}\
							std::cout<<'|';\
						 }\
						 cout<<"\n-------------DEBUG--------------"<<endl;


namespace Die{
	//! Using 4 different generators as well as sequences, randomly for each player
	static std::random_device dev_engine;
	static std::array<std::mt19937, 4> mt = { std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()) };
	static std::array<std::uniform_int_distribution<int>, 4> dist = { std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6) };	//uniform distribution from [1,6]

	std::vector<unsigned short> rolldie(void);	//! Undelete, if YOU like that version of rollDie
	void rolldie(std::vector<unsigned short>&);

}

struct _smartMoveData{	//!To be exploited by the 'THINKING' Robots
	std::pair<int,int> finalCoords;
	direction finalDirection;
	int moveProfit = 0;
};

struct _moveData{
	bool isPossible = false;
	_smartMoveData smartData;
};

class ludo_state;

class game{
private:
	typedef void (game::*functionPointer)(void);

	std::vector<std::vector<ludo_box>> board;
	std::map<colours, std::vector<std::reference_wrapper<ludo_box>>> lockedPositions;
	std::map<colours, std::vector<std::shared_ptr<ludo_goti>>> movingGotis;
	std::map<colours, unsigned short> numfinishedGotis;

	/*@brief 1st in this order is the one at bottom left, and next ones anti-clockwise to this*/
	std::array<colours,4> colourOrder;

	player currentPlayer;
	colours currentGotiColour;
	unsigned int number_of_GameRuns;
	unsigned int goti_per_user;

	ludo_coords _ludo_coords;	//! An object to make the ludo_coords available to us

	inline bool gameisFinished(void);
	inline bool isPlayerPlaying(player);
	unsigned short getNumLockedGotis(colours);
	/*  @brief Simply moves a goti of same colour from the locked goti positions,
			   and move the goti to movingGotis, and the std::make_shared to starting box
		@returns bool indicating whether enough locked gotis were available*/
	bool unlockGoti(void);
	bool lockGoti(std::shared_ptr<ludo_goti>);
	void takeIntro(void);	//! Initializes the PlayerMap
	void endGame() const;	//Only for use by shortcutsMap, and DEBUGGING purpose
	void endGame(std::string cause) const;

public:	
	// std::unordered_map<std::string, void(*)(std::string)> shortcutsMap;
	std::unordered_map<std::string, functionPointer> shortcutsMap;
	// functionPointer arr[10]; //Learnt - Array of 10 function pointers to functions taking nothing, and returning void

	std::map<colours, player> coloursMap;
/*  @guide
	PLAYER std::map
	======================
	PlayerID(enum)  ---->
						1. std::string PlayerName
						2. colour GotiColour

*/

	std::map<player, std::pair< std::string, colours >> activePlayerMap;
	std::map<player, RobotKind> robotPlayers;

	inline short moveGoti(std::shared_ptr<ludo_goti>, unsigned int dist);
	inline short moveGoti(std::shared_ptr<ludo_goti>, _moveData moveData);
	short moveGoti(std::shared_ptr<ludo_goti>, _smartMoveData moveData);	//Moves goti to ENDPOINT 'DIRECTLY' (basic checks only)
	bool handleMoveVal( short, std::vector<unsigned short> &dieNumbers, bool isRobot = true);	//Handles value returned by moveGoti() calls

	const _moveData isMovePossible(std::shared_ptr<ludo_goti>&, int dist);	//! Can use std::variant too
																	//The first is goti_index, and 2nd is tried roll
	bool autoMove();	//! The 'simple' function that will 'simply' call the private recursive overload

	/* @brief Simply removes the 1st goti, if attack request found valid
	   **IMPORTANT_NOTE - For simplicity, an empty vector passed for coloursToRemove will be considered as 'Remove all except this gotiColour'
	   FUTURE - If have used a vector of colours, in favor of future scope of support of FRIEND GOTIS
	*/
	void attack(std::vector<colours> coloursToRemove, std::shared_ptr<ludo_goti> attacker);

	void updateDisplay(void);
		/*NOTE - getEmptyLocks(...) == {0,0} is a good test for 'ZERO LOCKED POSITIONS'*/
	std::pair<int,int> getEmptyLocks(colours) const;

	//! bool return values, used here, are usually for debugging purposes
	bool InitGame(short = 1);	//! Starts/Resets the game
	void play(bool = true);
	void settingsMenu();
	void notYetImplementedScr(void) const;
	inline ludo_box& getBoardBox(const std::pair<int,int>& coords);

	//Current State Validation Methods
	inline bool isValid(const std::pair<int,int>& coords) const;
	inline bool isValid(const std::shared_ptr<ludo_goti>&) const;

	game();
	~game();

	friend class _BoardPrinter;
	friend class ludo_state;
	friend class minimal_ludoState;
	friend class thinker;

};

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

//3/4 days after above the above comment, i have decided to go for this class, and separate my thinker into a different class
class ludo_state{
	bool deepCopied = false;
	std::shared_ptr<game> theDeepCopy;	//! Caution, this will be the owner of 'a' (not 'the') game object, created for a deepcopy

		//! Used raw pointers instead of 
	const game* theBoss;	//! the Boss is the 'actual game object', the state talks about

	const std::map<colours, std::vector<std::reference_wrapper<ludo_box>>>& state_lockedPositions;
	const std::map<colours, std::vector<std::shared_ptr<ludo_goti>>>& state_movingGotis;
	const std::map<colours, unsigned short>& state_numfinishedGotis;
	const std::map<player, std::pair< std::string, colours >>& state_PlayerMap;

	colours state_GotiColour;
	player state_Player;
	std::map<player, RobotKind> state_robots;

public:
	bool deepCopy(game&);	// Will make the object, have its own copies, that wont modify, due to modification of elements in original 'game object'
						//! For using it, convert the members from mere raw references, to reference_wrapper<>
							//Won't implement now, since no use currently

	ludo_state(const game* Boss) : theBoss(Boss),
									state_lockedPositions(Boss->lockedPositions),
									state_movingGotis(Boss->movingGotis), 
									state_numfinishedGotis(Boss->numfinishedGotis),
									state_PlayerMap(Boss->activePlayerMap),
									state_GotiColour(Boss->currentGotiColour),
									state_Player(Boss->currentPlayer),
									state_robots(Boss->robotPlayers){}
	ludo_state() = delete;

	friend class game;
};

#include "thinker.h"