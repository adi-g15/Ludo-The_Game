#include "keywords.hpp"
#include "exceptions.hpp"
#include "boardPrinter.hpp"
#include "game.hpp"
#include "thinker.hpp"
#include "util/find_remove.hpp"
#include "util/string_util.hpp"
#include "util/timer.hpp"
#include "util/terminal.hpp"
#include "util/combinatorics.hpp"

#include <iostream>
#include <utility>
#include <algorithm>
#include <set>
#include <sstream>
#include <cstdarg>

using namespace std;

//GAME_CLASS_DEFINTIONS START
const std::optional<_smartMoveData> game::isMovePossible(std::shared_ptr<ludo_goti>& the_goti, int dist) const{
	if( dist == 0 )
		return {};

	coord increment_coord(0, 0);
	coord updated_coords(the_goti->curr_coords);
	Direction turnDir, currDir = the_goti->curr_direction;

	while( dist-- ){
		increment_coord = { 0, 0 };
		turnDir = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.outer_corners); //! For Outer Corners

		if( turnDir != Direction::NO_TURN ){ //! ie. a turn will happen to go to next box
			currDir = turnDir;

			switch( currDir ){
				case Direction::UP:
					increment_coord = { -1, 0 };	break;
				case Direction::LEFT:
					increment_coord = { 0, 1 };	break;
				case Direction::RIGHT:
					increment_coord = { 0, -1 };	break;
				case Direction::DOWN:
					increment_coord = { 1, 0 };	break;
			}
		} else{
			turnDir = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.inner_turns); //For Inner Turns

			if( turnDir != Direction::NO_TURN ){
				currDir = turnDir;

				switch( currDir ){
					case Direction::UP:
						increment_coord = { -1, 1 };	break;
					case Direction::LEFT:
						increment_coord = { 1, 1 };	break;
					case Direction::RIGHT:
						increment_coord = { -1, -1 };	break;
					case Direction::DOWN:
						increment_coord = { 1, -1 };	break;
				}
			} else{
				//Checking for Home_Turns
				if( updated_coords == _ludo_coords.homePath_turns.at(the_goti->gotiColour).first ){
					currDir = _ludo_coords.homePath_turns.at(the_goti->gotiColour).second;
				}

				//! ie. needs to 'go straight' on its current path
				switch( currDir ){
					case Direction::UP:
						increment_coord = { -1, 0 };	break;
					case Direction::LEFT:
						increment_coord = { 0, 1 };	break;
					case Direction::RIGHT:
						increment_coord = { 0, -1 };	break;
					case Direction::DOWN:
						increment_coord = { 1, 0 };	break;
				}
			}
		}

		updated_coords.y += increment_coord.y;
		updated_coords.x += increment_coord.x;
		reference_wrapper<const ludo_box> currBox{ getBoardBox(updated_coords) };

		if( currBox.get().box_type == Box::HOME_END && dist != 0 ){ //! Reached finished point, but move still incomplete
			return {};
		}
	}

	return _smartMoveData({ updated_coords, currDir });
}

/*Return values - -1 : Can't Move, or FAILURE
				   0 : Normal
				   1 : Goti pung gayi
				   2 : Attacked (signifies signal to give extra diceRoll to player)*/
short game::moveGoti(std::shared_ptr<ludo_goti> the_goti, unsigned int dist){
	auto movePossibility = this->isMovePossible(the_goti, dist);
	if( movePossibility.has_value() )
		return moveGoti(the_goti, movePossibility.value());
	return -1;
}

