#include <algorithm>
#include <unordered_set>
#include "thinker.h"
#include "game.h"

typedef std::pair<unsigned short, unsigned short> combination;

thinker::thinker(game* original){
	this->state = new ludo_state(original);
	this->original = original;
}

thinker::~thinker(){
	delete state;
}

// namespace Die{
// 	void rolldie(std::vector<unsigned short>& dieNumbers){
// 		dieNumbers.push_back(6);
// 		dieNumbers.push_back(1);
// 	}
// }

using std::find;

bool thinker::unlock(){
	state_goti* goti = nullptr;

	for( auto &coord : state->lockedPositions ){
		auto &box = state->getBox(coord);

		if( goti!=nullptr && state->board[coord.first][coord.second].type != BOX_TYPE::_boxLOCK ) break;
		else	if( !box.inBoxGotis.empty() ) continue;
		for( auto i = box.inBoxGotis.begin(); i != box.inBoxGotis.end(); ++i ){
			if( (*i)->gotiColour == state->currColour ){
				goti = box.removeGoti(*i);
				break;
			}
		}
	}

	state->getBox(this->_ludo_coords.start_coords[ state->currColour ]).appendGoti( goti );
	state->movingColours[state->currColour].push_back(this->_ludo_coords.start_coords[state->currColour]);

	return true;
}

direction thinker::getDirOfMovement( const coordinate& coord){
	direction retVal;
	if( coord.first < (15/2 - 2) || coord.second  > (15/2 - 2) ){
		if( coord.first == 0 )	retVal = direction::EAST;
		else if( coord.first == (15-1) )	retVal = direction::WEST;
		else if( coord.second < 15/2 )	retVal = direction::NORTH;
		else	retVal = direction::SOUTH;
	}
	else{
		if( coord.second == 0 )	retVal = direction::NORTH;
		else if( coord.second == (15-1) )	retVal = direction::SOUTH;
		else if( coord.first > (15/2) )	retVal = direction::EAST;
		else	retVal = direction::WEST;
	}
	return retVal;
}

const _moveData thinker::isMovePossible(const coordinate& coord, int dist) const{
	direction turnDir, currDir = this->getDirOfMovement(coord);

	if( dist == 0 ) return { true, { coord, currDir,0} };

	_moveData retVal;
	int moveProfit(0);

	std::pair<int,int> increment_coord( {0,0} );
	std::pair<int,int> updated_coords( coord );
	auto currBox = std::cref(state->getBox(updated_coords));	// using reference wrapper to allow it to be changed to refer something else

	/*Move Profits ->
	    Each block +1
	    Home +2
	    Stops +3
	    Attack +4
	    Crosses enemy -3 (ie. slightly more attacking)
	*/	
	while(dist--){

		increment_coord = {0,0};
		turnDir = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.outer_corners);	//! For Outer Corners
		if(turnDir != NO_TURN){	//! ie. a turn will happen to go to next box

			currDir = turnDir;
			if(currDir == NORTH){
				increment_coord = {-1,0};
			}
			else if(currDir == EAST){
				increment_coord = {0,1};
			}
			else if(currDir == WEST){
				increment_coord = {0,-1};
			}
			else if(currDir == SOUTH){
				increment_coord = {1,0};
			}
		}
		else{
			turnDir = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.inner_turns); //For Inner Turns
		
			if (turnDir != NO_TURN)
			{
				currDir = turnDir;
				if(currDir == NORTH){
					increment_coord = {-1,1};
				}
				else if(currDir == EAST){
					increment_coord = {1,1};
				}
				else if(currDir == WEST){
					increment_coord = {-1,-1};
				}
				else if(currDir == SOUTH){
					increment_coord = {1,-1};
				}
			}
			else{
				//Checking for Home_Turns
				if( updated_coords == _ludo_coords.home_turns.at(state->currColour).first )
				{
					currDir = _ludo_coords.home_turns.at(state->currColour).second;
				}

				//! ie. needs to 'go straight' on its current path
				if(currDir == NORTH)	increment_coord = {-1,0};
				else if(currDir == EAST)	increment_coord = {0,1};
				else if(currDir == WEST)	increment_coord = {0,-1};
				else if(currDir == SOUTH)	increment_coord = {1,0};
			}
		}

		updated_coords.first += increment_coord.first;
		updated_coords.second += increment_coord.second;
		currBox = state->getBox(updated_coords);

		//Judging the profit START
		moveProfit += NORMAL_MOVE;

		if( currBox.get().areOpponentsPresent(state->currColour) && dist!=0 ){
			moveProfit += CROSSES_ENEMY;
		}
		//Judging the profit START

		if( currBox.get().type == _boxHOME_END && dist!=0 ){	//! Reached finished point, but move still incomplete
			retVal.isPossible = false;
			moveProfit += REACH_HOME;

			return retVal;
		}
	}

	if( currBox.get().areOpponentsPresent(state->currColour) && currBox.get().type == _boxNORMAL )	moveProfit += ATTACK;
	else if( currBox.get().type == _boxSTOP ) moveProfit += REACH_STOP;

	retVal.isPossible = true;
	retVal.smartData.finalCoords = updated_coords;
	retVal.smartData.finalDirection = currDir;
	retVal.smartData.moveProfit = moveProfit;

	return retVal ;
}

