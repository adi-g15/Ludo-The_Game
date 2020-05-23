#pragma once
//IMPORTANT NOTE - Import utilities at top (overridden by enumerations)

#define CUSTOM_UTIL_FUNC
#define stringify( name ) #name

#include<string>
#include<functional>
#include<iostream>
#include<vector>

#ifdef __linux__
  #include<unistd.h>
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
    std::string trimString(const std::string&);   //@returns trimmed std::string, but doesnt modify original string
	  std::string intTostring(int num);
    inline void pause(int sec);
    bool is_Sum_Permutation(unsigned short num, std::vector<unsigned short> Vec);  //Self Algo...
}
//  FUNCTIONS END//


//////// UTILITIES.CPP BELOW    !!!!
//This solved the 'undefined references' to the customUtil::functions in files
#include<algorithm>
#include "exceptions.hpp"

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
    //   outTuple = { windowsSize.ws_row , windowsSize.ws_col };    //NOTE - It was returning y*x (considering downward height of terminal to be y)
        outTuple = { windowsSize.ws_col , windowsSize.ws_row };

  #else //Windows API
    {
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbi );
        outTuple = { csbi.srWindow.Bottom - csbi.srWindow.Top + 1, csbi.srWindow.Right - csbi.srWindow.Left + 1 };
    }

  #endif

    // std::cout<<"std::pair<int,int> - "<<outTuple.first<<'x'<<outTuple.second<<std::endl;    //[FOR DEBUG PURPOSES]

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

std::string customUtil::trimString(const std::string &s){
    std::string str_out(s);
    auto lambda = [](char ch){
        return !isspace(ch) || ch=='\n';
    };

    str_out.erase(std::find_if( str_out.begin(),str_out.end(),lambda ), str_out.rend().base());
    str_out.erase(std::find_if( str_out.rbegin(),str_out.rend(),lambda ).base(), str_out.end());
    
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

void customUtil::pause(int seconds){  //Supports Win, Linux, Mac
  #ifdef __linux__
    usleep(1000000*seconds);
  #else
    Sleep(1000*seconds);
  #endif
}

bool is_Sum_Permutation(unsigned short num, std::vector<unsigned short> Vec){ //BUG - It seems all recursive calls have num=9, how??
	if(num == 0) return true;
	else if(num<0 || Vec.empty()) return false;

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
//LEARNT+QUESTIONS BELOW//
//ERROR - [in getTerminalDimen()] Replace std::pair<int,int> with simpleTuple<int,int> gives error : conversion from ‘simpleTuple<int, int>’ to non-scalar type ‘std::pair<int,int> {aka homoTuple<int>}’ requested
/*LEARNT - ASCII value of integers 0-9 are actually 48-57*/
    /*Learnt - In case of only 1 argument passed to std::string::erase()...
                1. If we provide an index number, ALL including and after it are erased
                2. If we provide an iterator,    ONLY that position is removed*/

    //LEARNT - algorithm -> find_if - Keep finding, until the predicate is true
    //LEARNT - Using .base() on a reverse_iterator returns us the normal_iterator, BUT CAUTION- This isn't advisable, and might cause SEGFAULT, (for eg. accessing .rbegin().base())

/*LEARNT - stringify() can turn any name into text, BUT remember, it ignores trailing and front spaces, for eg. in stringify( Hello World ), first and last space ignored*(Verified)/
*/

/*LEARNT - The component type of STL containers like vectors must be 'asignable'.
			References are NOT assignable, ie. we can only initialise them once, and,
			you can't make them reference something else later (you can't assign it to reference any other vairable, and even after equating 2 references, only the values of variable they reference is changed, not the addresses they both refer to)
			Due to same reason, other non-assignable types are also not allowed as components of containers, for eg. vector<const int> not allowed
*/

//LEARNT+QUESTIONS BELOW [SIMPLETUPLE]//
 /*[QUESTION_Answered] - This gives error without the 'this->'? Why?
                            Answer - It is because unqualified lookup doesn't work here because the base is a template.
                                      So, we need to use this-> to access inherited members in childs of template classes*/
/*[LEARNT] - Direct access through objects of Base Class, to protected members of Base Class,
                                                          is only in methods of the Base class, not even in Child
                                                          For. eg, you take an object of parent type (not 'this' object), then can't access in Child*/
  //DOWNSIDE_C++, QUESTION - We can't have virtual templated member function
  //LEARNT - In a templated function, if return types in if and blocks is different, then It wont be able to know the type
/*[LEARNT] - In C++, protected members can't be accessed directly
                         In Java, protected members 'can' be accessed directly, but in same package*/
/*NOTE, QUESTION - Did this to suppress this - In instantiation of ‘homoTuple<T>::homoTuple(T, T) [with T = int]’:
./util/terminalstd::pair<int,int>.hpp:17:24:   required from here
./util/./simpleTuple.hpp:70:30: error: no matching function for call to ‘simpleTuple<int, int>::simpleTuple()’
     homoTuple(T key, T value){ */