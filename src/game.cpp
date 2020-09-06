#include "game.hpp"
#include "boardPrinter.hpp"
#include "exceptions.hpp"
#include "keywords.hpp"
#include "thinker.hpp"
#include "util/find_remove.hpp"

#include <iostream>
#include <utility>
#include <algorithm>
#include <set>
#include <sstream>

using namespace std;

template <typename T1, typename T2> //Utility function
static inline std::ostream &operator<<(std::ostream &out, const std::pair<T1, T2> &p)
{
	return out << '(' << p.first << ", " << p.second << ')';
}

//GAME_CLASS_DEFINTIONS START
const _moveData game::isMovePossible(std::shared_ptr<ludo_goti> &the_goti, int dist)
{
	if (dist == 0)
		return {true, {the_goti->curr_coords, the_goti->curr_direction, 0}};

	_moveData retVal;

	std::pair<int, int> increment_coord({0, 0});
	std::pair<int, int> updated_coords(the_goti->curr_coords);
	reference_wrapper<ludo_box> currBox = getBoardBox(updated_coords);

	direction turnDirection, currDirection = the_goti->curr_direction;

	while (dist--)
	{

		increment_coord = {0, 0};
		turnDirection = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.outer_corners); //! For Outer Corners
		if (turnDirection != NO_TURN)
		{ //! ie. a turn will happen to go to next box

			currDirection = turnDirection;
			if (currDirection == NORTH)
			{
				increment_coord = {-1, 0};
			}
			else if (currDirection == EAST)
			{
				increment_coord = {0, 1};
			}
			else if (currDirection == WEST)
			{
				increment_coord = {0, -1};
			}
			else if (currDirection == SOUTH)
			{
				increment_coord = {1, 0};
			}
		}
		else
		{
			turnDirection = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.inner_turns); //For Inner Turns

			if (turnDirection != NO_TURN)
			{
				currDirection = turnDirection;
				if (currDirection == NORTH)
				{
					increment_coord = {-1, 1};
				}
				else if (currDirection == EAST)
				{
					increment_coord = {1, 1};
				}
				else if (currDirection == WEST)
				{
					increment_coord = {-1, -1};
				}
				else if (currDirection == SOUTH)
				{
					increment_coord = {1, -1};
				}
			}
			else
			{
				//Checking for Home_Turns
				if (updated_coords == _ludo_coords.home_turns[the_goti->gotiColour].first)
				{
					currDirection = _ludo_coords.home_turns[the_goti->gotiColour].second;
				}

				//! ie. needs to 'go straight' on its current path
				if (currDirection == NORTH)
				{
					increment_coord = {-1, 0};
				}
				else if (currDirection == EAST)
				{
					increment_coord = {0, 1};
				}
				else if (currDirection == WEST)
				{
					increment_coord = {0, -1};
				}
				else if (currDirection == SOUTH)
				{
					increment_coord = {1, 0};
				}
			}
		}

		updated_coords.first += increment_coord.first;
		updated_coords.second += increment_coord.second;
		currBox = getBoardBox(updated_coords);

		if (currBox.get().box_type == _boxHOME_END && dist != 0)
		{ //! Reached finished point, but move still incomplete
			retVal.isPossible = false;
			return retVal;
		}
	}

	retVal.isPossible = true;
	retVal.smartData.finalCoords = updated_coords;
	retVal.smartData.finalDirection = currDirection;

	return retVal;
}

/*Return values - -1 : Can't Move, or FAILURE
				   0 : Normal
				   1 : Goti pung gayi
				   2 : Attacked (signifies signal to give extra diceRoll to player)*/
short game::moveGoti(std::shared_ptr<ludo_goti> the_goti, unsigned int dist)
{
	return moveGoti(the_goti, isMovePossible(the_goti, dist));
}

short game::moveGoti(std::shared_ptr<ludo_goti> the_goti, _moveData moveData)
{
	return moveGoti(the_goti, moveData.smartData);
}

