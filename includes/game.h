#pragma once

#include "ludo_coords.h"
#include "ludo_box.h"
#include <unordered_map>
#include <set>
#include <functional>
#include <random>
#include <exceptions.h>

#define DEBUG_PRINTBOARD cout<<"------------DEBUG-------------";\
							for(auto &i : board) {\
							std::cout<<"\n|";\
							for(auto &j : i) {\
								std::cout<<j.get_box_content()<<' ';\
							}\
							std::cout<<'|';\
						 }\
						 cout<<"\n------------DEBUG-------------\n";


namespace Die{
	//! Using 4 different generators as well as sequences, randomly for each player
	static std::random_device dev_engine;
	static std::array<std::mt19937, 4> mt = { std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()) };
	static std::array<std::uniform_int_distribution<int>, 4> dist = { std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6) };	//uniform distribution from [1,6]
}

struct _movePossible{
	bool isPossible = false;
	std::pair<int,int> finalCoords;
	direction finalDirection;

	bool operator==( bool boolVal ){	//! Added this to make the struct, 'behave' like a boolean, when directly compared, since that is what its and isMovePossible's main motive
		return isPossible == boolVal;
	}
};

struct _smartMoveData{	//!To be exploited by the 'THINKING' Robots
	_movePossible possibilityData;
	int moveProfit = 0;

	_movePossible getPossibility() const{
		return possibilityData;
	}
};

class game{
private:
	typedef void (*functionPointer)();

	std::array<std::array<ludo_box,15>,15> board;
	std::map<colours, std::vector<std::reference_wrapper<ludo_box>>> lockedPositions;
	std::map<colours, std::vector<std::shared_ptr<ludo_goti>>> movingGotis;
	std::map<colours, unsigned short> numfinishedGotis;

	/*@brief 1st in this order is the one at bottom left, and next ones anti-clockwise to this*/
	std::array<colours,4> colourOrder;

	//NOTE - These have been used in many member functions, instead of exchanging between them, this also ensures that they all are in sync, for eg. updateDisplay(), unlockGoti()
		//! IMP_NOTE - Don't use these in box::append_goti(), or remove_goti(), or this->lockGoti, SINCE, THEY DEPEND ON THE GOTI PASSED, WHICH MAY BE DIFFERENT DUE TO CALLS LIKE FROM ATTACK() WHICH CALLS LOCK() ON DIFFERENT COLOURS
	player currentPlayer;
	colours currentGotiColour;
	unsigned int number_of_GameRuns;
	unsigned int min_boxlen;
	unsigned int goti_per_user;

	ludo_coords _ludo_coords;	//! An object to make the ludo_coords available to us

	inline bool gameisFinished(void);
	inline bool isPlayerPlaying(player);
	bool unlockGoti(void);
	bool lockGoti(std::shared_ptr<ludo_goti>);
	void takeIntro(void);	//! Initializes the PlayerMap
	bool autoMove(std::set< std::pair<unsigned short, unsigned short> >& triedCombinations, std::vector<unsigned short> dieNumbers);	//Goti, and Distance being random, rollOutput is used to pass to 'recursive' calls

public:	
	std::unordered_map<std::string, void(*)(std::string)> shortcutsMap;
	// std::unordered_map<std::string, functionPointer> shortcutsMap;
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
	std::set<player> robotPlayers;

	short moveGoti(std::shared_ptr<ludo_goti>, unsigned int dist);
	short moveGoti(std::shared_ptr<ludo_goti>, std::pair<int,int> finalCoords);	//Moves goti to ENDPOINT 'DIRECTLY' (basic checks only)
	const _smartMoveData isMovePossible(std::shared_ptr<ludo_goti>&, unsigned int dist);	//! Can use std::variant too
																	//The first is goti_index, and 2nd is tried roll
	bool autoMove(colours gotiColour);	//! The 'simple' function that will 'simply' call the private recursive overload

	/* @brief Simply removes the 1st goti, if attack request found valid
	   **IMPORTANT_NOTE - For simplicity, an empty vector passed for coloursToRemove will be considered as 'Remove all except this gotiColour'
	   FUTURE - If have used a vector of colours, in favor of future scope of support of FRIEND GOTIS
	*/
	void attack(std::vector<colours> coloursToRemove, std::shared_ptr<ludo_goti> attacker);

	/*  @brief Simply moves a goti of same colour from the locked goti positions,
			   and std::move the goti to movingGotis, and the std::make_shared to starting box
		@returns bool indicating whether enough locked gotis were available*/
	void rolldie(std::vector<unsigned short>&);	//The vector will have a values of the dieRoll; Made this independent function to reduce duplicacy
	void updateDisplay(void);
	unsigned short getNumGotis(colours);
		/*NOTE - getEmptyLocks(...) == {0,0} is a good test for 'ZERO LOCKED POSITIONS'*/
	std::pair<int,int> getEmptyLocks(colours);

	//!Bool return values are usually for debugging purposes
	bool InitGame(short = 1);	//! Starts/Resets the game
	void play(bool = true);
	void settingsMenu(const std::string& source);
	void notYetImplementedScr(void);
	inline ludo_box& getBoardBox(const std::pair<int,int>& coords);

	//Current State Validation Methods
	inline bool isValid(const std::pair<int,int>& coords) const;
	inline bool isValid(const std::shared_ptr<ludo_goti>&) const;

	game();
	~game();

	friend class _BoardPrinter;

};