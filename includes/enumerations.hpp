#pragma once

#define CUSTOM_ENUMERATIONS 1

enum colours{	//they get values starting from 0
					/*[LEARNT] - Defining enum data type, and having variables of this user defined type
					"enumerator value must have integral or unscoped enumeration type"
					3. Though they may have integer value, BUT we can't assign an integer to an enum variable, It shows this error - invalid conversion from ‘int’ to ‘colours’ [-fpermissive]
					4. These identifiers will be available in the same scope as the declaration, eg. here, it's global scope*/
	UnknownColour,
	ColourLAAL,
	ColourHARA,
	ColourPEELA,
	ColourNEELA
};

enum player{	//corresponds to 'colours' elements
	PlayerNotDefined,
	Player1,
	Player2,
	Player3,
	Player4
};

enum direction{
	NO_TURN,
	NORTH,
    EAST,
	WEST,
	SOUTH
};