//For this function, the move will initially be considered 'possible', since it is meant to be called through the inline overloads
short game::moveGoti(std::shared_ptr<ludo_goti> the_goti, _smartMoveData moveData)
{
	if (!isValid(moveData.finalCoords) || getBoardBox(moveData.finalCoords).box_type == _boxUNUSABLE)
	{
		cout << "Passed coords " << moveData.finalCoords << " to moveGoti is invalid !";
		endGame("Inalid goti to moveGoti");
		return -1;
	}

	const auto [finalCoords, finalDir, moveProfit] = moveData;

	//DEBUG
	if ((movingGotis[currentGotiColour].size() + numfinishedGotis[currentGotiColour] + getNumLockedGotis(currentGotiColour)) > 4)
	{ //DEBUG
		cout << "inBoxGotis -> " << getBoardBox(_ludo_coords.start_coords[currentGotiColour]).inBoxGotis.size() << endl
			 << "Finished -> " << numfinishedGotis[currentGotiColour] << endl
			 << "LockedGotis -> " << getNumLockedGotis(currentGotiColour) << endl
			 << "movingGotis -> ";
		for (auto &i : movingGotis[currentGotiColour])
			cout << i->curr_coords << ' ';
		cout << endl;
		DEBUG_PRINTBOARD
		DEBUG_NUMERICALBOARD
		throw endApplication("Multiple gotis at start");
	}

	if (!isValid(the_goti) && !isValid(finalCoords))
		return -1;

	getBoardBox(the_goti->curr_coords).removeGoti(the_goti); //Reaches here only if move possible
	if (getBoardBox(finalCoords).box_type == _boxHOME_END)
	{

		auto &lockBox = getBoardBox(getEmptyLocks(currentGotiColour));

		if (lockBox.coords == make_pair(0, 0))
		{ //debug

			cout << "movingGotis[curr].size() = " << movingGotis[currentGotiColour].size() << endl;
			cout << "inBoxGotis in the box of the goti " << getBoardBox(the_goti->curr_coords).inBoxGotis.size() << endl;
			DEBUG_PRINTBOARD
			DEBUG_NUMERICALBOARD
			cout << endl;
			endGame("Invalid lockbox");
		}

		//Removing from movingGotis
		auto iter = find(movingGotis[currentGotiColour].begin(), movingGotis[currentGotiColour].end(), the_goti);
		if (iter != movingGotis[currentGotiColour].end())
		{
			movingGotis[currentGotiColour].erase(iter);
			the_goti.reset(); //REMOVING ALL REFERENCES TO THE GOTI
		}
		else
			cout << "Goti not found";

		lockBox.content = ":D";
		lockBox.box_type = _boxUNUSABLE;
		// cout<<"DEBUG HOME MID and currentGotiColour="<<currentGotiColour<<" and size of numfinishedGotis="<<numfinishedGotis[currentGotiColour]<<endl;

		if ((movingGotis[currentGotiColour].size() + numfinishedGotis[currentGotiColour] + getNumLockedGotis(currentGotiColour)) > 4)
		{
			cout << "inBoxGotis -> ";
			for (auto &i : getBoardBox(the_goti->curr_coords).inBoxGotis)
				cout << i->curr_coords << " ";
			cout << endl
				 << "Finished -> " << numfinishedGotis[currentGotiColour] << endl
				 << "LockedGotis -> " << getNumLockedGotis(currentGotiColour) << endl
				 << "movingGotis -> ";
			for (auto &i : movingGotis[currentGotiColour])
				cout << i->curr_coords << ' ';
			cout << endl;
			DEBUG_PRINTBOARD
			DEBUG_NUMERICALBOARD
			endGame("eXTRA GOTIS of colour " + std::to_string(currentGotiColour) + " punGING");
		}

		++numfinishedGotis.at(currentGotiColour);

		return 1;
	}
	else
	{
		the_goti->curr_coords = finalCoords;
		the_goti->curr_direction = finalDir;

		if (getBoardBox(the_goti->curr_coords).appendGoti(the_goti) == 0)
		{ //appendGoti() signalled an Attack needed
			attack({}, the_goti);

			return 2;
		}
	}
	return 0;
}

