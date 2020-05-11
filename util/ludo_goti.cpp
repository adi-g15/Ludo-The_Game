#include "ludo_goti.hpp"

#ifndef CUSTOM_ENUMERATIONS
    #include "enumerations.hpp"
#endif
#ifndef TMP
    #include "global.hpp"
#endif
#ifndef CUSTOM_SIMPLETUPLE
    #include "simpleTuple.hpp"
#endif
#ifndef CUSTOM_EXCEPTIONS
    #include "exceptions.hpp"
#endif
#ifndef _GLIBCXX_FUNCTIONAL
    #include<functional>
#endif
#ifndef _GLIBCXX_IOSTREAM
    #include<iostream>
#endif




using namespace std;

const intTuple ludo_goti::getCoords() const{
    if(curr_coords.getKey() == 0 && curr_coords.getVal()==0){
        cerr<<"Coordinates of Goti are corrupt (0,0)\n"<<endl;
    }
    return curr_coords;
}
direction ludo_goti::get_curr_direction() const{
    return curr_direction;
}

ludo_goti::ludo_goti() : curr_coords(0,0) {
    gotiColour = UnknownColour;
}

colours ludo_goti::get_gotiColour(void) const{
    return gotiColour;
}

laalGoti::laalGoti(){
    this->gotiColour = ColourLAAL;
    this->curr_coords = get_initCoords(ColourNEELA);
}

hariGoti::hariGoti(){
    this->gotiColour = ColourHARA;
    this->curr_coords = get_initCoords(ColourNEELA);
}

peeliGoti::peeliGoti(){
    this->gotiColour = ColourPEELA;
    this->curr_coords = get_initCoords(ColourNEELA);
}

neeliGoti::neeliGoti(){
    this->gotiColour = ColourNEELA;
    this->curr_coords = get_initCoords(ColourNEELA);
}

//  LEARNT  //
//[LEARNT_KNOW_MORE] - 'abstract' isn't a keyword in C++
//[LEARNT] - 	//public Getters maybe used to ensure that private mamber can't be modified from outside
//ERROR_SOLVED - Showing error : expected identifier before numeric constant intTuple coords(0,0);
                    /*LEARNT - We can't call the constructor of a datatype right at declaration, "in a class outside any method"
                            But, we can braces{} instead of () to initialize right at declaration, BUT... by passing a constructed object
                                vector<int> v(5);	//Wrong
                                vector<int> v{vector<int>(5)}; //Right
                                
                            So, first declare then, call their constructors in constructor(See this class's constructor)*/
//LEARNT - We can't define, 'implicitly-"declared" function/constructor', we have to explicitly declare the constructor in the header
