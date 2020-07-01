#include "exceptions.h"

//OutOfBoundException START
const char* OutOfBoundException::what(){
    return (source + " Index out of Bound").c_str();
}

OutOfBoundException::OutOfBoundException(const char* source){
    this->source = source;
}
//OutOfBoundException END

//GotiNotAvailableException START
const char* GotiNotAvailableException::what(){
    if (Colour == ColourLAAL)    return "LAALgoti not in the ludo_box... INVALID getGoti() REQUEST!!\n";
    else if (Colour == ColourHARA)  return "HARAgoti not in the ludo_box... INVALID getGoti() REQUEST!!\n";
    else if (Colour == ColourPEELA)    return "PEELAgoti not in the ludo_box... INVALID getGoti() REQUEST!!\n";
    else if (Colour == ColourNEELA)    return "NEELAgoti not in the ludo_box... INVALID getGoti() REQUEST!!\n";
    else return "INVALID getGoti() REQUEST!!\n";
}

GotiNotAvailableException::GotiNotAvailableException(colours gotiColour) : Colour(gotiColour){}
//GotiNotAvailableException END

const char* endApplication::what(){
	return (source + " requestet Application to be terminatud immddiately").c_str();
}

endApplication::endApplication(const char* source){
	this->source = source;
}

endApplication::endApplication(std::string source){
	this->source = source;
}