#include "utilities.hpp"

#include<iostream>
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

intTuple utility::getTerminalDimen(){
  intTuple retTuple(0,0);
  #ifdef __linux__
    winsize windowsSize;
    ioctl (STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
      retTuple[0] = windowsSize.ws_row;
      retTuple[1] = windowsSize.ws_col;

  #else //Windows API
    {
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbi );
        retTuple[0] = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        retTuple[1] = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

  #endif

  return retTuple;
}

bool utility::align_text_center(int max_length,const std::string& str){    //@returns bool indicating, if max_len is enough or not
    if(max_length < str.size()){
        return false;
    }
    else
    {
        for (size_t i = 0; i < (max_length-str.size())/2; i++)  std::cout<<' ';
        std::cout<<str;
        for (size_t i = 0; i < (max_length-str.size()) - (max_length-str.size())/2; i++)  std::cout<<' ';
    }
    return true;
}

std::string utility::trimString(const std::string &s){
    std::string str_out(s);
    auto lambda = [](char ch){
        return ! isspace(ch);
    };

    str_out.erase(std::find_if( str_out.rbegin(),str_out.rend(),lambda ).base(), str_out.end());
    str_out.erase(std::find_if( str_out.begin(),str_out.end(),lambda ), str_out.rend().base());
    
    return str_out;
}

//LEARNT+QUESTIONS BELOW//
//ERROR - [in getTerminalDimen()] Replace intTuple with simpleTuple<int,int> gives error : conversion from ‘simpleTuple<int, int>’ to non-scalar type ‘intTuple {aka homoTuple<int>}’ requested
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
./util/terminalDimensions.hpp:17:24:   required from here
./util/./simpleTuple.hpp:70:30: error: no matching function for call to ‘simpleTuple<int, int>::simpleTuple()’
     homoTuple(T key, T value){ */