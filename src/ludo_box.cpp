#include "exceptions.hpp"
#include "keywords.hpp"
#include "ludo_box.hpp"

#include <iostream>
#include <utility>	//ONLY for creating a temp pair, in appendGoti()
#include <algorithm>
#include <array>

using namespace std;

ludo_box::ludo_box(const coord& coord, Box type) : coords(coord), box_type(type){}

bool ludo_box::areOpponentsPresent(_colour colour) const{
	auto colour_char = colourCodes.at(colour);

	for( auto const& ch : content ){
		if( !isdigit(ch) && ch != colour_char && box_type == Box::NORMAL ){
			return true;
		}
	}
	return false;
}

bool ludo_box::removeGoti(std::shared_ptr<ludo_goti>& toBeRemoved){
	char colour_char = colourCodes.at(toBeRemoved->gotiColour); //! Setting gotiColour

	for( auto& boxGoti : inBoxGotis ){
		//! Removing goti from the dataStructures is done by the next 3lines in the if block
		if( boxGoti == toBeRemoved ){
			//Logically Removing the goti now
			inBoxGotis.erase(std::find_if(inBoxGotis.begin(), inBoxGotis.end(), [&](shared_ptr<ludo_goti>& compare_goti){
				return compare_goti == toBeRemoved;
			}));

			//! Updating the string representing gotis in box from display
			auto loc = content.find(colour_char);
			if( loc + 1 == content.size() ) content.erase(content.begin() + loc);
			else if( content[loc + 1] >= 48 && content[loc + 1] <= 57 )	//ie. there are multiple gotis
			{
				if( content[loc + 1] > '2' )
					content[loc + 1] -= 1;
				else{
					content.erase(content.begin() + loc + 1);
				}
			} else content.erase(content.begin() + loc);

			if( isEmpty() && box_type == Box::STOP ){
					content = "X";
			}

			return true;
		}
	}

	return false; //Couldn't remove
}

short ludo_box::appendGoti(std::shared_ptr<ludo_goti> goti_to_add){
	_colour gotiColour = goti_to_add->get_gotiColour();
	auto colour_char = colourCodes.at(goti_to_add->gotiColour);

	if( goti_to_add->getCoords() == coord(0, 0) ) return -1;

	if( box_type == Box::STOP && isEmpty() )	content = colour_char;
	else content.push_back(colour_char);
	sanitizeContent();

	goti_to_add->curr_coords = coords;
	inBoxGotis.push_back(goti_to_add);

	if( areOpponentsPresent(gotiColour) && box_type == Box::NORMAL ) return 0;

	return 1;
}

string ludo_box::get_box_content() const{
	return content;
}

void ludo_box::sanitizeContent(){
	std::array<char, 4> gotiChar = { 'R', 'G', 'Y', 'B' };

	for( auto& i : gotiChar ){
		unsigned gotiCount = 0;
		auto loc = content.find(i);
		if( loc == static_cast<size_t>(-1) ){ continue; }
		++gotiCount; ++loc;
		while( loc < content.size() ){
			if( content[loc] == i ){
				++gotiCount;
				content.erase(content.begin() + loc);
				--loc;
			}
			++loc;
		}
		if( gotiCount > 1 ){
			if( content[content.find(i) + 1] >= 48 && content[content.find(i) + 1] <= 57 )	content[content.find(i) + 1] += gotiCount - 1;
			else content.insert(content.find(i) + 1, 1, static_cast<char>(48 + gotiCount));
		}
	}
}

bool ludo_box::isPresent(const ludo_goti& goti) const{
	for( auto& goti_ref : inBoxGotis ){
		if( *(goti_ref) == goti ){
			return true;
		}
	}
	return false;
}

bool ludo_box::isEmpty() const{
	if( box_type == Box::UNUSABLE )	return false;
	else return this->inBoxGotis.empty();
}