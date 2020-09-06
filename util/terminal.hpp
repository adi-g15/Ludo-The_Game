#pragma once

#include <iostream>
#include <utility>
#include <string>

//You may find `coloured output` functions in stream_util.hpp

namespace util{
    std::pair<int, int> getTerminalDimen();

    inline bool align_text_center(const std::string & = "");                          //With empty string, it sets the cursor to the horizontal center
    inline bool align_text_center(unsigned int max_length, const std::string & = ""); //NOTE - This functions does NOT add an extra '\n' at end
    inline bool place_center(unsigned int max_length, const std::string & = "");
    /*@brief Places the cursor at end of vertically centered printed string
      @params Terminal Height, and the string to print
      @returns bool indicating success or faiure to print the string
      NOTE - the string is not horizontally centered, for that, first call place_v_center() with empty string, then align_text_center()*/
    inline bool place_v_center(unsigned int v_length, const std::string & = "");
    inline bool place_v_center(const std::string & = ""); //Places cursor at vecrtically middle line

}

    //DEFINITIONS

#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#elif __MINGW32
#include <windows.h>
#elif __CYGWIN32
#include <windows.h>
#else //@todo - replace with flag for MSVC
#include <Windows.h>
#endif

//returns row*column dimension
std::pair<int, int> util::getTerminalDimen()
{
    std::pair<int, int> outTuple(0, 0);
#ifdef __linux__
    winsize windowsSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
    outTuple = {windowsSize.ws_row, windowsSize.ws_col};

#else //Windows API
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        outTuple = {csbi.srWindow.Bottom - csbi.srWindow.Top + 1, csbi.srWindow.Right - csbi.srWindow.Left + 1};
    }

#endif

    return outTuple;
}

bool util::align_text_center(const std::string &str)
{ //@returns bool indicating, if max_len is enough or not

    return align_text_center(getTerminalDimen().first, str);
}

bool util::align_text_center(unsigned int max_length, const std::string &str)
{ //@returns bool indicating, if max_len is enough or not
    if (place_center(max_length, str) == true)
    {
        for (size_t i = 0; i < (max_length - str.size()) - (max_length - str.size()) / 2; i++)
            std::cout << ' ';
        return true;
    }
    return false;
}

bool util::place_center(unsigned int max_length, const std::string &str)
{ //@returns bool indicating, if max_len is enough or not
    if (max_length < str.size())
    {
        return false;
    }
    else
    {
        for (size_t i = 0; i < (max_length - str.size()) / 2; i++)
            std::cout << ' ';
        std::cout << str;
    }
    return true;
}

bool util::place_v_center(const std::string &str)
{
    return place_v_center(getTerminalDimen().second, str);
}

bool util::place_v_center(unsigned int vert_length, const std::string &str)
{
    if (vert_length < 1)
        return false;

    for (size_t i = 0; i < (vert_length / 2) - 1; i++)
    {
        std::cout << '\n';
    }
    std::cout << str;

    return true;
}