//For this function, the move will initially be considered 'possible', since it is meant to be called through the overloads
short game::moveGoti(std::shared_ptr<ludo_goti> the_goti, _smartMoveData moveData){
	if( !isValid(moveData.finalCoord) || getBoardBox(moveData.finalCoord).box_type == Box::UNUSABLE ){
		cout << "Passed coords " << moveData.finalCoord << " to moveGoti is invalid !";
		endGame("Inalid goti to moveGoti");
		return -1;
	}

	const auto [finalCoord, finalDir, moveProfit] = moveData;

	// //DEBUG
	// if( (movingGotis[curr_colour].size() + numfinished[curr_colour] + getNumLockedGotis(curr_colour)) > 4 ){
	// 	cout << "inBoxGotis -> " << getBoardBox(_ludo_coords.start_coords[curr_colour]).inBoxGotis.size() << endl
	// 		<< "Finished -> " << numfinished[curr_colour] << endl
	// 		<< "LockedGotis -> " << getNumLockedGotis(curr_colour) << endl
	// 		<< "movingGotis -> ";
	// 	for( auto& i : movingGotis[curr_colour] )
	// 		cout << i->curr_coords << ' ';
	// 	cout << endl;
	// 	throw endApplication("Multiple gotis at start");
	// }

	if( !isValid(the_goti) && !isValid(finalCoord) )
		return -1;

	getBoardBox(the_goti->curr_coords).removeGoti(the_goti); //Reaches here only if move possible
	if( getBoardBox(finalCoord).box_type == Box::HOME_END ){
		auto& lockBox = getBoardBox(getEmptyLocks(curr_colour));

		if( lockBox.coords == coord(0, 0) ){ //debug
			cout << "movingGotis[curr].size() = " << movingGotis[curr_colour].size() << endl;
			cout << "inBoxGotis in the box of the goti " << getBoardBox(the_goti->curr_coords).inBoxGotis.size() << endl;
			endGame("Invalid lockbox");
		}

		//Removing from movingGotis
		auto iter = find(movingGotis[curr_colour].begin(), movingGotis[curr_colour].end(), the_goti);
		if( iter != movingGotis[curr_colour].end() ){
			movingGotis[curr_colour].erase(iter);
			the_goti.reset(); //REMOVING ALL REFERENCES TO THE GOTI
		} else
			cout << "Goti not found";

		lockBox.content = ":D";
		lockBox.box_type = Box::UNUSABLE;

		if( (movingGotis[curr_colour].size() + numfinished[curr_colour] + getNumLockedGotis(curr_colour)) > 4 ){	//debug
			clog << "inBoxGotis -> ";
			for( auto& i : getBoardBox(the_goti->curr_coords).inBoxGotis )
				clog << i->curr_coords << " ";
			clog << endl
				<< "Finished -> " << numfinished[curr_colour] << endl
				<< "LockedGotis -> " << getNumLockedGotis(curr_colour) << endl
				<< "movingGotis -> ";
			for( auto& i : movingGotis[curr_colour] )
				clog << i->curr_coords << ' ';
			this->endGame(3, "Extra Gotis of colour ", colourNames[curr_colour], " punGING");
		}

		++numfinished.at(curr_colour);

		return 1;
	} else{
		the_goti->curr_coords = finalCoord;
		the_goti->curr_direction = finalDir;

		if( getBoardBox(the_goti->curr_coords).appendGoti(the_goti) == 0 ){ //appendGoti() signalled an Attack needed
			attack({}, the_goti);

			return 2;
		}
	}
	return 0;
}

bool game::handleMoveVal(short moveVal, vector<_dieVal>& dieNumbers, bool isRobot){ //! Messages won't print for robots
	try{
		if( moveVal == 1 || moveVal == 2 ){
			if( !isRobot ){
				cout << "\nPress enter to roll the die once more...";
				cin.get();
			}
			Die::getDieResult(dieNumbers);
		}
		return true;
	} catch( ... ){
		return false;
	}
}

/*BUG - For example for a random_goti_index the goti isn't movable, then that diceNumber is being discarded. [For eg. in 6,1 only 1 is moved and 6 not] */
bool game::autoMove(){ //! Return values same as the moveGoti_function
	std::vector<_dieVal> dieNumbers{ Die::getDieResult() };
	std::set<unsigned> triedGotis_indices; //! Stores the gotis that have been tried to move, FOR A SINGLE DIE_NUMBER
	std::optional< _smartMoveData > moveObj;
	bool wasSuccess = false;

	updateDisplay();
	cout << "Roll Output : ";
	for( auto& i : dieNumbers )
		cout << i << ' ';

	if( movingGotis[curr_colour].empty() ){
		auto iter = find(dieNumbers.begin(), dieNumbers.end(), 6);
		if( iter != dieNumbers.end() ){
			unlockGoti();
			dieNumbers.erase(iter);

			updateDisplay();
			cout << "Roll Output : ";
			for( auto& i : dieNumbers ){
				cout << i << ' ';
			}
		} else{
			clog << "Can't Move, due to absence of gotis" << endl;
			return false;
		}
	}

	int index = 0;
	// for( auto currentRoll : dieNumbers ){	//Outer loop iterates through the gotis
	//BUG_solved_for_now - See the bugs.txt file for this
	for( size_t i = 0; i < dieNumbers.size(); ++i ){
		auto currentRoll = dieNumbers[i];
		++index;

		if( currentRoll == 6 ){
			if( rand() % 2 == 1 && getNumLockedGotis(curr_colour) > 0 ){ //50% chance of robot using 6 to unlockGoti
				unlockGoti();
				continue;
			}
		}

		do{ //! ie. gotis still remaining to try
			if( movingGotis[curr_colour].empty() )
				return false;
			unsigned random_goti_index = rand() % (movingGotis[curr_colour].size()); // Randomly selected a goti to move this much units

			try{
				if( triedGotis_indices.size() == movingGotis.at(curr_colour).size() ){
					cout << currentRoll << " can't be utilized for any move" << endl;
					break; //Break out to the outer loop, thus rejecting currentRoll
				}

				moveObj = isMovePossible(movingGotis.at(curr_colour).at(random_goti_index), currentRoll);
				if( moveObj.has_value() ){
					auto moveVal = moveGoti(movingGotis.at(curr_colour).at(random_goti_index), moveObj.value());
					handleMoveVal(moveVal, dieNumbers);

					wasSuccess = true;
				}
			} catch( std::out_of_range& e ){
				cerr << e.what();
				endGame("AutoMove->Out_Of_Range exception");
			}

			if( !movingGotis[curr_colour].empty() )
				random_goti_index = std::rand() % (movingGotis[curr_colour].size()); //Since, AFTER FINISH, MOVINGGOTIS maybe empty by this time, causing FLOATING POINT EXCEPTION

			triedGotis_indices.insert(random_goti_index);
		} while( !moveObj.has_value() && !movingGotis[curr_colour].empty() );

		triedGotis_indices.clear();
	}

	return wasSuccess;
}

