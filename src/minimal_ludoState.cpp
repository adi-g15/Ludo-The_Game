#include "thinker.h"
#include "game.h"

using namespace std;

unsigned int minimal_ludoState::used_count = 0;
minimal_ludoState::minimal_ludoState(const game *original){
	this->update(original);
}

state_box& minimal_ludoState::getBox(const coordinates& coord){
	return board.at(coord.first).at(coord.second);
}

const state_box& minimal_ludoState::getBox(const coordinates& coord) const{
	return board.at(coord.first).at(coord.second);
}

size_t minimal_ludoState::getNumLocks(){
	size_t retVal = 0;
	for( auto &coord : this->lockedPositions ){ 
		if( board[coord.first][coord.second].type != BOX_TYPE::_boxLOCK ) continue;
		else if( ! this->getBox(coord).inBoxGotis.empty() ){
				++retVal;
				break;
			}
	}
	return retVal;
}

#include <iostream>
void minimal_ludoState::update(const game* original){
	for( size_t row=0; row < original->board.size(); ++row ){
		for( size_t col=0; col < original->board[row].size(); ++col ){
			this->board.at(row).at(col).type = original->board.at(row).at(col).box_type;
			if( this->board[row][col].type == _boxUNUSABLE ) continue;
			for( auto &i : original->board[row][col].inBoxGotis ){
				this->board[row][col].appendGoti( new state_goti( i->get_gotiColour(), i->get_curr_direction(), i->getCoords()) );	//No need to attack here, since that was in the original game itself
				this->movingColours.at(i->get_gotiColour()).push_back( {row, col} );
			}
		}
	}

	this->currPlayer = original->currentPlayer;
	this->currColour = original->currentGotiColour;
}

void minimal_ludoState::resetBoard(){
	map<colours, vector<state_goti*>> gotis ({{ColourLAAL, {}}, {ColourHARA, {}}, {ColourPEELA, {}}, {ColourNEELA, {}}));
	for ( int i=0; i < this->board.size(); ++i ){
		for ( int j=0; j < this->board.at(i).size(); ++j ){
			auto boxGotis = this->getBox({i,j}).inBoxGotis;
			for( int k=0; k < boxGotis.size(); ++k ){
				gotis.at(boxGotis.at(k)->gotiColour).push_back(boxGotis.at(k));
				--k;
			}
		}
	}

	//Now we have removed all gotis from the board, and now will add them acocording to the original moving Colours
	for( auto &pair : this->movingColours ){
		for( auto &coord : pair.second ){
			auto goti = gotis.at(pair.first).back;
			goti->coords = coord;	//Should be enough gotis to pass through this
			goti->currDir = this->getDirOfMovement(coord);
			this->getBox(coord).appendGoti(goti);
			gotis[pair.first].pop_back();			
		}
	}
}

bool minimal_ludoState::isInSync(const game* original){
	try{
			//Iterating through the movingGotis of original, then for each of them, iterating through this->movingColours coords, to see if 'some' same colour goti is available at same coords, and then make the index in found vector as true, and continue
		for( auto &pair : original->movingGotis ){
			vector<bool> found(pair.second.size(), false);
			for( int j=0; j < pair.second.size(); ++j ){
				auto goti = pair.second[j];
				for ( auto &coord : this->movingColours.at(goti->get_gotiColour()) ){
					if ( goti.getCoords() == coord ){
						found[j] = true;
						break;
					}
				}
				if( !found[j] )	return false;
			}
		}
	}catch(std::out_of_range& e){
		cerr<<e.what()<<endl;
		return false;
	}
	return true;
}

bool state_box::appendGoti(state_goti* goti){
	return inBoxGotis.insert(goti).second;
}

state_goti* state_box::removeGoti(state_goti *goti){
	for ( auto &i : this->inBoxGotis ){
		if( i->gotiColour == goti->gotiColour ){
			inBoxGotis.erase(i);
			return i;
		}
	}
	throw std::exception();	//will return NULL otherwise
}

bool state_box::areOpponentsPresent(colours colour) const{
	for( auto &i : this->inBoxGotis ) if( i->gotiColour != colour ) return true;
	return false;
}

state_box::~state_box(){
	for( auto &goti : this->inBoxGotis ){
		delete goti;
	}
	inBoxGotis.clear();
}

state_goti::state_goti(colours gotiColour, direction gotiDir, coordinate coord) : gotiColour(gotiColour), currDir(gotiDir), coords(coord){}

state_goti::~state_goti(){}