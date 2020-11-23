#pragma once

#include "forward_decl.hpp"
#include "keywords.hpp"
#include "ludo_state.hpp"
#include "die.hpp"

#include <vector>
#include <mutex>
#include <thread>
#include <optional>

typedef std::pair<unsigned, _dieVal> combination;
/*Each 'combination' holds these -
		0. index to the goti
		1. dieNum (*0 signifies unlock)*/

class thinker{
private:
	std::mutex thinker_mutex; //! A common mutex for all threads

	game *original;
	bool bestMove_available = false; //signifies, whether getBestMove() has been called earlier
	std::vector<combination> bestMove;
	std::map<int, std::vector<combination>> completeMoves; //Mapping profit to the move, instead of move(a whole vector) to an int, to lesser the hash function's 'probable' overhead, also it will automatically chose the last found move with same profit, which would indead have been the case, even when we would have used
	ludo_state *state;									   //The colour and Player used will be of these
	ludo_coords _ludo_coords;

public:
	bool unlock(); //! Only friend 'thinker' can use these to modify the class
	bool move();
	bool operator()(); //Both, move(), and the () operator will call 'getBestMove'

	static Direction getDirOfMovement(const coord &);
	const std::optional<_smartMoveData> isMovePossible(const coord &, int dist) const; //! This will use 'currColour from state only'

	//NOTE - A consideration that has been used is, that when any move is made by any colour, then its goti CANT BE REMOVED, so the indexes of gotis that will be in combination will be valid for the whole move, since neither their order will change being in a set*/
	bool setBestMove(); //Uses 'lots' of midlessMovers to gather moveProfit from each, then pick out the best ones
	std::vector<combination> getBestMove();
	bool mindlessMovers(_dieVal roll, std::vector<_dieVal> dieNumbers, unsigned gotiIndex, std::vector<coord> movingColoursPos, std::vector<coord> opponentsPos, std::pair<std::vector<combination>, int> prevMoves);
	bool implementBestMove();

	bool updateState(game *);
	explicit thinker(game *);
	~thinker();
};