/* @brief Simply removes the 1st goti, if attack request found valid
   **IMPORTANT_NOTE - For simplicity, an empty vector passed for coloursToRemove will be considered as 'Remove all except this gotiColour'
   FUTURE - If have used a vector of _colour, in favor of future scope of support of FRIEND GOTIS
*/
void game::attack(std::vector<_colour> coloursToRemove, std::shared_ptr<ludo_goti> attacker){
	if( coloursToRemove.empty() ){ //Will consider all other _colour as opponents
		coloursToRemove.insert(coloursToRemove.begin(), { _colour::NEELA, _colour::HARA, _colour::PEELA, _colour::LAAL });
		coloursToRemove.erase(find(coloursToRemove.begin(), coloursToRemove.end(), curr_colour)); //Using currentGotiPlayer and not attacker's colour, since only the curr_player should be able to attack
	}

	ludo_box& currBox = getBoardBox(attacker->curr_coords);

	for( unsigned i = 0; i < currBox.inBoxGotis.size(); ++i ){ //Caution - May be a probable cause for iterator invalidation
		if( std::find(coloursToRemove.begin(), coloursToRemove.end(), currBox.inBoxGotis[i]->gotiColour) != coloursToRemove.end() ){ //! emulating "if gotiColour in colourToRemove"
			lockGoti(currBox.inBoxGotis[i]);
			--i;
		}
	}
}

bool game::isValid(const coord& coords) const{
	if( coords.y >= 0 && coords.y < 15 && coords.x >= 0 && coords.x < 15 ){
		return true;
	}

	endGame("Invalid Coords, in isValid");
	return false;
}

bool game::isValid(const shared_ptr<ludo_goti>& goti) const{
	if( goti->gotiColour == curr_colour && isValid(goti->curr_coords) ){
		return true;
	}
	cerr << "GOTI COLOUR DOESN'T RELATE TO CURRENT_PLAYER" << endl;
	endGame();
	return false;
}

ludo_box& game::getBoardBox(const coord& coords){
	if( isValid(coords) )
		return board[coords.y][coords.x];
	else
		throw std::out_of_range("OUT_OF_RANGE: Board indices, passed to getBoardBox, are out of bounds");
}

const ludo_box& game::getBoardBox(const coord& coords) const{
	if( isValid(coords) )
		return board[coords.y][coords.x];
	else
		throw std::out_of_range("OUT_OF_RANGE: Board indices, passed to getBoardBox, are out of bounds");
}

/*  @brief Simply moves a goti of same colour from the locked goti positions,
		   and move the goti to movingGotis, and the std::make_shared to starting box
	@returns bool indicating whether enough locked gotis were available*/
bool game::unlockGoti(){
	if( movingGotis[curr_colour].size() >= goti_per_user ){
		cerr << "Goti request makes the number of " << colourNames[curr_colour] << " more than " << goti_per_user << endl;
		return false;
	}

	//First std::find_if gives us the raw reference of box
	auto box_iter = std::find_if(lockedPositions[curr_colour].begin(), lockedPositions[curr_colour].end(), [&](auto& lockedBox){
		return !lockedBox.get().inBoxGotis.empty() && !lockedBox.get().areOpponentsPresent(curr_colour);
	});

	if( box_iter == lockedPositions[curr_colour].end() ){	  //no locked goti present
		{ //DEBUG block
			clog << "No locked goti was present" << endl;
			clog << "inBoxGotis -> " << getBoardBox(_ludo_coords.start_coords[curr_colour]).inBoxGotis.size() << endl
				<< "Finished -> " << numfinished[curr_colour] << endl
				<< "LockedGotis -> " << getNumLockedGotis(curr_colour) << endl
				<< "movingGotis -> ";
			for( auto& i : movingGotis[curr_colour] )
				clog << i->curr_coords << ' ';
			clog << endl;
			throw endApplication("Multiple gotis at start");
		}
		return false;
	}

	auto goti = box_iter->get().inBoxGotis.back(); //Causing the SEG_Fault
	box_iter->get().removeGoti(goti);

	goti->curr_coords = _ludo_coords.start_coords[curr_colour];
	goti->curr_direction = _ludo_coords.start_dir[curr_colour];

	getBoardBox(goti->curr_coords).appendGoti(goti);
	movingGotis[curr_colour].push_back(goti);

	if( (getBoardBox(_ludo_coords.start_coords[curr_colour]).inBoxGotis.size() + numfinished[curr_colour] + getNumLockedGotis(curr_colour)) > 4 + 1 ){ //DEBUG
		clog << "inBoxGotis -> " << getBoardBox(_ludo_coords.start_coords[curr_colour]).inBoxGotis.size() << endl
			<< "Finished -> " << numfinished[curr_colour] << endl
			<< "LockedGotis -> " << getNumLockedGotis(curr_colour) << endl
			<< "movingGotis -> ";
		for( auto& i : movingGotis[curr_colour] )
			clog << i->curr_coords << ' ';
		clog << endl;
		throw endApplication("Multiple gotis at start");
	}

	return true;
}

