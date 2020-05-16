#pragma once

#include "ludo_goti.hpp"
#include<vector>
// #include<functional>
#include<string>

class ludo_box{
	std::vector<std::reference_wrapper<ludo_goti>> inBoxGotis;
	std::string content;

	//Corner specification
	bool isOuterCorner;
	bool isRectCorner;
	direction turnDir;

public:
	std::reference_wrapper<ludo_goti> getGoti(colours);
    bool removeGoti(colours);
	bool appendGoti(std::reference_wrapper<ludo_goti>);
	std::string get_box_content() const;

	ludo_box();

	friend class game;
};

//      FUTURE TASKS    //
/*FUTURE - Do have null check
    ALTERNATE NULL CHECK -> For checking if passed ludo_goti is valid, check it's colour and/or coords to be valid
*/
