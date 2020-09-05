#pragma once
//IMPORTANT NOTE - Import utilities at top (overridden by enumerations)

#define CUSTOM_UTIL_FUNC
#define stringify( name ) #name

#include <functional>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <chrono>

#ifdef __linux__
  #include<ctime> //For nanosleep()
#else
  #include<windows.h>
#endif

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
#define DEBUG_START	std::cout<<"[DEBUG] START :"<<endl;
#define DEBUG_END	std::cout<<"\n: [DEBUG] PASSED"<<endl;
/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
/*HELPFUL FOR GETTING TO THE SEG_FAULT SITE*/
#define DEBUG_BLOCK(code) DEBUG_START \
                          code \
                          DEBUG_END
/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/


/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
/* Colour any output, due to the code, just use COLOUR_THIS*/
#define COLOUR_THIS(colour, code)     customUtil::_colouredOutput(colour); \
                          					code \
									 	 customUtil::_resetColouredOutput();
/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

// template<typename T1, typename T2>	//LINKER_ERROR - When this fuinction is uncommented, and When utilities.hpp included in ludo_box.cpp, and game.cpp, then 'multiple defintion errors show up for the mebmbers of the customUtil namespace (Wait...WHAT? WHY?... I dont know now)'
// static inline std::ostream& operator<<(std::ostream& out, const std::pair<T1,T2>& p){
// 	return out<<'('<<p.first<<", "<<p.second<<')';
// }

// inline std::ostream& operator<<(std::ostream& out, const std::pair<int,int>& p){
// 	return out<<'('<<p.first<<", "<<p.second<<')';
// }


//  FUNCTIONS START//
namespace customUtil
{
    enum colours{
      RED,
      GREEN,
      YELLOW,
      BLUE,
      MAGENTA,
      CYAN,

      RESET
    };

    struct Timer{
	    private:
	    	std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> start, end;
	    public:
	    	Timer();
	    	~Timer();
    };

    //! NOTE - The bool values are generally for debugging purposes, and may/may not have been ignored in code
    std::pair<int,int> getTerminalDimen();

    static std::map<colours, std::string> terminalColourMapping({
      { RED, "[0;31m" },
      { GREEN, "[0;32m" },
      { YELLOW, "[0;33m" },
      { BLUE, "[0;34m" },
      { MAGENTA, "[0;35m" },
      { CYAN, "[0;36m" },
      { RESET, "[0m" },
    });

    //! NOTE - The bool values are generally for debugging purposes, and may/may not have been ignored in code
    std::pair<int,int> getTerminalDimen();
    inline bool align_text_center(const std::string& = ""); //With empty string, it sets the cursor to the horizontal center
    inline bool align_text_center(unsigned int max_length,const std::string& = ""); //NOTE - This functions does NOT add an extra '\n' at end
    inline bool place_center(unsigned int max_length,const std::string& = "");
    /*@brief Places the cursor at end of vertically centered printed string
      @params Terminal Height, and the string to print
      @returns bool indicating success or faiure to print the string
      NOTE - the string is not horizontally centered, for that, first call place_v_center() with empty string, then align_text_center()*/
    inline bool place_v_center(unsigned int v_length,const std::string& = "");
    inline bool place_v_center(const std::string& = ""); //Places cursor at vecrtically middle line
    std::string trimString(std::string);   //@returns trimmed std::string, but doesnt modify original string
    std::string stripOff(std::string, char toRemove); /*Returns a string without the passed character*/
	  std::string intTostring(int num);
    inline void pause(float sec);
    /*@brief Returns true if `num` can be written as a sum of some arbitrary elements of the vector v; Else returns false*/
    bool isSum(unsigned int num, const std::vector<unsigned short> &v);
    std::vector<unsigned short> isSumOfElements(unsigned int num, const std::vector<unsigned short> &v);
    void printcoloured(char, colours colourName, bool isBold = false);
    void printcoloured(std::string, colours colourName, bool isBold = false);
    void _colouredOutput(colours colourName, bool isBold = false);
    void _resetColouredOutput();
    inline bool icompare(const std::string& s1, const std::string& s2) noexcept;

}
//  FUNCTIONS END//


//Defintions Start
//This solved the 'undefined references' to the customUtil::functions in files
#include<algorithm>
#include "exceptions.h"

#ifdef __linux__
    #include<sys/ioctl.h>
    #include<unistd.h>
#elif __MINGW32
    #include<windows.h>
#elif __CYGWIN32
    #include<windows.h>
#endif

std::pair<int,int> customUtil::getTerminalDimen(){
  std::pair<int,int> outTuple(0,0);
  #ifdef __linux__
    winsize windowsSize;
    ioctl (STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
        outTuple = { windowsSize.ws_col , windowsSize.ws_row };

  #else //Windows API
    {
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbi );
        outTuple = { csbi.srWindow.Bottom - csbi.srWindow.Top + 1, csbi.srWindow.Right - csbi.srWindow.Left + 1 };
    }

  #endif

  return outTuple;
}

bool customUtil::align_text_center(const std::string& str){    //@returns bool indicating, if max_len is enough or not

  return align_text_center(getTerminalDimen().first, str);
}

bool customUtil::align_text_center(unsigned int max_length,const std::string& str){    //@returns bool indicating, if max_len is enough or not
    if( place_center(max_length, str) == true){
      for (size_t i = 0; i < (max_length-str.size()) - (max_length-str.size())/2; i++)  std::cout<<' ';
      return true;
    }
    return false;
}

