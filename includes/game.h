#pragma once

#include <unordered_map>
#include <set>
#include <functional>

#include "die.hpp"
#include "ludo_state.h"
#include "ludo_box.h"
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

struct _smartMoveData{
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
	friend class ludo_state;
	friend class thinker;

};