unsigned game::getNumLockedGotis(_colour gotiColour){
	auto num = 0;
	for( auto const& lockedBox : lockedPositions.find(gotiColour)->second ){
		if( !lockedBox.get().isEmpty() && lockedBox.get().box_type == Box::LOCK ){
			++num;
		}
	}
	return num;
}

bool game::lockGoti(std::shared_ptr<ludo_goti> goti_to_lock){ //wrong gotiColour goti called to be removed, curr_player was different
	cout << "BEFORE MovingGotis -> ";
	for( auto& i : movingGotis[curr_colour] )
		cout << i->curr_coords << ' ';
	cout << endl;

	if( !isValid(goti_to_lock->curr_coords) )
		return false;

	goti_to_lock->curr_direction = Direction::NO_TURN;

	getBoardBox(goti_to_lock->curr_coords).removeGoti(goti_to_lock);
	auto iter = find(movingGotis[goti_to_lock->gotiColour].begin(), movingGotis[goti_to_lock->gotiColour].end(), goti_to_lock);

	if( iter == movingGotis[goti_to_lock->gotiColour].end() ){
		clog << "ERROR => Trying to lock a non-moving Goti" << endl;
		// cout<<"goti_to_lock -> "<<goti_to_lock->curr_coords;
		clog << "goti_to_lock Data-> " << goti_to_lock->curr_coords << " " << goti_to_lock->gotiColour << ' ' << goti_to_lock->curr_direction << endl;
		for( auto& i : movingGotis[curr_colour] )
			if( goti_to_lock->curr_coords == i->curr_coords )
				cout << "Details of goti in movingGotis -> " << i->gotiColour << ' ' << i->curr_direction;
		cout << endl;
		endGame("Invalid Request to lockGoti");
	} else
		movingGotis[goti_to_lock->gotiColour].erase(iter);

	auto&& lockBox = std::find_if(lockedPositions[goti_to_lock->gotiColour].begin(), lockedPositions[goti_to_lock->gotiColour].end(), [](std::reference_wrapper<ludo_box>& box){
		return box.get().isEmpty();
	});

	lockBox->get().appendGoti(goti_to_lock);

	cout << "AFTER MovingGotis -> ";
	for( auto& i : movingGotis[curr_colour] )
		cout << i->curr_coords << ' ';
	cout << endl;
	return true;
}

void game::takeIntro(){
	coord tmpDimen(0, 0);
	tmpDimen = util::getTerminalDimen();

	_BoardPrinter::errorScreen("Please ensure window size is at least 31*31");
	cout << endl;

	//QUESTION - How to call the function pointer through an iterator ?

	do{
		tmpDimen = util::getTerminalDimen();
	} while( tmpDimen.y < 31 || tmpDimen.x < 62 ); //tmpDimen -> row*column

	_BoardPrinter::titleBar(tmpDimen.x);

	util::place_v_center(tmpDimen.y - 7, "");
	util::align_text_center(tmpDimen.x, "Enter names of the Players (Leave empty if not playing, or type \"ROBOT\") : ");

	string playerName;
	_colour colour;
	auto p = Player::_1;
	auto numRobots = 0, numThinkers = 0;

	colour = colourOrder.front();

	for( auto i = 0;i < 4;++i ){	//Loop it 4 times (not mandatory to give details of all players though)
		util::place_center(tmpDimen.x, string("Player").append(playerId[p]).append(" - "));

		getline(cin, playerName);
		util::trim(playerName);

		if( !playerName.empty() ){
			if( shortcutsMap.find(playerName) != shortcutsMap.end() ){
				shortcutsMap.find(playerName)->second; //Calling using the function pointer
			} else if( util::icompare(playerName, robot_keyword) ){
				++numRobots;
				activePlayerMap.insert(
					{ p, {string("ROBOT ").append(to_string(numRobots)), colour} }
				);
				robotPlayers.insert({ p, RobotKind::randomRobo });
			} else if( util::icompare(playerName, thinker_keyword) ){
				++numThinkers;
				activePlayerMap.insert(
					{ p, {string("Thinker").append(to_string(numThinkers)), colour} }
				);
				robotPlayers.insert({ p, RobotKind::thinkerRobo });
			} else{
				activePlayerMap.insert(
					{ p, {playerName, colour} }
				);
			}
		}

		util_lamdas::nextPlayer(p);
		util_lamdas::nextColour(colour, this->colourOrder);
	}

	//Setting the board, on basis of given players
	for( auto&& player : activePlayerMap ){
		auto gotiColour = player.second.second;
		for( auto&& lockedBox : lockedPositions[gotiColour] ){
			lockedBox.get().appendGoti(std::shared_ptr<ludo_goti>(new ludo_goti(gotiColour, getEmptyLocks(gotiColour))));
			lockedBox.get().box_type = Box::LOCK;
		}
		coloursMap.insert({ player.second.second, player.first });
		numfinished.insert({ player.second.second, 0 });
	}

	for( auto&& stop : _ludo_coords.stops ){
		getBoardBox(stop).content = 'X';
	}

	if( this->activePlayerMap.empty() )
		this->endGame("No Players to play");
}

