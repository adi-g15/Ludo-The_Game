#include<exception>

#ifndef _GLIBCXX_DEBUG_STRING
  #include<string>
#endif

#include "./enumerations.hpp"

#define CUSTOM_EXCEPTION true

class OutOfBoundException : std::exception{
  std::string source = "Array";
public:
  const char* what(){
    return (source + " Index out of Bound").c_str();
  }
  OutOfBoundException(const char* source){
    this->source = source;
  }
  OutOfBoundException() = delete;
};

class GotiNotAvailableException : std::exception{
  colours Colour;
  public:
   const char* what(){
      if(Colour == UnknownColour){
        return "INVALID getGoti() REQUEST!!\n";
      }else if (Colour == ColourLAAL)
      {
        return "LAALgoti not in the box... INVALID getGoti() REQUEST!!\n";
      }else if (Colour == ColourHARA)
      {
        return "HARAgoti not in the box... INVALID getGoti() REQUEST!!\n";
      }else if (Colour == ColourPEELA)
      {
        return "PEELAgoti not in the box... INVALID getGoti() REQUEST!!\n";
      }else if (Colour == ColourNEELA)
      {
        return "NEELAgoti not in the box... INVALID getGoti() REQUEST!!\n";
      }
  }
  GotiNotAvailableException(colours gotiColour) : Colour(gotiColour){}
  GotiNotAvailableException() = delete;
};