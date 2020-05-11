#define CUSTOM_LUDO_BOX 1

#ifndef CUSTOM_ENUMERATIONS
    #include "enumerations.hpp"
#endif
#ifndef CUSTOM_LUDO_GOTI
    #include "ludo_goti.hpp"
#endif

#ifndef _GLIBCXX_STRING
    #include<vector>
#endif
#ifndef _GLIBCXX_STRING
    #include<string>
#endif
#ifndef _GLIBCXX_FUNCTIONAL
    #include<functional>
#endif

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
	bool appendGoti(ludo_goti&);
	std::string get_box_content();

	ludo_box();
};

//      FUTURE TASKS    //
/*FUTURE - Do have null check
    ALTERNATE NULL CHECK -> For checking if passed ludo_goti is valid, check it's colour and/or coords to be valid
*/
