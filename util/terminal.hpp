#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <string>
#include <string_view>

#include "util/coord.hpp"

namespace util{
    util::_coord<int> getTerminalDimen();

    /*
        TIP - align* functions, and place_center() can be used for file streams too
    */
    bool align_text_center(std::string_view = "", std::ostream& stream = std::cout);                          //With empty string, it sets the cursor to the horizontal center
    bool align_text_center(unsigned int max_length, std::string_view = "", std::ostream& stream = std::cout); //NOTE - This functions does NOT add an extra '\n' at end
    bool place_center(unsigned int max_length, std::string_view = "", std::ostream& stream = std::cout);
    /*@brief Places the cursor at end of vertically centered printed string
      @params Terminal Height, and the string to print
      @returns bool indicating success or faiure to print the string
      NOTE - the string is not horizontally centered, for that, first call place_v_center() with empty string, then align_text_center()*/
    bool place_v_center(unsigned int v_length, std::string_view = "");
    bool place_v_center(std::string_view = ""); //Places cursor at vecrtically middle line

}

//DEFINITIONS

#if defined(__unix__) || defined(__unix) || defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
#include <sys/ioctl.h>
#include <unistd.h>
#define OS_LINUX
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define OS_WIN
#else
#define OS_Unknown
#endif

//returns row*column dimension
util::_coord<int> util::getTerminalDimen(){
    util::_coord<int> outTuple(0, 0);
#ifdef OS_WIN
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    outTuple.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    outTuple.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    winsize windowsSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
    outTuple.y = windowsSize.ws_row;
    outTuple.x = windowsSize.ws_col;
#endif

    return outTuple;
}

bool util::align_text_center(std::string_view str, std::ostream& stream){ //@returns bool indicating, if max_len is enough or not
    return align_text_center(getTerminalDimen().x, str, stream);
}

bool util::align_text_center(unsigned int max_length, std::string_view str, std::ostream& stream){ //@returns bool indicating, if max_len is enough or not
    if( place_center(max_length, str) == true ){
        for( size_t i = 0; i < (max_length - str.size()) - (max_length - str.size()) / 2; i++ )
            stream << ' ';
        return true;
    }
    return false;
}

bool util::place_center(unsigned int max_length, std::string_view str, std::ostream& stream){ //@returns bool indicating, if max_len is enough or not
    if( max_length < str.size() ){
        return false;
    } else{
        for( size_t i = 0; i < (max_length - str.size()) / 2; i++ )
            stream << ' ';
        stream << str;
    }
    return true;
}

bool util::place_v_center(std::string_view str){
    return place_v_center(getTerminalDimen().y, str);
}

bool util::place_v_center(unsigned int vert_length, std::string_view str){
    if( vert_length < 1 )
        return false;

    for( size_t i = 0; i < (vert_length / 2) - 1; i++ ){
        std::cout << '\n';
    }
    std::cout << str;

    return true;
}
