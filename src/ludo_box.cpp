#include "ludo_box.h"

#include "exceptions.h"
#include<iostream>
#include<utility>	//ONLY for creating a temp pair, in appendGoti()
#include<algorithm>

using namespace std;

template<typename T1, typename T2>	//Utility function
static inline std::ostream& operator<<(std::ostream& out, const std::pair<T1,T2>& p){
	return out<<'('<<p.first<<", "<<p.second<<')';
}

ludo_box::ludo_box() : numGotis({
		{ ColourLAAL, 0},
		{ ColourHARA, 0},
		{ ColourPEELA, 0},
		{ ColourNEELA, 0}
	})
{
	box_type = _boxNORMAL;
}

bool ludo_box::areOpponentsPresent(colours colour){
	auto colourChar = getColourChar(colour);

	for ( auto const& ch : content )
	{
		if( !isdigit(ch) && ch != colourChar && box_type== _boxNORMAL ){
			return true;

			cout<<"\n\n\n\n\n\n[DEBUG] At "<<coords<<" : "<<colourChar<<" has opponent "<<ch<<" AND content = "<<content<<"\n\n\n\n\n"<<endl;	//DEBUG

		}
	}
	return false;
}

char ludo_box::getColourChar(colours colour){
	return (colour == ColourLAAL) ? 'R' : ( (colour == ColourHARA) ? 'G' : ( (colour == ColourPEELA) ? 'Y' : ( (colour == ColourNEELA) ? 'B' : 'U') ) );
}

std::weak_ptr<ludo_goti> ludo_box::getGoti(colours gotiColour){
	for (auto &&i : inBoxGotis)
	{
		if(i->get_gotiColour() == gotiColour){
			return i;
		}
	}		

	throw GotiNotAvailableException(gotiColour);
}

bool ludo_box::removeGoti(std::shared_ptr<ludo_goti>& toBeRemoved){
	auto colourChar = getColourChar(toBeRemoved->gotiColour); //! Setting gotiColour
	cout<<"\n[DEBUG] REMOVE START - toBeRemoved colour is "<<toBeRemoved->gotiColour<<endl;

	for (auto &boxGoti : inBoxGotis)
	{
		//!Removing goti from the dataStructures is done by the next 3lines in the if block

		cout<<"[DEBUG] inBoxGotis contained : ";
		for(auto const&box : inBoxGotis) cout<<box->gotiColour<<' ';
		cout<<endl;
		if( boxGoti == toBeRemoved ){
					
				//Logically Removing the goti now
			--numGotis[toBeRemoved->gotiColour];
			inBoxGotis.erase(std::find_if(inBoxGotis.begin(), inBoxGotis.end(), [&](shared_ptr<ludo_goti>& compare_goti){
				return compare_goti == toBeRemoved;
			}));

			//! Removing the goti from display(content)
	cout<<"\n[DEBUG] REMOVE START 2"<<endl;
		cout<<"[DEBUG] inBoxGotis contained : ";
		for(auto const&box : inBoxGotis) cout<<box->gotiColour<<' ';
		cout<<endl;

			auto loc = content.find(colourChar);
			if( loc == static_cast<size_t>(-1) ){
				cout<<"INVALID DEBUG -> To remove="<<toBeRemoved->get_gotiColour()<<" and found_goti="<<boxGoti->gotiColour<<" to remove from "<<toBeRemoved->getCoords()<<" while finding "<<colourChar<<" in content = "<<content<<endl;
				cin.get();
				// return false;
			}

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

	cout<<"\n[DEBUG] REMOVE END 1,2"<<endl;
			return true;
		}
	}

	return false; //Couldn't remove
}

short ludo_box::appendGoti(std::shared_ptr<ludo_goti> goti_to_add, bool onlyRequiresWillItAttack){
	colours gotiColour = goti_to_add->get_gotiColour();
	auto colourChar = getColourChar(goti_to_add->gotiColour);

	if( onlyRequiresWillItAttack == true ){	//! Used by thinker, to know whether attack happens or not, without actually altering the board
		 if( areOpponentsPresent(gotiColour) && box_type == _boxNORMAL) return 0;
		 else return 1;
	}

	if(goti_to_add->getCoords() == make_pair(0,0)) return -1;
	
	cout<<"\n[DEBUG] Append START of "<<goti_to_add->get_gotiColour()<<" to "<<coords<<". And box isempty "<<boolalpha<<isEmpty()<<endl;
	if( box_type == _boxSTOP && isEmpty() )	content = colourChar;
	else content.push_back(colourChar);
	sanitizeContent();

	goti_to_add->curr_coords = coords;
	inBoxGotis.push_back( goti_to_add );
	++numGotis[gotiColour];

	if( areOpponentsPresent(gotiColour) && box_type == _boxNORMAL ) return 0;

	cout<<"\n[DEBUG] Append END"<<endl;
	return 1;
}

string ludo_box::get_box_content() const{
	return content;
}

void ludo_box::sanitizeContent(void){
    unsigned short gotiCount = 0;
    std::array<char, 4> gotiChar = {'R', 'G', 'Y', 'B'};

    for( auto &i : gotiChar ){
        gotiCount = 0;
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

bool ludo_box::isEmpty(){
	if( box_type == _boxUNUSABLE )	return false;
	for (auto &&i : numGotis)
	{
		if( i.second != 0 )
			return false;
	}
	return true;
}


//      LEARNT  //
/*[LEARNT] - The error "pointer to incomplete class type is not allowed", generally related to header files, when "I forward declared the class in the header, and failed to include the full header for the class"
			AND, "An 'incomplete class' is one that is declared but not defined"
			ALSO, If your class is defined as a typedef: "typedef struct struct{};", and then try to refer to it as 'struct myclass' anywhere else, you'll get many such errors, to solve it, remove 'class/struct' from variable declarations, ie. 'mystruct *var = value;' instead of 'struct mystruct *var=value'*/
