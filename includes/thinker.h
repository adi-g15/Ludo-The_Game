#include <set>
#include <vector>
#include <mutex>
#include <thread>
#include "enumerations.hpp"
#include "keywords.hpp"

class game;

class state_goti{
	const colours gotiColour;
	direction currDir;
	coordinates coords;
	~state_goti();	//! state_box will take the ownership

public:
	state_goti(colours, direction, coordinates);

	friend class state_box;
	friend class thinker;
};

class state_box{
	std::set<state_goti*> inBoxGotis;
public:
	BOX_TYPE type;
	inline bool appendGoti(state_goti*);
	state_goti* removeGoti(state_goti*);	//It doesn't delete the goti pointer, delete it yourself, from returned pointer or use this pointer to append somewhere else
	bool areOpponentsPresent(colours) const;

	friend class minimal_ludoState;
	friend class thinker;

	~state_box();
};

//Highly dependent on 'currGoti' and 'currPlayer', and that is intentional, since it is to be used mainly for recovering/saving a 'state' of an object
class minimal_ludoState{
	static unsigned int used_count;

	std::vector<std::vector<state_box>> board;

	std::vector<coordinates> lockedPositions;
	std::map<colours, std::vector<coordinates>> movingColours;	//DONT UPDATE THIS, EVEN IF UPDATED THE BOARD, SINCE IT WILL LATER BE USED TO RESET THE BOARD

	player currPlayer;
	colours currColour;

	void update(const game*);
	void resetBoard();	//Uses 'org_movingColours' to reset the board
	state_box& getBox(const coordinates&);
	const state_box& getBox(const coordinates&) const;

public:
	minimal_ludoState(const game*);
	bool isInSync(const game*);
	size_t getNumLocks();

	friend class thinker;
};

class thinker{
	/*Each 'combination' holds these - 
		0. index to the goti
		1. dieNum (*0 signifies unlock)*/
	typedef std::pair<unsigned short, unsigned short> combination;
private:
	std::mutex thinker_mutex;	//! A common mutex for all threads

	bool bestMove_available = false;	//signifies, whether getBestMove() has been called earlier
	std::vector<combination> bestMove;
	std::map<int, std::vector<combination>> completeMoves;	//Mapping profit to the move, instead of move(a whole vector) to an int, to lesser the hash function's 'probable' overhead, also it will automatically chose the last found move with same profit, which would indead have been the case, even when we would have used
	minimal_ludoState* state;	//The colour and player used will be of these
	ludo_coords _ludo_coords;

public:
	bool unlock();	//! Only friend 'thinker' can use these to modify the class
	inline bool move(game*);
	inline bool operator()(game*);	//Both, move(), and the () operator will call 'getBestMove'

	inline direction getDirOfMovement( const coordinates& ) const;
	const _moveData isMovePossible(const coordinates&, int dist) const;	//! This will use 'currColour from state only'

	//NOTE - A consideration that has been used is, that when any move is made by any colour, then its goti CANT BE REMOVED, so the indexes of gotis that will be in combination will be valid for the whole move, since neither their order will change being in a set*/
	bool setBestMove();	//Uses 'lots' of midlessMovers to gather moveProfit from each, then pick out the best ones
	std::vector<combination> getBestMove();
	bool mindlessMovers ( unsigned short roll, std::vector<unsigned short> dieNumbers, unsigned short gotiIndex, std::vector<coordinates> movingColoursPos, std::vector<coordinates> opponentsPos, std::pair<std::vector<combination>, int> prevMoves );
	bool implementBestMove(game*);

	bool updateState(const game*);
	thinker(const game*);
	~thinker();
};