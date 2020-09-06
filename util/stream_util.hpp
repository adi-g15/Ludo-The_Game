#pragma once

#include <iostream>
#include <map>

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
/* Colour any output, due to the code, just use COLOUR_THIS*/
#define COLOUR_THIS(colour, code)  \
    util::_colouredOutput(colour); \
    code                           \
    util::_resetColouredOutput();
/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

//LINKER_ERROR - When this fuinction is uncommented, and When utilities.hpp included in ludo_box.cpp, and game.cpp, then 'multiple defintion errors show up for the mebmbers of the util namespace (Wait...WHAT? WHY?... I dont know now)'
// template<typename T1, typename T2>
// static inline std::ostream& operator<<(std::ostream& out, const std::pair<T1,T2>& p){
// 	return out<<'('<<p.first<<", "<<p.second<<')';
// }

// inline std::ostream& operator<<(std::ostream& out, const std::pair<int,int>& p){
// 	return out<<'('<<p.first<<", "<<p.second<<')';
// }

namespace util
{
    enum _colour
    {
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,

        RESET
    };

    static std::map<_colour, std::string> terminalColourMapping({
        {RED, "[0;31m"},
        {GREEN, "[0;32m"},
        {YELLOW, "[0;33m"},
        {BLUE, "[0;34m"},
        {MAGENTA, "[0;35m"},
        {CYAN, "[0;36m"},
        {RESET, "[0m"},
    });

    void printcoloured(char, _colour colourName, bool isBold = false);
    void printcoloured(std::string, _colour colourName, bool isBold = false);
    void _colouredOutput(_colour colourName, bool isBold = false);
    void _resetColouredOutput();
}

    //DEFINTIONS
void util::_colouredOutput(_colour colourName, bool isBold)
{
    if (!isBold)
    {
        printf("%s", terminalColourMapping[colourName].c_str());
    }
    else
    { //ie. Text is bold
        std::string colourStr;

        if (colourName != YELLOW)
        {
            colourStr = terminalColourMapping[colourName];
            colourStr[1] = '1';
            //! '\033' is the escape sequence
            printf("\033%s\n", colourStr.c_str());
        }
        else
        {
            colourStr = "[01" + terminalColourMapping[colourName].substr(2);
            printf("\033%s\n", colourStr.c_str());
        }
    }
}

void util::printcoloured(char character, _colour colourName, bool isBold)
{
    _colouredOutput(colourName, isBold);
    std::cout << character;
    _resetColouredOutput();
}

void util::printcoloured(std::string str, _colour colourName, bool isBold)
{
    _colouredOutput(colourName, isBold);
    printf("%s", str.c_str());
    _resetColouredOutput();
}

void util::_resetColouredOutput()
{
    printf("\033%s", terminalColourMapping[RESET].c_str());
}
