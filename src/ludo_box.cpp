#include "ludo_box.h"  

#include<iostream>
#include "exceptions.h"
#include<utility>	//ONLY for creating a temp pair, in appendGoti()
#include<algorithm>

using namespace std;

ludo_box::ludo_box() : numGotis({
		{ ColourLAAL, 0},
		{ ColourHARA, 0},
		{ ColourPEELA, 0},
		{ ColourNEELA, 0}
	})
{
	box_type = _boxNORMAL;
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
	for (auto &boxGoti : inBoxGotis)
	{
		//!Removing goti from the dataStructures is done by the next 3lines in the if block
		if( boxGoti == toBeRemoved ){
			// toBeRemoved->curr_direction = NO_TURN;	//NOTE - These will be reset by game::lockGoti()
			// toBeRemoved->curr_coords = { 0,0 };
			
			--numGotis[toBeRemoved->gotiColour];
			inBoxGotis.erase(std::find_if(inBoxGotis.begin(), inBoxGotis.end(), [&](shared_ptr<ludo_goti>& compare_goti){
				return compare_goti == toBeRemoved;
			}));
			
			//! Removing the goti from display(content)
			char colourChar = 'U'; //Undefined Colour
			if(toBeRemoved->gotiColour == ColourLAAL){
				colourChar = 'R';
			}
			else if(toBeRemoved->gotiColour == ColourHARA){
				colourChar = 'G';
			}
			else if(toBeRemoved->gotiColour == ColourPEELA){
				colourChar = 'Y';
			}
			else if(toBeRemoved->gotiColour == ColourNEELA){
				colourChar = 'B';
			}

			auto contentiter = std::find(content.begin(), content.end(), colourChar);
			if ( *(contentiter+1) >= 48 && *(contentiter+1) <= 57 )	//ie. there are multiple gotis
			{
				if(*(contentiter+1) > '2')	//ie. It will be greater than or equal to 2
					*(contentiter+1) -= 1;
				else{
					content.erase(contentiter+1);
				}
			}
			else content.erase(contentiter);

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

bool ludo_box::appendGoti(std::shared_ptr<ludo_goti> goti_to_add){
	
	if( goti_to_add->get_gotiColour() != UnknownColour && goti_to_add->getCoords() != std::make_pair(0,0) ){
		colours gotiColour = goti_to_add->get_gotiColour();

		goti_to_add->curr_coords = coords;
		inBoxGotis.push_back( goti_to_add );
		++numGotis[gotiColour];
		
		char gotiChar;
		if( gotiColour == ColourLAAL )
			gotiChar = 'R';
		else if( gotiColour == ColourHARA )
			gotiChar = 'G';
		else if( gotiColour == ColourPEELA )
			gotiChar = 'Y';
		else if( gotiColour == ColourNEELA)
			gotiChar = 'B';

		if( !isEmpty() && content != "X" ){
			auto iter = std::find(content.begin(), content.end(), gotiChar);
			if( iter != content.end() ){	//Goti of same colour present
				if(*(iter+1) >=48 && *(iter+1) <= 57){	//Handles the case where numberOfGotis alreasy follow the gotiChar( for eg. R2 )
					++(*(iter+1));
				}
				else{	//Used if-else to prevent iterator invalidation,due to erasing (iter+1) [CHANGED NOW]
					short gotiCount=1;
				
					for (auto &&ch : content)
					{
						if(ch == gotiChar)	++gotiCount;
					}
					if(gotiCount > 1)
						content.insert(iter+1,char(gotiCount + 48)); //casted to char, and added 48, since 0 is ASCII value 48
				}
			}
			else content.push_back(gotiChar);
		}else content = gotiChar;

		return true;
	}
	std::cerr<<"Can't append Goti, invalid parameters"<<std::endl;
	return false;
}

string ludo_box::get_box_content() const{
	return content;
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
