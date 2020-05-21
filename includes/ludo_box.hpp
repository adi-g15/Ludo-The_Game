#pragma once

#include "ludo_goti.hpp"
#include<vector>
// #include<functional>
#include<memory>
#include<map>
#include<string>

enum BOX_TYPE{
	_boxNORMAL,
	_boxLOCK,
	_boxSTOP,
	_boxHOMEPATH,
	_boxHOMEAREA,
	_boxUNUSABLE
};

class ludo_box{
	//Used shared_ptr since when lockedPositions are initialised, these will be the one primarily responsible for holding the owner
	std::vector<std::shared_ptr<ludo_goti>> inBoxGotis;	//Can make it a map too, but might be inefficient for such less data
	std::string content;

	//Corner specification
	bool isOuterCorner;
	bool isRectCorner;
	direction turnDir;
	std::pair<int,int> coords;

	//NOTE - Before calling this function, explicitly add the shared_ptr to lockedPositions
    bool removeGoti(std::shared_ptr<ludo_goti>&);

public:
	BOX_TYPE box_type;
	std::map<colours, int> numGotis;
	std::weak_ptr<ludo_goti> getGoti(colours);
	/*QUESTION - Is it OK to pass 'references' of weak_ptr, and not by value ?*/
	
	//STYLE_NOTE - I have used pass by value, in append, in case of weak_ptr and shared_ptr, where the scope of the incoming is important, so that we don't get reference to local var
	//But used pass by reference in case of move,... BUT 'WHY' DID I DO SO !??
	
	bool appendGoti(std::shared_ptr<ludo_goti>);
	std::string get_box_content() const;
	bool isPresent(const ludo_goti& goti) const;
	bool isEmpty(void);

	ludo_box();

	friend class game;
	friend class _BoardPrinter;
};

//      FUTURE TASKS    //
/*FUTURE - Do have null check
    ALTERNATE NULL CHECK -> For checking if passed ludo_goti is valid, check it's colour and/or coords to be valid
*/