bool game::gameisFinished(){
	for( auto& p : activePlayerMap ){
		if( numfinished[p.second.second] != 4 )
			return false;
	}
	return true;
}

bool game::isPlayerPlaying(Player p){
	return activePlayerMap.find(p) != activePlayerMap.end();
}

vector<_dieVal>&& Die::getDieResult(){
	std::vector<_dieVal> v;
	Die::getDieResult(v);
	return std::move(v);
}

int Die::rollDie() {
#if USE_UNIFORM_DISTRIB
	return distrib(dev_engine);
#else
	return mt_gen()%6 + 1;
#endif

}

void Die::getDieResult(vector<_dieVal>& currDieRolls){
	_dieVal dieNum = Die::rollDie();
	if( dieNum != 6 ){ //To prevent cases like, dieNumbers = {4, 6} (Think! It IS possible in the do-while)
		currDieRolls.push_back(dieNum);
		return;
	}

	do{
		currDieRolls.push_back(dieNum);
		dieNum = Die::rollDie();
	} while( dieNum == 6 );
	currDieRolls.push_back(dieNum); //To insert the last left non-6

		//! Main logic is above only, below is removing cases of 3 sixes (since not allowed in ludo)
	std::vector<size_t> sixPos; //Temporary vector, used in 'Cleaning cases of 3 sixes'
	for( size_t i = 0; i < currDieRolls.size(); ++i ){
		if(currDieRolls[i] == 6 )
			sixPos.push_back(i);
	}
	while( sixPos.size() > 3 ){		  //ie. more than 3 sixes existing
		currDieRolls.erase(currDieRolls.begin() + sixPos[sixPos.size() - 1]); // Removing last 3 positions which all had 6
		currDieRolls.erase(currDieRolls.begin() + sixPos[sixPos.size() - 2]);
		currDieRolls.erase(currDieRolls.begin() + sixPos[sixPos.size() - 3]);

		sixPos.resize( sixPos.size() - 3 );	// remove the last 3 positions that had 6
	}

}

void game::updateDisplay(){
	uint16_t boxlen = 0;
	coord tmpDimen(0, 0);

	util::pause(0.5f);

	bool flag = false;	// this flag is set when the screen is small, and the while loop runs atleast 2 times
	do{
		if( flag )	// if reaching here for the second time
			_BoardPrinter::errorScreen("Please ensure window size is at least 31*31");
		tmpDimen = util::getTerminalDimen();
		boxlen = (2 * min(tmpDimen.y, tmpDimen.x) - 32) / 15;
		flag = true;	//nevertheless, it won't reach again if the loop only runs once
	} while( min(tmpDimen.y, tmpDimen.x) < 32 );

	_BoardPrinter::titleBar(tmpDimen.x);

	util::place_center(tmpDimen.x - 15 * (boxlen + 1) + 3 - 4);
	cout << "  ";
	for( size_t i = 0; i < 10; i++ ){
		for( auto j = boxlen; j-- > 0;) //NOTE - '-->' is a combination of the 2 operators '--' and '>' ;D
			cout << ' ';
		cout << i;
	}
	for( size_t i = 10; i < 15; i++ ){
		for( auto j = boxlen - 1; j-- > 0;) //NOTE - '-->' is a combination of the 2 operators '--' and '>' ;D
			cout << ' ';
		cout << i;
	}
	cout << "\n  ";
	util::place_center(tmpDimen.x - 15 * (boxlen + 1) + 3 - 4);

	for(auto i = 0; i < (boxlen + 1) * 15 + 1; i++ )
		cout << '-';
	cout << "\n";

#define BOARD_CENTER util::place_center(tmpDimen.x - 15 * (boxlen + 1) + 3 - 4);

	_BoardPrinter _board_printer(board);
	_board_printer.boxlen = boxlen;

	BOARD_CENTER cout << 0 << ' ';	_board_printer.row_type1(0);
	_board_printer.inter_type1();
	BOARD_CENTER cout << 1 << ' ';	_board_printer.row_type2(1);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 2 << ' ';	_board_printer.row_type1(2);
	_board_printer.inter_type3();
	BOARD_CENTER cout << 3 << ' ';	_board_printer.row_type1(3);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 4 << ' ';	_board_printer.row_type2(4);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 5 << ' ';	_board_printer.row_type1(5);
	_board_printer.inter_type4();

	BOARD_CENTER cout << 6 << ' ';	_board_printer.row_type3(6);
	_board_printer.inter_type5();
	BOARD_CENTER cout << 7 << ' ';	_board_printer.row_type4(7);
	_board_printer.inter_type5();
	BOARD_CENTER cout << 8 << ' ';	_board_printer.row_type3(8);
	_board_printer.inter_type4();

	BOARD_CENTER cout << 9 << ' ';	_board_printer.row_type1(9);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 10;		_board_printer.row_type2(10);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 11;		_board_printer.row_type1(11);
	_board_printer.inter_type3();
	BOARD_CENTER cout << 12;		_board_printer.row_type1(12);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 13;		_board_printer.row_type2(13);
	_board_printer.inter_type1();
	BOARD_CENTER cout << 14;		_board_printer.row_type1(14);

	cout << "  ";
	BOARD_CENTER for(auto i = 0; i < (boxlen + 1) * 15 + 1; i++ ) cout << '-';

#undef BOARD_CENTER

	cout << "\n\n";
	util::align_text_center(activePlayerMap[curr_player].first);
	cout << "\n";
	for( int i = 0; i < tmpDimen.x; ++i )
		cout << '-';
	cout << '\n';
}

