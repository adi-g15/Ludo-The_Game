#include <algorithm>
#include <set>
#include "thinker.hpp"
#include "game.hpp"

using std::find;

thinker::thinker(game *original)
{
	this->state = new ludo_state(original);
	this->original = original;
}

thinker::~thinker()
{
	delete state;
}

bool thinker::unlock()
{
	state_goti *goti = nullptr;

	for (auto &coord : state->lockedPositions)
	{
		auto &box = state->getBox(coord);

		if (goti != nullptr && state->board[coord.y][coord.x].type != Box::LOCK)
			break;
		else if (!box.inBoxGotis.empty())
			continue;
		for (auto i = box.inBoxGotis.begin(); i != box.inBoxGotis.end(); ++i)
		{
			if ((*i)->gotiColour == state->currColour)
			{
				goti = box.removeGoti(*i);
				break;
			}
		}
	}

	state->getBox(this->_ludo_coords.start_coords[state->currColour]).appendGoti(goti);
	state->movingColours[state->currColour].push_back(this->_ludo_coords.start_coords[state->currColour]);

	return true;
}

Direction thinker::getDirOfMovement(const coord &coord)
{
	Direction retVal;
	if (coord.y < (15 / 2 - 2) || coord.x > (15 / 2 - 2))
	{
		if (coord.y == 0)
			retVal = Direction::LEFT;
		else if (coord.y == (15 - 1))
			retVal = Direction::RIGHT;
		else if (coord.x < 15 / 2)
			retVal = Direction::UP;
		else
			retVal = Direction::DOWN;
	}
	else
	{
		if (coord.x == 0)
			retVal = Direction::UP;
		else if (coord.x == (15 - 1))
			retVal = Direction::DOWN;
		else if (coord.y > (15 / 2))
			retVal = Direction::LEFT;
		else
			retVal = Direction::RIGHT;
	}
	return retVal;
}

const std::optional<_smartMoveData> thinker::isMovePossible(const coord &coords, int dist) const
{
	if( dist == 0 )
		return {};

	coord increment_coord{ 0, 0 };
	coord updated_coords{ coords };

	Direction turnDir, currDir = this->getDirOfMovement(coords);
	int moveProfit(0);

	auto currBox = std::cref(state->getBox(updated_coords)); // using reference wrapper to allow it to be changed to refer something else

	/*Move Profits ->
	    Move forward one box +1
	    Home +2
	    Stops +3
	    Attack +4
	    Crosses enemy -3 (ie. slightly more attacking)
	*/
	while (dist--)
	{
		increment_coord = {0, 0};
		turnDir = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.outer_corners); //! For Outer Corners
		if (turnDir != Direction::NO_TURN)
		{ //! ie. a turn will happen to go to next box
			currDir = turnDir;
			if (currDir == Direction::UP)
			{
				increment_coord = {-1, 0};
			}
			else if (currDir == Direction::LEFT)
			{
				increment_coord = {0, 1};
			}
			else if (currDir == Direction::RIGHT)
			{
				increment_coord = {0, -1};
			}
			else if (currDir == Direction::DOWN)
			{
				increment_coord = {1, 0};
			}
		}
		else
		{
			turnDir = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.inner_turns); //For Inner Turns

			if (turnDir != Direction::NO_TURN)
			{
				currDir = turnDir;
				if (currDir == Direction::UP)
				{
					increment_coord = {-1, 1};
				}
				else if (currDir == Direction::LEFT)
				{
					increment_coord = {1, 1};
				}
				else if (currDir == Direction::RIGHT)
				{
					increment_coord = {-1, -1};
				}
				else if (currDir == Direction::DOWN)
				{
					increment_coord = {1, -1};
				}
			}
			else
			{
				//Checking for Home_Turns
				if (updated_coords == _ludo_coords.homePath_turns.at(state->currColour).first)
				{
					currDir = _ludo_coords.homePath_turns.at(state->currColour).second;
				}

				//! ie. needs to 'go straight' on its current path
				if (currDir == Direction::UP)
					increment_coord = {-1, 0};
				else if (currDir == Direction::LEFT)
					increment_coord = {0, 1};
				else if (currDir == Direction::RIGHT)
					increment_coord = {0, -1};
				else if (currDir == Direction::DOWN)
					increment_coord = {1, 0};
			}
		}

		updated_coords.y += increment_coord.y;
		updated_coords.x += increment_coord.x;
		currBox = state->getBox(updated_coords);

		//Judging the profit START
		moveProfit += ProfitType::NORMAL_MOVE;

		if (currBox.get().areOpponentsPresent(state->currColour) && dist != 0)
		{
			moveProfit += ProfitType::ProfitType::CROSSES_ENEMY;
		}
		//Judging the profit START

		if (currBox.get().type == Box::HOME_END && dist != 0)
		{ //! Reached finished point, but move still incomplete
			return {};
		}
	}

	if (currBox.get().areOpponentsPresent(state->currColour) && currBox.get().type == Box::NORMAL)
		moveProfit += ProfitType::ATTACK;
	else if (currBox.get().type == Box::STOP)
		moveProfit += ProfitType::REACH_STOP;

	return _smartMoveData({ updated_coords, currDir, moveProfit });
}

