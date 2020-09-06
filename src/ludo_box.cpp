#include "exceptions.hpp"
#include "keywords.hpp"
#include "ludo_box.hpp"

#include <iostream>
#include <utility>	//ONLY for creating a temp pair, in appendGoti()
#include <algorithm>
#include <array>

using namespace std;

template<typename T1, typename T2>	//Utility function
static inline std::ostream& operator<<(std::ostream& out, const std::pair<T1,T2>& p){
	return out<<'('<<p.first<<", "<<p.second<<')';
}

ludo_box::ludo_box(const _coord& coord, BOX_TYPE type) : coords(coord), box_type(type){}

bool ludo_box::areOpponentsPresent(_colour colour) const{
	auto colourChar = colourCodes.at(colour);

	for ( auto const& ch : content )
	{
		if( !isdigit(ch) && ch != colourChar && box_type== _boxNORMAL ){
			return true;
		}
	}
	return false;
}

bool ludo_box::removeGoti(std::shared_ptr<ludo_goti>& toBeRemoved){
	auto colourChar = colourCodes.at(toBeRemoved->gotiColour); //! Setting gotiColour

	for (auto &boxGoti : inBoxGotis)
	{
		//!Removing goti from the dataStructures is done by the next 3lines in the if block
		if( boxGoti == toBeRemoved ){

				//Logically Removing the goti now
			inBoxGotis.erase(std::find_if(inBoxGotis.begin(), inBoxGotis.end(), [&](shared_ptr<ludo_goti>& compare_goti){
				return compare_goti == toBeRemoved;
			}));

			//! Removing the goti from display(content)

			auto loc = content.find(colourChar);

		//Expects sanitized string
			if( loc+1 == content.size() ) content.erase(content.begin() + loc);
			else if ( content[loc+1] >= 48 && content[loc+1] <= 57 )	//ie. there are multiple gotis
			{
				if(content[loc+1] > '2')
					content[loc+1] -= 1;
				else{
					content.erase(content.begin() + loc+1);
				}
			}
			else content.erase(content.begin() + loc);

			if(isEmpty()){
				if(box_type == _boxSTOP){
					content = "X";
				}
			}

			return true;
		}
	}

	return false; //Couldn't remove
}

short ludo_box::appendGoti(std::shared_ptr<ludo_goti> goti_to_add){
	_colour gotiColour = goti_to_add->get_gotiColour();
	auto colourChar = colourCodes.at(goti_to_add->gotiColour);

	if(goti_to_add->getCoords() == make_pair(0,0)) return -1;

	if( box_type == _boxSTOP && isEmpty() )	content = colourChar;
	else content.push_back(colourChar);
	sanitizeContent();

	goti_to_add->curr_coords = coords;
	inBoxGotis.push_back( goti_to_add );

	if( areOpponentsPresent(gotiColour) && box_type == _boxNORMAL ) return 0;

	return 1;
}

string ludo_box::get_box_content() const{
	return content;
}

void ludo_box::sanitizeContent(){
    std::array<char, 4> gotiChar = {'R', 'G', 'Y', 'B'};

    for( auto &i : gotiChar ){
	    unsigned gotiCount = 0;
        auto loc = content.find(i);
        if( loc == static_cast<size_t>(-1) ){ continue; }
        ++gotiCount; ++loc;
        while( loc<content.size() ){
            if( content[loc] == i ){
                ++gotiCount;
                content.erase(content.begin() + loc);
                --loc;
            }
            ++loc;
        }
        if( gotiCount > 1 ){
        	if( content[content.find(i)+1] >=48 && content[content.find(i)+1] <=57 )	content[content.find(i)+1] += gotiCount-1;
            else content.insert(content.find(i)+1, 1, static_cast<char>(48+gotiCount) );
        }
    }
}

bool ludo_box::isPresent(const ludo_goti& goti) const{
	for (auto &goti_ref : inBoxGotis)
	{
		if( *(goti_ref) == goti ){
			return true;
		}
	}
	return false;
}

bool ludo_box::isEmpty() const{
	if( box_type == _boxUNUSABLE )	return false;
	else return this->inBoxGotis.empty();
}