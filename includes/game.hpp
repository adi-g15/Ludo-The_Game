#pragma once

#include "die.hpp"
#include "exceptions.hpp"
#include "ludo_box.hpp"
#include "ludo_state.hpp"

#include <map>
#include <set>
#include <functional>
#include <optional>

struct _smartMoveData
{
	_coord finalCoords;
	direction finalDirection;

	int moveProfit; //only to be utilised by thinkers
	_smartMoveData() : _smartMoveData({}, direction::NO_TURN){}
	_smartMoveData(const _coord &c, direction dir, int profit = 0) : finalCoords(c),
																	 finalDirection(dir) {}
};

class ludo_state;

class game
{
private:
	typedef void (game::*functionPointer)();

	std::vector<std::vector<ludo_box>> board;
	std::map<_colour, std::vector<std::reference_wrapper<ludo_box>>> lockedPositions;
	std::map<_colour, std::vector<std::shared_ptr<ludo_goti>>> movingGotis;
	std::map<_colour, unsigned> numfinishedGotis;

	/*@brief 1st in this order is the one at bottom left, and next ones anti-clockwise to this*/
	std::array<_colour, 4> colourOrder;

	player currentPlayer;
	_colour currentGotiColour;
	unsigned int number_of_GameRuns;
	unsigned int goti_per_user;

	ludo_coords _ludo_coords; //! An object to make the ludo_coords available to us

	inline bool gameisFinished();
	inline bool isPlayerPlaying(player);
	unsigned getNumLockedGotis(_colour);
	/*  @brief Simply moves a goti of same colour from the locked goti positions,
			   and move the goti to movingGotis, and the std::make_shared to starting box
		@returns bool indicating whether enough locked gotis were available*/
	bool unlockGoti();
	bool lockGoti(std::shared_ptr<ludo_goti>);
	void takeIntro(); //! Initializes the PlayerMap
	void endGame() const; //Only for use by shortcutsMap, and DEBUGGING purpose
	void endGame(std::string cause) const;

public:
	// std::unordered_map<std::string, void(*)(std::string)> shortcutsMap;
	std::map<std::string, functionPointer> shortcutsMap;
	// functionPointer arr[10]; //Learnt - Array of 10 function pointers to functions taking nothing, and returning void

	std::map<_colour, player> coloursMap;
	/*  @guide
	PLAYER std::map
	======================
	PlayerID(enum)  ---->
						1. std::string PlayerName
						2. colour GotiColour

*/

	std::map<player, std::pair<std::string, _colour>> activePlayerMap;
	std::map<player, RobotKind> robotPlayers;

	inline short moveGoti(std::shared_ptr<ludo_goti>, unsigned int dist);
	short moveGoti(std::shared_ptr<ludo_goti>, _smartMoveData moveData);			  //Moves goti to ENDPOINT 'DIRECTLY' (basic checks only)
	bool handleMoveVal(short, std::vector<_dieVal> &dieNumbers, bool isRobot = true); //Handles value returned by moveGoti() calls

	const std::optional<_smartMoveData> isMovePossible(std::shared_ptr<ludo_goti> &, int dist) const; //! Can use std::variant too
																								//The first is goti_index, and 2nd is tried roll
	bool autoMove();																			//! The 'simple' function that will 'simply' call the private recursive overload

	/* @brief Simply removes the 1st goti, if attack request found valid
	   **IMPORTANT_NOTE - For simplicity, an empty vector passed for coloursToRemove will be considered as 'Remove all except this gotiColour'
	   FUTURE - If have used a vector of _colour, in favor of future scope of support of FRIEND GOTIS
	*/
	void attack(std::vector<_colour> coloursToRemove, std::shared_ptr<ludo_goti> attacker);

	void updateDisplay();
	/*NOTE - getEmptyLocks(...) == {0,0} is a good test for 'ZERO LOCKED POSITIONS'*/
	_coord getEmptyLocks(_colour) const;

	bool InitGame(short = 1); //! Starts/Resets the game
	void play(bool = true);
	void settingsMenu();
	void notYetImplementedScr() const;
	inline ludo_box &getBoardBox(const _coord &coords);
	inline const ludo_box &getBoardBox(const _coord &coords) const;

	//Current State Validation Methods
	inline bool isValid(const _coord &coords) const;
	inline bool isValid(const std::shared_ptr<ludo_goti> &) const;

	game();
	~game();

	friend class _BoardPrinter;
	friend class ludo_state;
	friend class ludo_state;
	friend class thinker;
};