coord game::getEmptyLocks(_colour gotiColour) const{
	for( auto const& lockedBox : lockedPositions.find(gotiColour)->second ){
		if( lockedBox.get().isEmpty() && lockedBox.get().box_type == Box::LOCK ){
			return lockedBox.get().coords;
		}
	}
	return { 0, 0 }; //ie. no locked gotis of this colour
}

bool game::InitGame(short playerConsent){ //! 1 for complete reset, 2 is with previous playerMap
	if( playerConsent != 1 && playerConsent != 2 ){
		return false;
	} else{
		goti_per_user = 4;

		for( auto& ludoBoardRow : board ){
			for( auto& ludoBoardSquare : ludoBoardRow ){
				ludoBoardSquare.content.clear();
				ludoBoardSquare.inBoxGotis.clear();
			}
		}

		if( playerConsent == 1 ){ //not changed, in case it is 2
			activePlayerMap.clear();
			coloursMap.clear();

			takeIntro();
		}

		//coloursMap and activePlayerMap initialised by takeIntro()
	}

	++number_of_GameRuns;

	return true;
}

// @todo - [For functionality for directly entering dieNumber if only 1 'movable' goti available] Also inside when loop for checking if choice_num is correct, add another condition, to check if there is only one goti available to move(not matters whether 6inRoll or not), if thats true, and the choice_num is in dieNumbers, considering enteredRoll=choice_num and choice_num=1*/
void game::play(bool boolVal){
	if( !boolVal )
		return;

	curr_colour = colourOrder.front();
	curr_player = coloursMap[curr_colour];

	std::vector<_dieVal> dieNumbers;

	//Lambda Defintions
	auto lambda_next = [&](){
		do{
			util_lamdas::nextPlayer(this->curr_player);
			util_lamdas::nextColour(this->curr_colour, this->colourOrder);
		} while( (!gameisFinished() && (this->numfinished[this->curr_colour] == this->goti_per_user))
			|| this->coloursMap.find(curr_colour) == this->coloursMap.end()	//ie. curr_colour is not playing, then keep looping, until a playing colour is chosen
			);

		dieNumbers.clear();
	};

	while( !this->gameisFinished() ){ //The gameplay loop
		_dieVal enteredRoll;
		unsigned choice_num;
		bool isRobot = robotPlayers.find(curr_player) != robotPlayers.end();
		string inputStr; //! Note - To use shortcuts like ":settings", ":rules, and to have 'variable number of inputs'
		if( isRobot ){ //ie. It is a Robot Player
			// auto robo = thinker(this);

			// if( robotPlayers[curr_player] == RobotKind::thinkerRobo )
			// 	robo.implementBestMove();
			// else if( robotPlayers[curr_player] == RobotKind::randomRobo )
			autoMove();
			lambda_next();
			continue;
		}

		updateDisplay();
		cout << "\nPress Enter to roll the die...";
		cin.get();
		Die::getDieResult(dieNumbers);
		cout << "\nRoll Output - ";
		for( auto&& outNum : dieNumbers ){
			cout << outNum << ' ';
		}

		//This whole if-else nested block is for "AUTOMATIC DECISIONS", eg. moving a single goti possible to move
		//Automating Condition #1
		if( movingGotis[curr_colour].empty() ){
			//no six
			if( !util::contains<_dieVal>(dieNumbers, 6) ){ //No movable goti, so continue to next player
				cout << "\nNo movable goti, for the moves" << endl;

				lambda_next();
				continue;
			} else{ //No movable gotis currently, hence automatically open one, since 6 is available
				unlockGoti();
				updateDisplay();
				cout << "Goti unlocked!" << endl;
				util::pause(0.8f);
				dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
			}
		}
		//! Actual logic to move each player is this nested while loop
		while( !dieNumbers.empty() && !movingGotis[curr_colour].empty() ){
			updateDisplay();
			unsigned counter = 1;
			cout << "\nChose from these gotis : ";
			if( util::contains<_dieVal>(dieNumbers, 6) )
				cout << "\n0. Unlock New Goti (just type 0)\n\n";
			for( auto&& goti_ptr : movingGotis[curr_colour] ){
				cout << counter << ". [" << goti_ptr->curr_coords.y << "][" << goti_ptr->curr_coords.x << "]\n";
				++counter;
			}
			cout << "\nRoll Output - ";
			for( auto&& outNum : dieNumbers ){
				cout << outNum << ' ';
			}

			//The Automating Condition #2
			auto continue_OuterWhile = false; //To break out of outer loop, from inside an inner loop, BCZ coninue INSIDE THIS WILL NOT SHIFT TO THE NEXT PLAYER
			while( movingGotis[curr_colour].size() == 1 && dieNumbers[0] != 6 && dieNumbers.size() == 1 ){
				auto moveData = isMovePossible(movingGotis[curr_colour][0], dieNumbers[0]);
				if( !moveData.has_value() ){
					cout << "Move not possible" << endl;
				} else{ //goti has been moved
					auto moveVal = moveGoti(movingGotis[curr_colour][0], moveData.value());
					handleMoveVal(moveVal, dieNumbers, false);
				}
				dieNumbers.erase(dieNumbers.begin()); //! Removing the used goti number (which is actually dieNumbers[0])
				lambda_next();
				continue_OuterWhile = true;
			}
			if( continue_OuterWhile )
				continue;

			cout << "\nEnter Goti and dieNumber : ";
			do{
				getline(cin, inputStr);
				util::trim(inputStr);
			} while( inputStr.empty() );

			//! This is just a simple block, for separating the process, of Fetching choice_num and enteredRoll
			{
				if( shortcutsMap.find(inputStr) != shortcutsMap.end() ){
					shortcutsMap.find(inputStr)->second; //Calling using the function pointer
					return;
				} else{
					istringstream stream(inputStr);
					stream >> choice_num;
					if( choice_num == 0 ){
						if( util::contains<_dieVal>(dieNumbers, 6) ){
							unlockGoti();
							dieNumbers.erase(std::find(dieNumbers.begin(), dieNumbers.end(), 6));
						} else{
							cout << "Chhaka to aaye pehle, tab na goti niklegi yar!\n";
							continue; //Wrong Input
						}
					} else{ //! Usual Case - No unlock, nor any cheat/shortcut used
						stream >> enteredRoll;

						if(
							util::isSum(enteredRoll, dieNumbers) //If output is 6,5 then entering 11 is valid
							&& (choice_num <= counter)			 //Verifies Entered values are valid
							){
							short moveVal = moveGoti(movingGotis[curr_colour][choice_num - 1], enteredRoll);
							if( moveVal == -1 ){ //Failure
								cout << "Invalid Move" << endl;
								util::pause(1);
							} else{ //goti has been moved
								handleMoveVal(moveVal, dieNumbers);

								auto elements = util::isSumOfElements(enteredRoll, dieNumbers);
								util::remove_common(dieNumbers, elements);
							}
						}
					}
				}
			}
			// updateDisplay();
		}
		//! dieNumbers is again Empty
		lambda_next();
	}
	if( gameisFinished() )
		_BoardPrinter::finishedScreen();
}

