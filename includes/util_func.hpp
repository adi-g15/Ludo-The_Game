#pragma once

#define CUSTOM_UTIL_FUNC
#define stringify( name ) #name

// #ifndef _GLIBCXX_STRING
//     #include<string>
// #endif

// #ifndef CUSTOM_EXCEPTION
//   #include "./exceptions.hpp"
// #endif

// #ifndef CUSTOM_SIMPLETUPLE
//     #include "./simpleTuple.hpp"
// #endif

#include<simpleTuple.hpp>
#include<string>

namespace util_func
{
    intTuple getTerminalDimen();
    inline bool align_text_center(int max_length,const std::string&);
    inline std::string trimString(const std::string&);   //@returns trimmed std::string, but doesnt affect original std::string
}


#include<iostream>
#include<algorithm>

#ifdef __linux__
    #include<sys/ioctl.h>
    #include<unistd.h>
#elif __MINGW32
    #include<windows.h>
#elif __CYGWIN32
    #include<windows.h>
#endif

intTuple util_func::getTerminalDimen(){
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

inline bool util_func::align_text_center(int max_length,const std::string& str){    //@returns bool indicating, if max_len is enough or not
    if(max_length < str.size()){
        return false;
    }
    else
    {
        for (size_t i = 0; i < (max_length-str.size())/2; i++)  std::cout<<' ';
        std::cout<<str;
        for (size_t i = 0; i < (max_length-str.size()) - (max_length-str.size())/2; i++)  std::cout<<' ';
    }
    
}

inline std::string util_func::trimString(const std::string &s){
    std::string str_out(s);
    str_out.erase(std::find_if(str_out.rbegin(),str_out.rend(),[](char ch){
        return ! isspace(ch);
    }).base(), str_out.end());

    str_out.erase(std::find_if(str_out.begin(),str_out.end(),[](char ch){
        return ! isspace(ch);
    }), str_out.rend().base());
    
}