bool customUtil::place_center(unsigned int max_length,const std::string& str){    //@returns bool indicating, if max_len is enough or not
    if(max_length < str.size()){
        return false;
    }
    else
    {
        for (size_t i = 0; i < (max_length-str.size())/2; i++)  std::cout<<' ';
        std::cout<<str;
    }
    return true;
}

bool customUtil::place_v_center(const std::string& str){
  return place_v_center(getTerminalDimen().second, str);
}

bool customUtil::place_v_center(unsigned int vert_length, const std::string& str){
  if(vert_length < 1)
    return false;
  
  for (size_t i = 0; i < (vert_length/2) - 1; i++)
  {
    std::cout<<'\n';
  }
  std::cout<<str;

  return true;
}

std::string customUtil::trimString(std::string str_out){
    auto lambda = [](char ch){
        return !isspace(ch) || ch=='\n';
    };

    str_out.erase(std::find_if( str_out.begin(),str_out.end(),lambda ), str_out.rend().base());
    str_out.erase(std::find_if( str_out.rbegin(),str_out.rend(),lambda ).base(), str_out.end());
    
    return str_out;
}

std::string stripOff(std::string str_out, char toRemove){
	auto left_iter = str_out.begin(), right_iter = str_out.begin();

	while( right_iter != str_out.end() ){
		while( *left_iter != toRemove ){
			++left_iter;
			if(left_iter == str_out.end()) break;
		}
		if(left_iter == str_out.end()) break;
		else{
			right_iter = left_iter;
			while( *right_iter != toRemove ){
				++right_iter;
				if(right_iter == str_out.end()) break;
			}
		}

		str_out.erase( left_iter, right_iter );

		left_iter = str_out.begin();  //! To get rid of 'iterator invalidation' [Know_More]

	}

  return str_out;
}

std::string customUtil::intTostring(int num){
  std::string outString;
  while(num > 0){
    outString.insert( outString.begin() , char(num%10 + 48) );
    num/=10;
  }

  return outString;
}

/*Issue pointed by codacy - 
This C routine is considered obsolete (as opposed to the shell command by the same name). The interaction of this function with SIGALRM and other timer functions such as sleep(), alarm(), setitimer(), and nanosleep() is unspecified (CWE-676). Use nanosleep(2) or setitimer(2) instead. */

//! Changed from int to float, since, 0.7 would have been implicitly converted to 0
void customUtil::pause(float seconds){  //Supports Win, Linux, Mac

  return; //DEBUG PURPOSES, REMOVE THIS LATER

  #ifdef __linux__
    int sec = (int)seconds;
    // int millisec = (int)(seconds*100) - 100*sec; //DEBUG, mmaking the pause 10 times smaller
    int millisec = (int)(seconds*1000) - 1000*sec;
    const timespec tmp = { sec,millisec*1000000L };
    nanosleep( &tmp , NULL);
  #else
	//  deepcode ignore CppDeadCode: It is reachable on any other OS, eg. windows
    Sleep(1000*seconds);
  #endif
}

void customUtil::_colouredOutput(colours colourName, bool isBold){
  if( !isBold ){
    printf("%s",terminalColourMapping[colourName].c_str());
  }
  else{	//ie. Text is bold
    std::string colourStr;

    if( colourName != YELLOW ){
      	colourStr = terminalColourMapping[colourName];
      	colourStr[1] = '1';
      		//! '\033' is the escape sequence
      	printf("\033%s\n", colourStr.c_str());
    }
    else{
    	colourStr = "[01" + terminalColourMapping[colourName].substr(2);
      	printf("\033%s\n", colourStr.c_str());
    }
  }
}

void customUtil::printcoloured(char character, colours colourName, bool isBold){
  _colouredOutput(colourName, isBold);
  std::cout<<character;
  _resetColouredOutput();
}

void customUtil::printcoloured(std::string str, colours colourName, bool isBold){
	_colouredOutput(colourName, isBold);
	printf("%s",str.c_str());
	_resetColouredOutput();
}

void customUtil::_resetColouredOutput(){
  printf("\033%s",terminalColourMapping[RESET].c_str());
}

bool customUtil::isSum(unsigned int num, const std::vector<unsigned short> &v){
    auto total = 1<<v.size();

    for (int i = 0; i < total; ++i)
    {
        unsigned sum = 0;
        for(size_t j=0; j<v.size(); ++j){
            if( i & 1<<j ){ //Cheching if the jth bit is set
                sum += v[j];
            }
        }
        if( num == sum ) return true;
    }
    return false;
}

std::vector<unsigned short> customUtil::isSumOfElements(unsigned int num, const std::vector<unsigned short> &v){
    unsigned int sum = 0; auto total = 1<<v.size();
    std::vector<unsigned short> elements;

    for (int i = 0; i < total; ++i)
    {
        for(size_t j=0; j<v.size(); ++j){
            if( i & 1<<j ){ //Cheching if the jth bit is set
                elements.push_back(v[j]);
                sum += v[j];
            }
        }
        if( num == sum ) return elements;
        sum=0; elements.clear();
    }
    return {};
}

inline bool customUtil::icompare(const std::string& s1, const std::string& s2) noexcept{
  for( size_t i=0; i<s1.size(); ++i ){
      if( std::tolower(s1[i]) != tolower(s2[i]) ) return false;
  }
  return true;
}
