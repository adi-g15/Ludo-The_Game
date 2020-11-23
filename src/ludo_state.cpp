#include "game.hpp"
#include "ludo_state.hpp"
#include "thinker.hpp"
#include <iostream>	//only for a single cerr statement

using namespace std;

unsigned int ludo_state::used_count = 0;
ludo_state::ludo_state(const game *original){
	this->update(original);
}

state_box& ludo_state::getBox(const coord& coord){
	return board.at(coord.y).at(coord.x);
}

const state_box& ludo_state::getBox(const coord& coord) const{
	return board.at(coord.y).at(coord.x);
}

size_t ludo_state::getNumLocks(){
	size_t retVal = 0;
	for( auto &coord : this->lockedPositions ){
		if( board[coord.y][coord.x].type != Box::LOCK ) continue;
		else if( ! this->getBox(coord).inBoxGotis.empty() ){
				++retVal;
				break;
			}
	}
	return retVal;
}

// #include <iostream>
void ludo_state::update(const game* original){
	for( auto row=0; row < original->board.size(); ++row ){
		for( auto col=0; col < original->board[row].size(); ++col ){
			this->board.at(row).at(col).type = original->board.at(row).at(col).box_type;
			if( this->board[row][col].type == Box::UNUSABLE ) continue;
			for( auto &i : original->board[row][col].inBoxGotis ){
				this->board[row][col].appendGoti( new state_goti( i->get_gotiColour(), i->get_curr_direction(), i->getCoords()) );	//No need to attack here, since that was in the original game itself
				this->movingColours.at(i->get_gotiColour()).push_back( {row, col} );
			}
		}
	}

	this->currPlayer = original->curr_player;
	this->currColour = original->curr_colour;
}

void ludo_state::resetBoard(){
	map<_colour, vector<state_goti*>> gotis ({{_colour::LAAL, {}}, {_colour::HARA, {}}, {_colour::PEELA, {}}, {_colour::NEELA, {}}});
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
			auto goti = gotis.at(pair.first).back();
			goti->coords = coord;	//Should be enough gotis to pass through this
			goti->currDir = thinker::getDirOfMovement(coord);
			this->getBox(coord).appendGoti(goti);
			gotis[pair.first].pop_back();
		}
	}
}

bool ludo_state::isInSync(const game* original){
	try{
			//Iterating through the movingGotis of original, then for each of them, iterating through this->movingColours coords, to see if 'some' same colour goti is available at same coords, and then make the index in found vector as true, and continue
		for( auto &pair : original->movingGotis ){
			vector<bool> found(pair.second.size(), false);
			for( int j=0; j < pair.second.size(); ++j ){
				auto goti = pair.second[j];
				for ( auto &coord : this->movingColours.at(goti->get_gotiColour()) ){
					if ( goti->getCoords() == coord ){
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
	this->inBoxGotis.push_back(goti);
	return true;
}

state_goti* state_box::removeGoti(state_goti* goti){
	for ( auto &i : this->inBoxGotis ){
		if( i->gotiColour == goti->gotiColour ){
			auto iter = std::find_if(inBoxGotis.begin(), inBoxGotis.end(), [&](state_goti* g){
				return goti->coords == g->coords && goti->gotiColour == g->gotiColour;
			});
			inBoxGotis.erase(iter);
			return i;
		}
	}
	throw std::exception();	//will return NULL otherwise
}

bool state_box::areOpponentsPresent(_colour colour) const{
	for( auto &i : this->inBoxGotis ) if( i->gotiColour != colour ) return true;
	return false;
}

state_box::~state_box(){
	for( auto &goti : this->inBoxGotis ){
		delete goti;
	}
	inBoxGotis.clear();
}

state_goti::state_goti(_colour gotiColour, Direction gotiDir, coord coord) : gotiColour(gotiColour), currDir(gotiDir), coords(coord){}

state_goti::~state_goti(){}
