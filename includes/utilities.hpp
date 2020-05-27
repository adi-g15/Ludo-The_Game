#pragma once
//IMPORTANT NOTE - Import utilities at top (overridden by enumerations)

#define CUSTOM_UTIL_FUNC
#define stringify( name ) #name

#include<string>
#include<functional>
#include<iostream>
#include<vector>

#ifdef __linux__
  #include<ctime> //For nanosleep()
#else
  #include<windows.h>
#endif

#define DEBUG_START	std::cout<<"[DEBUG] START :"<<endl;
#define DEBUG_END	std::cout<<"\n: [DEBUG] PASSED"<<endl;

/*SELF-THOUGHT AND IT IS JUST A BOON! CHECKS WHETHER A PARTICULAR FUNCTION, OR CODE RAN*/
/*HELPFUL FOR GETTING TO THE SEG_FAULT SITE*/
#define DEBUG_BLOCK(code) DEBUG_START \
                          code \
                          DEBUG_END

//  FUNCTIONS START//
namespace customUtil
{
    //! NOTE - The bool values are generally for debugging purposes, and may/may not have been ignored in code
    std::pair<int,int> getTerminalDimen();
    inline bool align_text_center(const std::string& = ""); //With empty string, it sets the cursor to the horizontal center
    inline bool align_text_center(int max_length,const std::string& = ""); //NOTE - This functions does NOT add an extra '\n' at end
    inline bool place_center(int max_length,const std::string& = "");
    /*@brief Places the cursor at end of vertically centered printed string
      @params Terminal Height, and the string to print
      @returns bool indicating success or faiure to print the string
      NOTE - the string is not horizontally centered, for that, first call place_v_center() with empty string, then align_text_center()*/
    inline bool place_v_center(int v_length,const std::string& = "");
    inline bool place_v_center(const std::string& = ""); //Places cursor at vecrtically middle line
    std::string trimString(std::string);   //@returns trimmed std::string, but doesnt modify original string
    std::string stripOff(std::string, char toRemove); /*Returns a string without the passed character*/
	  std::string intTostring(int num);
    inline void pause(float sec);
    bool is_Sum_Permutation(unsigned short num, std::vector<unsigned short> Vec);  //Self Algo...
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

bool customUtil::align_text_center(int max_length,const std::string& str){    //@returns bool indicating, if max_len is enough or not
    if( place_center(max_length, str) == true){
      for (size_t i = 0; i < (max_length-str.size()) - (max_length-str.size())/2; i++)  std::cout<<' ';
      return true;
    }
    return false;
}

bool customUtil::place_center(int max_length,const std::string& str){    //@returns bool indicating, if max_len is enough or not
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

bool customUtil::place_v_center(int vert_length, const std::string& str){
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
	auto left_iter = str_out.begin(), right_iter = str_out.end();

	while( right_iter != str_out.end() ){
    left_iter = str_out.begin();  //! To get rid of 'iterator invalidation' [Know_More]
    right_iter = str_out.end();

    while( *left_iter != toRemove && left_iter != str_out.end() ) ++left_iter;
    if(left_iter == str_out.end()) break;
    right_iter = left_iter;
    while( *right_iter == toRemove && right_iter != str_out.end() ) ++right_iter;
    str_out.erase( left_iter, right_iter );
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
  #ifdef __linux__
    int sec = (int)seconds;
    int millisec = (int)(seconds*1000) - 1000*sec;
    const timespec tmp = { sec,millisec*1000000L };
    nanosleep( &tmp , NULL);
  #else
    Sleep(1000*seconds);
  #endif
}

bool is_Sum_Permutation(unsigned short num, std::vector<unsigned short> Vec){ //BUG - It seems all recursive calls have num=9, how??
  if(num == 0) return true;
  else if(Vec.empty()) return false;

  std::sort(Vec.begin(), Vec.end());
  for (auto i = Vec.begin(); i < Vec.end(); i++)
  {
    if( *i > num){
      Vec.erase(i, Vec.end());
      break;
    }
  }

  auto j = Vec.begin();
  while ( j<Vec.end() ){
    Vec.erase(j);
    if( is_Sum_Permutation(num, Vec) || is_Sum_Permutation(num-(*j), Vec)) return true;
    ++j;
  }
  return false;
}
