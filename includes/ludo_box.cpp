#include "ludo_box.hpp"  

// #ifndef CUSTOM_SIMPLETUPLE
//     #include "simpleTuple.hpp"
// #endif
// #ifndef CUSTOM_EXCEPTIONS
//     #include "exceptions.hpp"
// #endif

// #ifndef _GLIBCXX_FUNCTIONAL
//     #include<functional>
// #endif
// #ifndef _GLIBCXX_IOSTREAM
//     #include<iostream>
// #endif

#include<iostream>
#include<exceptions.hpp>

using namespace std;

ludo_box::ludo_box(){
    content = " ";
}

reference_wrapper<ludo_goti> ludo_box::getGoti(colours gotiColour){
	for (auto &&i : inBoxGotis)
	{
		if(i.get().get_gotiColour() == gotiColour){
			return i;
		}
	}		

	throw GotiNotAvailableException(gotiColour);
}

bool ludo_box::removeGoti(colours gotiColour){
	vector<reference_wrapper<ludo_goti>>::const_iterator i = inBoxGotis.begin();
	for ( ; i < inBoxGotis.end(); ++i)
	{
		if( i->get().get_gotiColour() == gotiColour){
			inBoxGotis.erase(i);
			return true;
		}
	}

	return false;
}

bool ludo_box::appendGoti(ludo_goti& in_goti){
	if( in_goti.get_gotiColour() != UnknownColour && in_goti.getCoords() != intTuple(0,0) ){
		inBoxGotis.push_back(ref(in_goti));
		return true;
	}
	std::cerr<<"Can't append Goti, invalid parameters"<<std::endl;
	return false;
}

string ludo_box::get_box_content() const{
	return content;
}

//      LEARNT  //
/*[LEARNT] - The error "pointer to incomplete class type is not allowed", generally related to header files, when "I forward declared the class in the header, and failed to include the full header for the class"
			AND, "An 'incomplete class' is one that is declared but not defined"
			ALSO, If your class is defined as a typedef: "typedef struct struct{};", and then try to refer to it as 'struct myclass' anywhere else, you'll get many such errors, to solve it, remove 'class/struct' from variable declarations, ie. 'mystruct *var = value;' instead of 'struct mystruct *var=value'*/