bool thinker::implementBestMove()
{
	if (!this->bestMove_available)
	{
		this->setBestMove();
	}

	state->resetBoard();
	if (!state->isInSync(original))
		return false;
	else if (!this->bestMove_available)
	{
		if (this->setBestMove() == false)
			return false; //ie. bestcombination not available
	}

	//Now moving using the bestcombination found ;D
	for (auto &p : bestMove)
	{
		if (p.second == 0) //Ie. unlock a goti here
		{
			original->unlockGoti();
			continue;
		}

		auto move_possibility = original->isMovePossible(original->movingGotis.at(state->currColour)[p.first], p.second);
		auto is_possible = move_possibility.has_value();
		auto smart_data = move_possibility.value();
		if (!is_possible)
			return false;

		//more conditions maybe added here
		original->moveGoti(original->movingGotis.at(state->currColour)[p.first], smart_data);
		//Won't handle_moveVal() here since it is an auto-move, so we don't need additional messages
	}

	this->bestMove_available = false;
	this->bestMove.clear();
	return true;
}

std::vector<combination> thinker::getBestMove()
{
	if (!this->bestMove_available)
		this->setBestMove();
	return this->bestMove;
}

bool thinker::setBestMove()
{
	std::vector<_dieVal> dieNumbers;
	std::map<std::vector<combination>, int> completeMoves;

	int maxProfit = 0;

	Die::getDieResult(dieNumbers);

	std::set<_dieVal> usedRolls;

	if (state->movingColours.at(state->currColour).empty())
	{
		if (find(dieNumbers.begin(), dieNumbers.end(), 6) != dieNumbers.end())
		{
			this->unlock();
			dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
		}
		else
		{
			// "Move Not Possible";
			return false;
		}
	}

	std::vector<coord> movingPos, opponentsPos;
	for (long i = 0; i < state->board.size(); ++i)
	{
		for (long j = 0; j < state->board.at(i).size(); ++j)
		{
			if (state->board[i][j].areOpponentsPresent(state->currColour))
				opponentsPos.push_back({i, j});
		}
	}
	movingPos = state->movingColours[state->currColour];

	this->mindlessMovers(0, dieNumbers, 0, movingPos, opponentsPos, {{}, 0}); //The genesis/starter
																			  //Now we have the completeMoves vector filled
	for (auto &comb : this->completeMoves)
	{ //! Chosing the best move
		if (comb.first > maxProfit)
		{
			maxProfit = comb.first;
			this->bestMove = comb.second;
		}
	}

	this->bestMove_available = !this->bestMove.empty();
	return this->bestMove_available;
}

bool thinker::mindlessMovers(_dieVal roll, std::vector<_dieVal> dieNumbers, unsigned gotiIndex, std::vector<coord> movingColoursPos, std::vector<coord> opponentsPos, std::pair<std::vector<combination>, int> prevMoves)
{
	if (dieNumbers.empty())
	{
		//This is the time for that 'heuristic' function's task
		// m.lock();
		this->completeMoves.insert({prevMoves.second, prevMoves.first});
		// m.unlock();
		return true;
	}

	/*Updated isMovePossible to take coord instead of goti pointer...
			BENEFITS -> 1. No need to update the gotis themselves
						2. Passing a vector of coords, as movingGotis, is lighter, SAFER to pass by value to further chain*/
	auto movePossible = this->isMovePossible(movingColoursPos.at(gotiIndex), roll);
	_smartMoveData smartData;
	auto isPossible = movePossible.has_value();
	if(movePossible.has_value())
		smartData = movePossible.value();

	if (roll)
	{
		if (roll == 6)
		{
			if (isPossible)
			{
				if (smartData.moveProfit > ProfitType::UNLOCK)
				{
					auto box = state->getBox(smartData.finalCoord);
					if ( (box.type == Box::HOME_END) || (box.areOpponentsPresent(state->currColour) && box.type == Box::NORMAL))
					{
						Die::getDieResult(dieNumbers);
					}

					prevMoves.first.push_back({gotiIndex, roll});
					prevMoves.second += smartData.moveProfit;
				}
				else if (state->getNumLocks() > 0)
				{ //Maybe the move was not possible on the goti above, but if it was 6, and there are locked gotis available, lets ProfitType::UNLOCK
					prevMoves.first.push_back({gotiIndex, 0});
					prevMoves.second += ProfitType::UNLOCK;
				}
			}
			else if (state->getNumLocks() > 0)
			{ //Maybe the move was not possible on the goti above, but if it was 6, and there are locked gotis available, lets ProfitType::UNLOCK
				prevMoves.first.push_back({gotiIndex, 0});
				prevMoves.second += ProfitType::UNLOCK;
			}
		}
		else if (isPossible)
		{
			auto box = state->getBox(smartData.finalCoord);
			if ((box.type == Box::HOME_END) || (box.areOpponentsPresent(state->currColour) && box.type == Box::NORMAL))
			{
				Die::getDieResult(dieNumbers);
			}

			prevMoves.first.push_back({gotiIndex, roll});
			prevMoves.second += smartData.moveProfit;
		}
		else
		{
			return false;
		}

		dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
		if (dieNumbers.empty())
		{
			completeMoves.insert({prevMoves.second, prevMoves.first});
			return true;
		}
	}

	std::set<_dieVal> set_rolls;

	std::vector<std::thread> threads;
	for (size_t index = 0; index < movingColoursPos.size(); ++index)
	{
		for (auto r : dieNumbers)
		{
			if (set_rolls.find(r) != set_rolls.end())
				continue;
			set_rolls.insert(r);
			this->mindlessMovers(r, dieNumbers, index, movingColoursPos, opponentsPos, prevMoves);
			// threads.push_back(std::thread( thinker::mindlessMovers, this , r, dieNumbers, index, movingColoursPos, opponentsPos , prevMoves));
		}
		// for( auto& t: threads ){
		// 	if( t.joinable() )	t.join();
		// }
	}

	return true;
}

bool thinker::operator()()
{
	return this->move();
}

bool thinker::move()
{
	this->setBestMove();
	return this->implementBestMove();
}