bool thinker::implementBestMove(){
	if( !this->bestMove_available ){
		this->setBestMove();
	}

	state->resetBoard();
	if( !state->isInSync(original) ) return false;
	else if( !this->bestMove_available ){
		if ( this->setBestMove() == false ) return false;	//ie. bestcombination not available
	}

	//Now moving using the bestcombination found ;D
	for( auto& p : bestMove ){
		if ( p.second == 0 )	//Ie. unlock a goti here
		{
			original->unlockGoti();
			continue;
		}

		auto [ isPossible, smartData ] = original->isMovePossible(original->movingGotis.at(state->currColour)[p.first], p.second );
		if( !isPossible )    return false;

			//add other conditions here
		original->moveGoti(original->movingGotis.at(state->currColour)[p.first], smartData);
			//Won't handle moveVal
	}

	this->bestMove_available = false;
	this->bestMove.clear();
	return true;
}

std::vector<combination> thinker::getBestMove(){
	if( ! this->bestMove_available ) this->setBestMove();
	return this->bestMove;
}

bool thinker::setBestMove(){
	std::vector<unsigned short> dieNumbers;
	std::map<std::vector<combination>, int> completeMoves;

	int maxProfit = 0;

	Die::rolldie(dieNumbers);

	std::unordered_set<unsigned short> usedRolls; 

	if( state->movingColours.at(state->currColour).empty() ){
        if( find(dieNumbers.begin(), dieNumbers.end(),6) != dieNumbers.end() ){
			this->unlock();
			dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
		}
      else{
            // "Move Not Possible";
            return false;
        }
 	}
 
 	std::vector<coordinate> movingPos, opponentsPos;
 	for( size_t i=0; i < state->board.size(); ++i ){
	 	for( size_t j=0; j < state->board.at(i).size(); ++j ){
			if( state->board[i][j].areOpponentsPresent(state->currColour) )	opponentsPos.push_back({i,j});
		}
 	}
 	movingPos = state->movingColours[state->currColour];

    this->mindlessMovers(0, dieNumbers, 0, movingPos, opponentsPos, {{}, 0});	//The genesis/starter
        //Now we have the completeMoves vector filled
	for( auto& comb : this->completeMoves ){  //! Chosing the best move
		if( comb.first > maxProfit ){
			maxProfit = comb.first;
			this->bestMove = comb.second;
		}
	}

	this->bestMove_available = ! this->bestMove.empty();
	return this->bestMove_available;
}

bool thinker::mindlessMovers ( unsigned short roll, std::vector<unsigned short> dieNumbers, unsigned short gotiIndex, std::vector<coordinate> movingColoursPos, std::vector<coordinate> opponentsPos, std::pair<std::vector<combination>, int> prevMoves ){
	if( dieNumbers.empty() ){
		//This is the time for that 'heuristic' function's task
		// m.lock();
		this->completeMoves.insert({prevMoves.second, prevMoves.first});
		// m.unlock();
		return true;
	}

		/*Updated isMovePossible to take coordinate instead of goti pointer... 
			BENEFITS -> 1. No need to update the gotis themselves
						2. Passing a vector of coords, as movingGotis, is lighter, SAFER to pass by value to further chain*/
	auto [ isPossible, smartData ] = this->isMovePossible(movingColoursPos.at(gotiIndex), roll);
	if( roll ){
		if( roll == 6 ){
			if( isPossible ){
    			if( smartData.moveProfit > UNLOCK ){
    				auto box = state->getBox(smartData.finalCoords);
		            if( (box.type == _boxHOME_END) || (box.areOpponentsPresent(state->currColour) &&  box.type == _boxNORMAL) ){
					    Die::rolldie(dieNumbers);
					}

				    prevMoves.first.push_back({gotiIndex, roll});
				    prevMoves.second += smartData.moveProfit;

    			}else if( state->getNumLocks()>0 ){	//Maybe the move was not possible on the goti above, but if it was 6, and there are locked gotis available, lets UNLOCK
				    prevMoves.first.push_back({gotiIndex, 0});
			    	prevMoves.second += UNLOCK;
    			}
    		}
    		else if( state->getNumLocks()>0 ){	//Maybe the move was not possible on the goti above, but if it was 6, and there are locked gotis available, lets UNLOCK
			    prevMoves.first.push_back({gotiIndex, 0});
			    prevMoves.second += UNLOCK;
    		}
    	}else if( isPossible ){
			auto box = state->getBox(smartData.finalCoords);
            if( (box.type == _boxHOME_END) || (box.areOpponentsPresent(state->currColour) &&  box.type == BOX_TYPE::_boxNORMAL) ){
			    Die::rolldie(dieNumbers);
			}

		    prevMoves.first.push_back({gotiIndex, roll});
		    prevMoves.second += smartData.moveProfit;

	    }
    	else{ return false; }

        dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
	    if( dieNumbers.empty() ){
	    	completeMoves.insert({prevMoves.second, prevMoves.first});
	    	return true;
	    }
    }

    std::unordered_set<unsigned short> set_rolls;

	std::vector<std::thread> threads;
	for(size_t index=0; index < movingColoursPos.size() ; ++index){
		for( auto r : dieNumbers ){
			if( set_rolls.find(r) != set_rolls.end() )	continue;
			set_rolls.insert(r);
			this->mindlessMovers(r, dieNumbers, index, movingColoursPos, opponentsPos , prevMoves);
			// threads.push_back(std::thread( thinker::mindlessMovers, this , r, dieNumbers, index, movingColoursPos, opponentsPos , prevMoves));
		}
		// for( auto& t: threads ){
		// 	if( t.joinable() )	t.join();
		// }
    }

	return true;
}

bool thinker::operator()(){
	return this->move();
}

bool thinker::move(){
	this->setBestMove();
	return this->implementBestMove();
}