void game::endGame() const{
	_BoardPrinter::finishedScreen();
	// this->~game();	//Causes segFault
	throw endApplication("A shortcut");
}

void game::endGame(string_view msg1) const{
	_BoardPrinter::errorScreen(msg1);

	throw endApplication("EndGame Called");
}

void game::endGame(int n, ...) const{
	va_list	args;
	va_start(args, n);
	_BoardPrinter::errorScreen(args, n);
	va_end(args);

	throw endApplication("EndGame called, with multiple reasons");
}

//! Source is the name of function from which it was called
// @todo - Complete Logic for the options (Will implement after i have made a gui, or web version)
void game::settingsMenu(){
	coord termDimen = util::getTerminalDimen();
	unsigned choice = 11;
	std::string inputStr;

	do{
		_BoardPrinter::titleBar(termDimen.x);

		util::place_v_center(termDimen.y - 2 * (2 + 11 + 4)); //height of 2 taken by titleBar, 11 by Choices, and 4 by Input

		util::align_text_center(termDimen.x, "1. Change 'GamePlay' Order (PlayerWise)");
		cout << '\n';
		util::align_text_center(termDimen.x, "2. Change 'Number of gotis_per_user'");
		cout << '\n';
		util::align_text_center(termDimen.x, "3. Which goti should 'Start' First?");
		cout << '\n';
		util::align_text_center(termDimen.x, "4. Assign Colours to Players");
		cout << '\n';
		util::align_text_center(termDimen.x, "5. Remove 'stops'");
		cout << '\n';
		util::align_text_center(termDimen.x, "6. Change Title shown at Top");
		cout << '\n';
		util::align_text_center(termDimen.x, "7. Change Player Names");
		cout << '\n';
		util::align_text_center(termDimen.x, "8. Reset to Original Positions");
		cout << '\n';
		util::align_text_center(termDimen.x, "9. Use different Random_Sequence for each (y/n)");
		cout << '\n';
		util::align_text_center(termDimen.x, "10. Let's Cheat... ;D (Can remove opponents gotis)");
		cout << '\n';
		util::align_text_center(termDimen.x, "[ 11. Alwida... ]");
		cout << "\n\n";

		util::align_text_center(termDimen.x, "----------------------------");
		util::align_text_center(termDimen.x - 2, "Enter Choice : ");
		cin >> choice;
	} while( choice <= 11 );

	switch( choice ){
		case 1:
		{
			_BoardPrinter::titleBar(termDimen.x);
			util::place_v_center(termDimen.y - 2 * (2 + 1)); //height of 2 taken by titleBar, 11 by Choices, and 4 by Input

			cout << "Type playerNumbers in order (for eg. \"1432\") : ";
			cin >> inputStr;
			// util::strip(inputStr, ' ');
			// inputNum = std::stoi(inputStr);

			// @todo

			break;
		}

		case 2:
			notYetImplementedScr();
			break;
		case 3:
			notYetImplementedScr();
			break;
		case 4:
			notYetImplementedScr();
			break;
		case 5:
			notYetImplementedScr();
			break;
		case 6:
			notYetImplementedScr();
			break;
		case 7:
			notYetImplementedScr();
			break;
		case 8:
			notYetImplementedScr();
			break;
		case 9:
			notYetImplementedScr();
			break;
		case 10:
			notYetImplementedScr();
			break;
		case 11:
			notYetImplementedScr();
			break;
	}
}