bool game::handleMoveVal(short moveVal, vector<_dieVal> &dieNumbers, bool isRobot)
{ //! Messages won't print for robots
	try
	{
		if (moveVal == 1 || moveVal == 2)
		{
			if (!isRobot)
			{
				cout << "\nPress enter to roll the die once more...";
				cin.get();
			}
			Die::rolldie(dieNumbers);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

/*BUG - For example for a random_goti_index the goti isn't movable, then that diceNumber is being discarded. [For eg. in 6,1 only 1 is moved and 6 not] */
bool game::autoMove()
{ //! Return values same as the moveGoti_function
	std::vector<_dieVal> dieNumbers;
	std::set<unsigned> triedGotis_indices; //! Stores the gotis that have been tried to move, FOR A SINGLE DIE_NUMBER
	_moveData moveObj;
	bool wasSuccess = false;

	Die::rolldie(dieNumbers);
	cout << "Roll Output : ";
	for (auto &i : dieNumbers)
		cout << i << ' ';
	cout << endl;

	if (movingGotis[currentGotiColour].empty())
	{
		auto iter = find(dieNumbers.begin(), dieNumbers.end(), 6);
		if (iter != dieNumbers.end())
		{
			unlockGoti();
			dieNumbers.erase(iter);
		}
		else
		{
			clog << "Can't Move, due to absence of gotis" << endl;
			return false;
		}
	}

	int index = 0;
	// for( auto currentRoll : dieNumbers ){	//Outer loop iterates through the gotis
	//BUG_solved_for_now - See the bugs.txt file for this
	for (size_t i = 0; i < dieNumbers.size(); ++i)
	{
		auto currentRoll = dieNumbers[i];
		++index;

		updateDisplay();
		util::pause(1);

		if (currentRoll == 6)
		{
			if (rand() % 2 == 1 && getNumLockedGotis(currentGotiColour) > 0)
			{ //50% chance of robot using 6 to unlockGoti
				unlockGoti();
				continue;
			}
		}

		do
		{ //! ie. gotis still remaining to try
			if (movingGotis[currentGotiColour].empty())
				return false;
			unsigned random_goti_index = rand() % (movingGotis[currentGotiColour].size()); // Randomly selected a goti to move this much units

			try
			{
				if (triedGotis_indices.size() == movingGotis.at(currentGotiColour).size())
				{
					cout << currentRoll << " can't be utilized for any move" << endl;
					break; //Break out to the outer loop, thus rejecting currentRoll
				}

				moveObj = isMovePossible(movingGotis.at(currentGotiColour).at(random_goti_index), currentRoll);
				if (moveObj.isPossible)
				{
					wasSuccess = true;
					auto moveVal = moveGoti(movingGotis.at(currentGotiColour).at(random_goti_index), moveObj);
					handleMoveVal(moveVal, dieNumbers);

					util::pause(0.7f);
				}
			}
			catch (std::out_of_range &e)
			{
				cerr << e.what();
				endGame("AutoMove->Out_Of_Range exception");
			}

			if (!movingGotis[currentGotiColour].empty())
				random_goti_index = std::rand() % (movingGotis[currentGotiColour].size()); //Since, AFTER FINISH, MOVINGGOTIS maybe empty by this time, causing FLOATING POINT EXCEPTION

			triedGotis_indices.insert(random_goti_index);

		} while (!moveObj.isPossible && !movingGotis[currentGotiColour].empty());

		triedGotis_indices.clear();
	}

	return wasSuccess;
}

void game::attack(std::vector<_colour> coloursToRemove, std::shared_ptr<ludo_goti> attacker)
{
	if (coloursToRemove.empty())
	{ //Will consider all other _colour as opponents
		coloursToRemove.insert(coloursToRemove.begin(), {ColourNEELA, ColourHARA, ColourPEELA, ColourLAAL});
		coloursToRemove.erase(find(coloursToRemove.begin(), coloursToRemove.end(), currentGotiColour)); //Using currentGotiPlayer and not attacker's colour, since only the currentPlayer should be able to attack
	}

	ludo_box &currBox = getBoardBox(attacker->curr_coords);

	for (unsigned i = 0; i < currBox.inBoxGotis.size(); ++i)
	{ //Caution - May be a probable cause for iterator invalidation
		if (std::find(coloursToRemove.begin(), coloursToRemove.end(), currBox.inBoxGotis[i]->gotiColour) != coloursToRemove.end())
		{ //! emulating "if gotiColour in colourToRemove"
			lockGoti(currBox.inBoxGotis[i]);
			--i;
		}
	}
}

bool game::isValid(const std::pair<int, int> &coords) const
{
	if (coords.first >= 0 && coords.first < 15 && coords.second >= 0 && coords.second < 15)
	{
		return true;
	}

	endGame("Invalid Coords, in isValid");
	return false;
}

bool game::isValid(const shared_ptr<ludo_goti> &goti) const
{
	if (goti->gotiColour == currentGotiColour && isValid(goti->curr_coords))
	{
		return true;
	}
	cerr << "GOTI COLOUR DOESN'T RELATE TO CURRENT_PLAYER" << endl;
	endGame();
	return false;
}

ludo_box &game::getBoardBox(const std::pair<int, int> &coords)
{
	if (isValid(coords))
		return board[coords.first][coords.second];
	else
		throw std::out_of_range("OUT_OF_RANGE: Board indices, passed to getBoardBox, are out of bounds");
}

bool game::unlockGoti()
{
	if (currentGotiColour == UnknownColour)
	{
		cerr << "unlockGoti() request made for Unknown Colour" << endl;
		return false;
	}
	else if (movingGotis[currentGotiColour].size() >= goti_per_user)
	{
		cerr << "Goti request makes the number of ludo_goti of " << stringify(currentGotiColour) << " more than " << goti_per_user << endl;
		return false;
	}
	else
	{

		//First std::find_if gives us the raw reference of box
		auto box_iter = std::find_if(lockedPositions[currentGotiColour].begin(), lockedPositions[currentGotiColour].end(), [&](auto &lockedBox) {
			return !lockedBox.get().inBoxGotis.empty() && !lockedBox.get().areOpponentsPresent(currentGotiColour);
		});

		if (box_iter == lockedPositions[currentGotiColour].end())
		{	  //no locked goti present
			{ //DEBUG block
				cout << "No locked goti was present" << endl;
				cout << "inBoxGotis -> " << getBoardBox(_ludo_coords.start_coords[currentGotiColour]).inBoxGotis.size() << endl
					 << "Finished -> " << numfinishedGotis[currentGotiColour] << endl
					 << "LockedGotis -> " << getNumLockedGotis(currentGotiColour) << endl
					 << "movingGotis -> ";
				for (auto &i : movingGotis[currentGotiColour])
					cout << i->curr_coords << ' ';
				cout << endl;
				DEBUG_PRINTBOARD
				DEBUG_NUMERICALBOARD
				throw endApplication("Multiple gotis at start");
			}
			return false;
		}

		cout << "\nGoti to unlock from " << box_iter->get().coords << endl;

		auto goti = box_iter->get().inBoxGotis.back(); //Causing the SEG_Fault
		box_iter->get().removeGoti(goti);

		goti->curr_coords = _ludo_coords.start_coords[currentGotiColour];
		goti->curr_direction = _ludo_coords.start_dir[currentGotiColour];

		getBoardBox(goti->curr_coords).appendGoti(goti);
		movingGotis[currentGotiColour].push_back(goti);

		if ((getBoardBox(_ludo_coords.start_coords[currentGotiColour]).inBoxGotis.size() + numfinishedGotis[currentGotiColour] + getNumLockedGotis(currentGotiColour)) > 4 + 1)
		{ //DEBUG
			cout << "inBoxGotis -> " << getBoardBox(_ludo_coords.start_coords[currentGotiColour]).inBoxGotis.size() << endl
				 << "Finished -> " << numfinishedGotis[currentGotiColour] << endl
				 << "LockedGotis -> " << getNumLockedGotis(currentGotiColour) << endl
				 << "movingGotis -> ";
			for (auto &i : movingGotis[currentGotiColour])
				cout << i->curr_coords << ' ';
			cout << endl;
			DEBUG_PRINTBOARD
			DEBUG_NUMERICALBOARD
			throw endApplication("Multiple gotis at start");
		}

		return true;
	}
}

unsigned game::getNumLockedGotis(_colour gotiColour)
{
	auto num = 0;
	for (auto const &lockedBox : lockedPositions.find(gotiColour)->second)
	{
		if (!lockedBox.get().isEmpty() && lockedBox.get().box_type == _boxLOCK)
		{
			++num;
		}
	}
	return num;
}

bool game::lockGoti(std::shared_ptr<ludo_goti> goti_to_lock)
{ //wrong gotiColour goti called to be removed, currentPlayer was different
	cout << "BEFORE MovingGotis -> ";
	for (auto &i : movingGotis[currentGotiColour])
		cout << i->curr_coords << ' ';
	cout << endl;

	if (!isValid(goti_to_lock->curr_coords))
		return false;

	goti_to_lock->curr_direction = NO_TURN;

	getBoardBox(goti_to_lock->curr_coords).removeGoti(goti_to_lock);
	auto iter = find(movingGotis[goti_to_lock->gotiColour].begin(), movingGotis[goti_to_lock->gotiColour].end(), goti_to_lock);

	if (iter == movingGotis[goti_to_lock->gotiColour].end())
	{
		cout << "ERROR => Trying to lock a non-moving Goti" << endl;
		// cout<<"goti_to_lock -> "<<goti_to_lock->curr_coords;
		cout << "goti_to_lock Data-> " << goti_to_lock->curr_coords << " " << goti_to_lock->gotiColour << ' ' << goti_to_lock->curr_direction << endl;
		for (auto &i : movingGotis[currentGotiColour])
			if (goti_to_lock->curr_coords == i->curr_coords)
				cout << "Details of goti in movingGotis -> " << i->gotiColour << ' ' << i->curr_direction;
		cout << endl;
		endGame("Invalid Request to lockGoti");
	}
	else
		movingGotis[goti_to_lock->gotiColour].erase(iter);

	auto &&lockBox = std::find_if(lockedPositions[goti_to_lock->gotiColour].begin(), lockedPositions[goti_to_lock->gotiColour].end(), [](std::reference_wrapper<ludo_box> &box) {
		return box.get().isEmpty();
	});

	lockBox->get().appendGoti(goti_to_lock);

	cout << "AFTER MovingGotis -> ";
	for (auto &i : movingGotis[currentGotiColour])
		cout << i->curr_coords << ' ';
	cout << endl;
	return true;
}

void game::takeIntro()
{
	std::pair<int, int> tmpDimen(0, 0);
	tmpDimen = util::getTerminalDimen();

	util::place_v_center("[NOTICE] Please ensure window is at least 31*31");
	cout << endl;

	//QUESTION - How to call the function pointer through an iterator ?

	do
	{
		tmpDimen = util::getTerminalDimen();

	} while (tmpDimen.first < 31 || tmpDimen.second < 62);	//tmpDimen -> row*column

	_BoardPrinter::titleBar(tmpDimen.second);

	util::place_v_center(tmpDimen.first - 7, "");
	util::align_text_center(tmpDimen.second, "Enter names of the Players (Leave empty if not playing, or type \"ROBOT\") : ");

	string playerName;
	_colour colour;
	auto p = Player1;
	auto numRobots = 0, numThinkers = 0;

	colour = colourOrder[0];

	while (p <= Player4)
	{

		util::place_center(tmpDimen.second, string("Player") + to_string(p) + string(" - "));

		getline(cin, playerName, '\n');
		util::trim(playerName);
		if (playerName != "")
		{
			if (shortcutsMap.find(playerName) != shortcutsMap.end())
			{
				shortcutsMap.find(playerName)->second; //Calling using the function pointer
			}
			else if (util::icompare(playerName, robot_keyword))
			{
				++numRobots;
				activePlayerMap.insert({{p, {"ROBOT " + to_string(numRobots), colour}}});
				robotPlayers.insert({p, RobotKind::randomRobo});
			}
			else if (util::icompare(playerName, thinker_keyword))
			{
				++numThinkers;
				activePlayerMap.insert({{p, {"Thinker " + to_string(numThinkers), colour}}});
				robotPlayers.insert({p, RobotKind::thinkerRobo});
			}
			else
			{
				util::trim(playerName);
				activePlayerMap.insert({{p, {playerName, colour}}});
			}
		}

		p = player(p + 1);
		colour = _colour(colour + 1);
	}

	//Setting the board, on basis of given players
	for (auto &&player : activePlayerMap)
	{
		auto gotiColour = player.second.second;
		for (auto &&lockedBox : lockedPositions[gotiColour])
		{
			lockedBox.get().appendGoti(std::shared_ptr<ludo_goti>(new ludo_goti(gotiColour, getEmptyLocks(gotiColour))));
			lockedBox.get().box_type = _boxLOCK;
		}
		coloursMap.insert({player.second.second, player.first});
		numfinishedGotis.insert({player.second.second, 0});
	}

	for (auto &&stop : _ludo_coords.stops)
	{
		getBoardBox(stop).content = 'X';
	}
}

bool game::gameisFinished()
{
	for (auto &p : activePlayerMap)
	{
		if (numfinishedGotis[p.second.second] != 4)
			return false;
	}
	return true;
}

bool game::isPlayerPlaying(player p)
{
	return activePlayerMap.find(p) != activePlayerMap.end();
}

vector<_dieVal> Die::rolldie()
{

	std::vector<_dieVal> v;

	_dieVal dieNum = Die::dist[rand() % 4](Die::mt[rand() % 4]);
	do
	{
		v.push_back(dieNum);
		dieNum = Die::dist[rand() % 4](Die::mt[rand() % 4]);
	} while (dieNum == 6);
	v.push_back(dieNum); //To insert the last left non-6

	//! Main logic is above only, below are some cleansing and optimisations
	std::vector<size_t> sixPos;
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (v[i] == 6)
			sixPos.push_back(i);
	}
	while (sixPos.size() > 3)
	{
		v.erase(v.begin() + sixPos[sixPos.size() - 1]); //Removing from end positions
		v.erase(v.begin() + sixPos[sixPos.size() - 2]);
		v.erase(v.begin() + sixPos[sixPos.size() - 3]);
		sixPos.erase(sixPos.begin() + sixPos.size() - 3, sixPos.end());
	}

	return v;
}

void Die::rolldie(vector<_dieVal> &Vec)
{

	std::vector<_dieVal> tmpVec;

	_dieVal dieNum = Die::dist[rand() % 4](Die::mt[rand() % 4]);
	if (dieNum != 6)
	{ //To prevent cases like, dieNumbers = {4, 6} (Think! It IS possible in the do-while)
		Vec.push_back(dieNum);
		return;
	}
	do
	{
		tmpVec.push_back(dieNum);
		dieNum = Die::dist[rand() % 4](Die::mt[rand() % 4]);
	} while (dieNum == 6);
	tmpVec.push_back(dieNum); //To insert the last left non-6

	//! Main logic is above only, below are some cleansing and optimisations
	std::vector<size_t> sixPos; //Temporary set, used in 'Cleaning cases of 3 sixes'
	for (size_t i = 0; i < tmpVec.size(); ++i)
	{
		if (tmpVec[i] == 6)
			sixPos.push_back(i);
	}
	while (sixPos.size() > 3)
	{															  //ie. more than 3 sixes existing
		tmpVec.erase(tmpVec.begin() + sixPos[sixPos.size() - 1]); //Removing from end positions
		tmpVec.erase(tmpVec.begin() + sixPos[sixPos.size() - 2]);
		tmpVec.erase(tmpVec.begin() + sixPos[sixPos.size() - 3]);
		sixPos.erase(sixPos.begin() + sixPos.size() - 3, sixPos.end());
	}

	Vec.insert(Vec.end(), tmpVec.begin(), tmpVec.end());
}

void game::updateDisplay()
{
	unsigned int boxlen = 0;
	std::pair<int, int> tmpDimen(0, 0);

	do
	{
		tmpDimen = util::getTerminalDimen();
		boxlen = (2 * min(tmpDimen.first, tmpDimen.second) - 32) / 15;

	} while (min(tmpDimen.first, tmpDimen.second) < 32);

	_BoardPrinter::titleBar(tmpDimen.first);

	util::place_center(tmpDimen.first - 15 * (boxlen + 1) + 3 - 4);
	cout << "  ";
	for (size_t i = 0; i < 10; i++)
	{
		for (auto j = boxlen; j-- > 0;) //NOTE - '-->' is a combination of the 2 operators '--' and '>' ;D
			cout << ' ';
		cout << i;
	}
	for (size_t i = 10; i < 15; i++)
	{
		for (auto j = boxlen - 1; j-- > 0;) //NOTE - '-->' is a combination of the 2 operators '--' and '>' ;D
			cout << ' ';
		cout << i;
	}
	cout << "\n  ";
	util::place_center(tmpDimen.first - 15 * (boxlen + 1) + 3 - 4);

	for (size_t i = 0; i < (boxlen + 1) * 15 + 1; i++)
		cout << '-';
	cout << "\n";

#define BOARD_CENTER customUtil::place_center(tmpDimen.first - 15 * (boxlen + 1) + 3 - 4);

	_BoardPrinter _board_printer(board);
	_board_printer.boxlen = boxlen;

	BOARD_CENTER cout << 0 << ' ';
	_board_printer.row_type1(0);
	_board_printer.inter_type1();
	BOARD_CENTER cout << 1 << ' ';
	_board_printer.row_type2(1);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 2 << ' ';
	_board_printer.row_type1(2);
	_board_printer.inter_type3();
	BOARD_CENTER cout << 3 << ' ';
	_board_printer.row_type1(3);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 4 << ' ';
	_board_printer.row_type2(4);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 5 << ' ';
	_board_printer.row_type1(5);
	_board_printer.inter_type4();

	BOARD_CENTER cout << 6 << ' ';
	_board_printer.row_type3(6);
	_board_printer.inter_type5();
	BOARD_CENTER cout << 7 << ' ';
	_board_printer.row_type4(7);
	_board_printer.inter_type5();
	BOARD_CENTER cout << 8 << ' ';
	_board_printer.row_type3(8);
	_board_printer.inter_type4();

	BOARD_CENTER cout << 9 << ' ';
	_board_printer.row_type1(9);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 10;
	_board_printer.row_type2(10);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 11;
	_board_printer.row_type1(11);
	_board_printer.inter_type3();
	BOARD_CENTER cout << 12;
	_board_printer.row_type1(12);
	_board_printer.inter_type2();
	BOARD_CENTER cout << 13;
	_board_printer.row_type2(13);
	_board_printer.inter_type1();
	BOARD_CENTER cout << 14;
	_board_printer.row_type1(14);

	cout << "  ";
	BOARD_CENTER for (size_t i = 0; i < (boxlen + 1) * 15 + 1; i++) cout << '-';

#undef BOARD_CENTER

	cout << "\n\n";
	util::align_text_center(activePlayerMap[currentPlayer].first);
	cout << "\n";
	for (int i = 0; i < tmpDimen.first; ++i)
		cout << '-';
	cout << '\n';
}

std::pair<int, int> game::getEmptyLocks(_colour gotiColour) const
{
	for (auto const &lockedBox : lockedPositions.find(gotiColour)->second)
	{
		if (lockedBox.get().isEmpty() && lockedBox.get().box_type == _boxLOCK)
		{
			return lockedBox.get().coords;
		}
	}
	return {0, 0}; //ie. no locked gotis of this colour
}

bool game::InitGame(short playerConsent)
{ //! 1 for complete reset, 2 is with previous playerMap

	if (playerConsent != 1 && playerConsent != 2)
	{
		return false;
	}
	else
	{
		goti_per_user = 4;

		for (size_t r = 0; r < board.size(); ++r)
		{
			for (size_t c = 0; c < board.at(r).size(); ++c)
			{
				board[r][c].content.clear();
				board[r][c].inBoxGotis.clear();
			}
		}

		if (playerConsent == 1)
		{ //not changed, in case it is 2
			activePlayerMap.clear();
			coloursMap.clear();

			takeIntro();
		}

		//coloursMap and activePlayerMap initialised by takeIntro()
	}

	++number_of_GameRuns;

	return true;
}

/*[DONE]TODO - When there's only a single goti, and no 6, move the goti automatically*/
/*TODO - [For functionality for directly entering dieNumber if only 1 'movable' goti available] Also inside when loop for checking if choice_num is correct, add another condition, to check if there is only one goti available to move(not matters whether 6inRoll or not), if thats true, and the choice_num is in dieNumbers, considering enteredRoll=choice_num and choice_num=1*/
void game::play(bool boolVal)
{
	if (!boolVal)
		return;

	currentGotiColour = colourOrder[0];
	currentPlayer = coloursMap[currentGotiColour];

	std::vector<_dieVal> dieNumbers;

	//Lambda Definti ons
	auto lambda_next = [&]() {
		std::vector<player> tmpVec;
		for (auto &&i : activePlayerMap)
		{
			tmpVec.push_back(i.first);
		}
		size_t pos = std::find(tmpVec.begin(), tmpVec.end(), currentPlayer) - tmpVec.begin();
		do
		{
			if (pos + 1 < tmpVec.size())
				++pos;
			else
				pos = 0;
			currentPlayer = tmpVec[pos];

			currentGotiColour = activePlayerMap[currentPlayer].second;

		} while (!gameisFinished() && this->numfinishedGotis[currentGotiColour] == this->goti_per_user);

		dieNumbers.clear();
	};
	auto lambda_isSIXinRoll = [&]() {
		return std::find(dieNumbers.begin(), dieNumbers.end(), 6) != dieNumbers.end();
	};
	//Lambda Defintions

	while (!this->gameisFinished())
	{ //The gameplay loop
		_dieVal enteredRoll;
		unsigned choice_num;
		bool isRobot = robotPlayers.find(currentPlayer) != robotPlayers.end();
		string inputStr; //! Note - To use shortcuts like ":settings", ":rules, and to have 'variable number of inputs'
		updateDisplay();
		if (isRobot)
		{ //ie. It is a Robot Player

			// auto robo = thinker(this);

			// if( robotPlayers[currentPlayer] == RobotKind::thinkerRobo )
			// 	robo.implementBestMove();
			// else if( robotPlayers[currentPlayer] == RobotKind::randomRobo )
			autoMove();
			lambda_next();
			continue;
		}

		cout << "\nPress Enter to roll the die...";
		cin.get();
		Die::rolldie(dieNumbers);
		cout << "\nRoll Output - ";
		for (auto &&outNum : dieNumbers)
		{
			cout << outNum << ' ';
		}

		//This whole if-else nested block is for "AUTOMATIC DECISIONS", eg. moving a single goti possible to move
		//Automating Condition #1
		if (movingGotis[currentGotiColour].empty())
		{
				//no six
			if ( ! util::contains<unsigned>(dieNumbers, 6) )
			{ //No movable goti, so continue to next player
				cout << "\nNo movable goti, for the moves" << endl;
				util::pause(1.0f);

				lambda_next();
				continue;
			}
			else
			{ //No movable gotis currently, hence automatically open one, since 6 is available
				util::pause(0.4f);
				unlockGoti();
				updateDisplay();
				cout << "Goti unlocked!" << endl;
				util::pause(0.8f);
				dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
			}
		}
		//! Actual logic to move each player is this nested while loop
		while (!dieNumbers.empty() && !movingGotis[currentGotiColour].empty())
		{

			updateDisplay();
			unsigned counter = 1;
			cout << "\nChose from these gotis : ";
			if (lambda_isSIXinRoll())
				cout << "\n0. Unlock New Goti (just type 0)\n\n";
			for (auto &&goti_ptr : movingGotis[currentGotiColour])
			{
				cout << counter << ". [" << goti_ptr->curr_coords.first << "][" << goti_ptr->curr_coords.second << "]\n";
				++counter;
			}
			cout << "\nRoll Output - ";
			for (auto &&outNum : dieNumbers)
			{
				cout << outNum << ' ';
			}

			//The Automating Condition #2
			auto continue_OuterWhile = false; //To break out of outer loop, from inside an inner loop, BCZ coninue INSIDE THIS WILL NOT SHIFT TO THE NEXT PLAYER
			while (movingGotis[currentGotiColour].size() == 1 && dieNumbers[0] != 6 && dieNumbers.size() == 1)
			{
				auto moveData = isMovePossible(movingGotis[currentGotiColour][0], dieNumbers[0]);
				if (!moveData.isPossible)
				{
					cout << "Move not possible" << endl;
					util::pause(0.6f);
				}
				else
				{ //goti has been moved
					auto moveVal = moveGoti(movingGotis[currentGotiColour][0], moveData);
					handleMoveVal(moveVal, dieNumbers, false);

					updateDisplay();
					util::pause(1);
				}
				dieNumbers.erase(dieNumbers.begin()); //! Removing the used goti number (which is actually dieNumbers[0])
				lambda_next();
				continue_OuterWhile = true;
			}
			if (continue_OuterWhile)
				continue;

			cout << "\nEnter Goti and dieNumber : ";
			do
			{
				getline(cin, inputStr);
				util::trim(inputStr);
			} while (inputStr.empty());

			//! This is just a simple block, for separating the process, of Fetching choice_num and enteredRoll
			{
				if (shortcutsMap.find(inputStr) != shortcutsMap.end())
				{
					shortcutsMap.find(inputStr)->second; //Calling using the function pointer
					return;
				}
				else
				{
					istringstream stream(inputStr);
					stream >> choice_num;
					if (choice_num == 0)
					{
						if (lambda_isSIXinRoll())
						{
							unlockGoti();
							dieNumbers.erase(std::find(dieNumbers.begin(), dieNumbers.end(), 6));
						}
						else
						{
							cout << "Chhaka to aaye pehle, tab na goti niklegi yar!\n";
							util::pause(1);
							continue; //Wrong Input
						}
					}
					else
					{ //! Usual Case - No unlock, nor any cheat/shortcut used
						stream >> enteredRoll;

						if (
							util::isSum(enteredRoll, dieNumbers) //If output is 6,5 then entering 11 is valid
							&& (choice_num <= counter)				   //Verifies Entered values are valid
						)
						{
							short moveVal = moveGoti(movingGotis[currentGotiColour][choice_num - 1], enteredRoll);
							if (moveVal == -1)
							{ //Failure
								cout << "Invalid Move" << endl;
								util::pause(1);
							}
							else
							{ //goti has been moved
								handleMoveVal(moveVal, dieNumbers);

								auto elements = util::isSumOfElements(enteredRoll, dieNumbers);
								for_each(elements.crbegin(), elements.crend(), [&](_dieVal i) {
									dieNumbers.erase(std::find(dieNumbers.begin(), dieNumbers.end(), i));
								});
								util::pause(0.5);
							}
						}
					}
				}
			}
			updateDisplay();
		}
		//! dieNumbers is again Empty
		lambda_next();
	}
	if (gameisFinished())
		_BoardPrinter::finishedScreen();
}

void game::endGame() const
{
	_BoardPrinter::finishedScreen();
	// this->~game();	//Causes segFault
	throw endApplication("A shortcut");
}

void game::endGame(string cause) const
{
	_BoardPrinter::errorScreen(cause);
	// this->~game();	//Causes segFault
	throw endApplication(cause);
}

//! Source is the name of function from which it was called
//TODO - Complete Logic for the options (Will implement after i have made a gui, or web version)
void game::settingsMenu()
{
	auto termDimen = util::getTerminalDimen();
	unsigned choice = 11;
	std::string inputStr;

	do
	{
		_BoardPrinter::titleBar(termDimen.second);

		util::place_v_center(termDimen.first - 2 * (2 + 11 + 4)); //height of 2 taken by titleBar, 11 by Choices, and 4 by Input

		util::align_text_center(termDimen.second, "1. Change 'GamePlay' Order (PlayerWise)");
		cout << '\n';
		util::align_text_center(termDimen.second, "2. Change 'Number of gotis_per_user'");
		cout << '\n';
		util::align_text_center(termDimen.second, "3. Which goti should 'Start' First?");
		cout << '\n';
		util::align_text_center(termDimen.second, "4. Assign Colours to Players");
		cout << '\n';
		util::align_text_center(termDimen.second, "5. Remove 'stops'");
		cout << '\n';
		util::align_text_center(termDimen.second, "6. Change Title shown at Top");
		cout << '\n';
		util::align_text_center(termDimen.second, "7. Change Player Names");
		cout << '\n';
		util::align_text_center(termDimen.second, "8. Reset to Original Positions");
		cout << '\n';
		util::align_text_center(termDimen.second, "9. Use different Random_Sequence for each (y/n)");
		cout << '\n';
		util::align_text_center(termDimen.second, "10. Let's Cheat... ;D (Can remove opponents gotis)");
		cout << '\n';
		util::align_text_center(termDimen.second, "[ 11. Alwida... ]");
		cout << "\n\n";

		util::align_text_center(termDimen.second, "----------------------------");
		util::align_text_center(termDimen.second - 2, "Enter Choice : ");
		cin >> choice;
	} while (choice <= 11);

	switch (choice)
	{
	case 1:
	{
		_BoardPrinter::titleBar(termDimen.second);
		util::place_v_center(termDimen.first - 2 * (2 + 1)); //height of 2 taken by titleBar, 11 by Choices, and 4 by Input

		cout << "Type playerNumbers in order (for eg. \"1432\") : ";
		cin >> inputStr;
		// util::strip(inputStr, ' ');
		// inputNum = std::stoi(inputStr);

		//TODO

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

void game::notYetImplementedScr() const
{

	_BoardPrinter::titleBar();

	util::place_v_center();
	util::align_text_center("This feature has not yet been implemented !");
}

game::game() : colourOrder({ColourLAAL, ColourNEELA, ColourPEELA, ColourHARA})
{
	number_of_GameRuns = 0;
	goti_per_user = 4;

	for (int i = 0; i < 15; ++i)
	{
		board.emplace_back();
		for (int j = 0; j < 15; ++j)
		{
			board.at(i).push_back(ludo_box({i, j}));
		}
	}

	shortcutsMap.insert({

		{":r", &game::updateDisplay},
		// {":refresh", &game::updateDisplay},
		// {":set", &game::settingsMenu },
		// {":settings", &game::settingsMenu},
		// {":q", &game::endGame },
		// {":quit", &game::endGame },
		// {":exit", &game::endGame }

	});

	unsigned i, j;
	//! Marking the LockRooms and the HomePath
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			board[i][j].box_type = _boxUNUSABLE;
			board[7][j].box_type = _boxHOMEPATH;
			board[7][14 - j].box_type = _boxHOMEPATH;
		}
		board[i][7].box_type = _boxHOMEPATH;
		board[14 - i][7].box_type = _boxHOMEPATH;
	}
	board[0][7].box_type = board[14][7].box_type = _boxNORMAL;
	board[7][0].box_type = board[7][14].box_type = _boxNORMAL;
	for (i = 0; i < 6; i++)
		for (j = 9; j < 15; j++)
			board[i][j].box_type = _boxUNUSABLE;
	for (i = 9; i < 15; i++)
		for (j = 0; j < 6; j++)
			board[i][j].box_type = _boxUNUSABLE;
	for (i = 9; i < 15; i++)
		for (j = 9; j < 15; j++)
			board[i][j].box_type = _boxUNUSABLE;
	//! Marking the HomeArea
	for (i = 6; i < 9; i++)
		for (j = 6; j < 9; j++)
			board[i][j].box_type = _boxUNUSABLE;
	//! Marking the Stops
	for (auto &i : _ludo_coords.stops)
	{
		getBoardBox(i).box_type = _boxSTOP;
	}
	board[6][7].box_type = board[8][7].box_type = board[7][6].box_type = board[7][8].box_type = _boxHOME_END;

	colourOrder = {ColourLAAL, ColourHARA, ColourPEELA, ColourNEELA};

	//! Storing references in order, starting from the starting positon of that colour, then anti-clockwise
	lockedPositions = {
		{ColourLAAL, {ref(board[13][4]), ref(board[13][1]), ref(board[10][1]), ref(board[10][4])}},
		{ColourHARA, {ref(board[4][1]), ref(board[1][1]), ref(board[1][4]), ref(board[4][4])}},
		{ColourPEELA, {ref(board[1][10]), ref(board[1][13]), ref(board[4][13]), ref(board[4][10])}},
		{ColourNEELA, {ref(board[10][13]), ref(board[13][13]), ref(board[13][10]), ref(board[10][10])}}};
	//! Marking the LockedPositions
	for (auto &lockArea : lockedPositions)
	{
		for (auto &lock : lockArea.second)
			lock.get().box_type = _boxLOCK;
	}

	coloursMap = {
		{ColourLAAL, Player1},
		{ColourHARA, Player1},
		{ColourPEELA, Player3},
		{ColourNEELA, Player4}};

	currentPlayer = Player1;
	currentGotiColour = ColourLAAL;
	number_of_GameRuns = 0;

	for (auto &&stop : _ludo_coords.stops)
	{
		getBoardBox(stop).content = 'X';
	}
}

game::~game() {}