void game::notYetImplementedScr() const{
	_BoardPrinter::titleBar();
	util::place_v_center();
	util::align_text_center("This feature has not yet been implemented !");
}

game::game(): colourOrder({ _colour::LAAL, _colour::NEELA, _colour::PEELA, _colour::HARA }){
	number_of_GameRuns = 0;
	goti_per_user = 4;

	board.resize(15);
	for( int i = 0; i < 15; ++i ){
		board[i].reserve(15);
		for( int j = 0; j < 15; ++j ){
			board[i].push_back(ludo_box({ i, j }));
		}
	}

	// shortcutsMap.insert({
	// 	// {":r", &game::updateDisplay},
	// 	// {":refresh", &game::updateDisplay},
	// 	// {":set", &game::settingsMenu },
	// 	// {":settings", &game::settingsMenu},
	// 	// {":q", &game::endGame },
	// 	// {":quit", &game::endGame },
	// 	// {":exit", &game::endGame }
	// 	});

	unsigned i, j;
	//! Marking the LockRooms and the HomePath
	for( i = 0; i < 6; i++ ){
		for( j = 0; j < 6; j++ ){
			board[i][j].box_type = Box::UNUSABLE;
			board[7][j].box_type = Box::HOMEPATH;
			board[7][14 - j].box_type = Box::HOMEPATH;
		}
		board[i][7].box_type = Box::HOMEPATH;
		board[14 - i][7].box_type = Box::HOMEPATH;
	}
	board[0][7].box_type = board[14][7].box_type = Box::NORMAL;
	board[7][0].box_type = board[7][14].box_type = Box::NORMAL;
	for( i = 0; i < 6; i++ ){
		for( j = 9; j < 15; j++ )
			board[i][j].box_type = Box::UNUSABLE;
	}
	for( i = 9; i < 15; i++ )
		for( j = 0; j < 6; j++ )
			board[i][j].box_type = Box::UNUSABLE;
	for( i = 9; i < 15; i++ )
		for( j = 9; j < 15; j++ )
			board[i][j].box_type = Box::UNUSABLE;
	//! Marking the HomeArea
	for( i = 6; i < 9; i++ )
		for( j = 6; j < 9; j++ )
			board[i][j].box_type = Box::UNUSABLE;
	//! Marking the Stops
	for( auto& i : _ludo_coords.stops ){
		getBoardBox(i).box_type = Box::STOP;
	}
	board[6][7].box_type = board[8][7].box_type = board[7][6].box_type = board[7][8].box_type = Box::HOME_END;

	colourOrder = { _colour::LAAL, _colour::HARA, _colour::PEELA, _colour::NEELA };

	//! Storing references in order, starting from the starting positon of that colour, then anti-clockwise
	lockedPositions = {
		{_colour::LAAL, {ref(board[13][4]), ref(board[13][1]), ref(board[10][1]), ref(board[10][4])}},
		{_colour::HARA, {ref(board[4][1]), ref(board[1][1]), ref(board[1][4]), ref(board[4][4])}},
		{_colour::PEELA, {ref(board[1][10]), ref(board[1][13]), ref(board[4][13]), ref(board[4][10])}},
		{_colour::NEELA, {ref(board[10][13]), ref(board[13][13]), ref(board[13][10]), ref(board[10][10])}} };
	//! Marking the LockedPositions
	for( auto& lockArea : lockedPositions ){
		for( auto& lock : lockArea.second )
			lock.get().box_type = Box::LOCK;
	}

	coloursMap = {
		{_colour::LAAL, Player::_1},
		{_colour::HARA, Player::_1},
		{_colour::PEELA, Player::_3},
		{_colour::NEELA, Player::_4} };

	curr_player = Player::_1;
	curr_colour = _colour::LAAL;
	number_of_GameRuns = 0;

	for( auto&& stop : _ludo_coords.stops ){
		getBoardBox(stop).content = 'X';
	}
}

game::~game(){}
