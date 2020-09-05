#pragma once
//IMPORTANT NOTE - Import utilities at top (overridden by enumerations)

#define CUSTOM_UTIL_FUNC
#define stringify(name) #name

#include <functional>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
#define DEBUG_START std::cout << "[DEBUG] START :" << endl;
#define DEBUG_END std::cout << "\n: [DEBUG] PASSED" << endl;
/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
/*HELPFUL FOR GETTING TO THE SEG_FAULT SITE*/
#define DEBUG_BLOCK(code) \
	DEBUG_START           \
	code                  \
		DEBUG_END
/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
/* Colour any output, due to the code, just use COLOUR_THIS*/
#define COLOUR_THIS(colour, code)        \
	customUtil::_colouredOutput(colour); \
	code                                 \
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

	//! NOTE - The bool values are generally for debugging purposes, and may/may not have been ignored in code
	std::pair<int, int> getTerminalDimen();

	static std::map<_colour, std::string> terminalColourMapping({
		{RED, "[0;31m"},
		{GREEN, "[0;32m"},
		{YELLOW, "[0;33m"},
		{BLUE, "[0;34m"},
		{MAGENTA, "[0;35m"},
		{CYAN, "[0;36m"},
		{RESET, "[0m"},
	});

	//! NOTE - The bool values are generally for debugging purposes, and may/may not have been ignored in code
	std::pair<int, int> getTerminalDimen();
	inline bool align_text_center(const std::string & = "");						  //With empty string, it sets the cursor to the horizontal center
	inline bool align_text_center(unsigned int max_length, const std::string & = ""); //NOTE - This functions does NOT add an extra '\n' at end
	inline bool place_center(unsigned int max_length, const std::string & = "");
	/*@brief Places the cursor at end of vertically centered printed string
      @params Terminal Height, and the string to print
      @returns bool indicating success or faiure to print the string
      NOTE - the string is not horizontally centered, for that, first call place_v_center() with empty string, then align_text_center()*/
	inline bool place_v_center(unsigned int v_length, const std::string & = "");
	inline bool place_v_center(const std::string & = ""); //Places cursor at vecrtically middle line
	void trim(std::string&);								  //@returns trimmed std::string, but doesnt modify original string
	std::string trim_copy(const std::string&);								  //@returns trimmed std::string, but doesnt modify original string
	void strip(std::string &, char toRemove);		  /*Returns a string without the passed character*/
	std::string strip_copy(const std::string&, char toRemove);		  /*Returns a string without the passed character*/
	inline void pause(float sec);
	/*@brief Returns true if `num` can be written as a sum of some elements of the vector v; Else returns false*/
	bool isSum(unsigned long num, const std::vector<unsigned short> &v);
	std::vector<unsigned short> isSumOfElements(unsigned int num, const std::vector<unsigned short> &v);
	void printcoloured(char, _colour colourName, bool isBold = false);
	void printcoloured(std::string, _colour colourName, bool isBold = false);
	void _colouredOutput(_colour colourName, bool isBold = false);
	void _resetColouredOutput();
	inline bool icompare(const std::string &s1, const std::string &s2) noexcept;

} // namespace customUtil
//  FUNCTIONS END//

//Defintions Start
#include <algorithm>

#ifdef __linux__
	#include <sys/ioctl.h>
	#include <unistd.h>
#elif __MINGW32
	#include <windows.h>
#elif __CYGWIN32
	#include <windows.h>
#else	//@todo - replace with flag for MSVC
	#include <Windows.h>
#endif

std::pair<int, int> customUtil::getTerminalDimen()
{
	std::pair<int, int> outTuple(0, 0);
#ifdef __linux__
	winsize windowsSize;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
	outTuple = {windowsSize.ws_col, windowsSize.ws_row};

#else //Windows API
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		outTuple = {csbi.srWindow.Bottom - csbi.srWindow.Top + 1, csbi.srWindow.Right - csbi.srWindow.Left + 1};
	}

#endif

	return outTuple;
}

bool customUtil::align_text_center(const std::string &str)
{ //@returns bool indicating, if max_len is enough or not

	return align_text_center(getTerminalDimen().first, str);
}

bool customUtil::align_text_center(unsigned int max_length, const std::string &str)
{ //@returns bool indicating, if max_len is enough or not
	if (place_center(max_length, str) == true)
	{
		for (size_t i = 0; i < (max_length - str.size()) - (max_length - str.size()) / 2; i++)
			std::cout << ' ';
		return true;
	}
	return false;
}

bool customUtil::place_center(unsigned int max_length, const std::string &str)
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

bool customUtil::place_v_center(const std::string &str)
{
	return place_v_center(getTerminalDimen().second, str);
}

bool customUtil::place_v_center(unsigned int vert_length, const std::string &str)
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

	//@todo - Chose the better one among this, and the one in calculator project
void customUtil::trim(std::string& s)
{
	s.erase(std::find_if(s.begin(), s.end(), isspace), s.rend().base());
	s.erase(std::find_if(s.rbegin(), s.rend(), isspace).base(), s.end());
}

std::string customUtil::trim_copy(const std::string& s){
	std::string str_out(s);
	customUtil::trim(str_out);
	return str_out;
}

void customUtil::strip(std::string& s, char toRemove)
{
	s.erase(
		std::remove(s.begin(), s.end(), toRemove),
		s.end()
	);
}

std::string customUtil::strip_copy(const std::string& s, char toRemove){
	std::string str_out(s);
	customUtil::strip(str_out, toRemove);

	return str_out;
}

void customUtil::pause(float seconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(seconds*1000)));
}

void customUtil::_colouredOutput(_colour colourName, bool isBold)
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

void customUtil::printcoloured(char character, _colour colourName, bool isBold)
{
	_colouredOutput(colourName, isBold);
	std::cout << character;
	_resetColouredOutput();
}

void customUtil::printcoloured(std::string str, _colour colourName, bool isBold)
{
	_colouredOutput(colourName, isBold);
	printf("%s", str.c_str());
	_resetColouredOutput();
}

void customUtil::_resetColouredOutput()
{
	printf("\033%s", terminalColourMapping[RESET].c_str());
}

bool customUtil::isSum(unsigned long num, const std::vector<unsigned short> &v)
{
	auto total = 1 << v.size();
	unsigned long sum;

	for (int i = 0; i < total; ++i)
	{
		sum = 0;
		for (size_t j = 0; j < v.size(); ++j)
		{
			if (i & 1 << j)
			{ //Cheching if the jth bit is set
				sum += v[j];
			}
		}
		if (num == sum)
			return true;
	}
	return false;
}

std::vector<unsigned short> customUtil::isSumOfElements(unsigned int num, const std::vector<unsigned short> &v)
{
	unsigned int sum = 0;
	auto total = 1 << v.size();
	std::vector<unsigned short> elements;

	for (int i = 0; i < total; ++i)
	{
		for (size_t j = 0; j < v.size(); ++j)
		{
			if (i & 1 << j)
			{ //Cheching if the jth bit is set
				elements.push_back(v[j]);
				sum += v[j];
			}
		}
		if (num == sum)
			return elements;
		sum = 0;
		elements.clear();
	}
	return {};
}

inline bool customUtil::icompare(const std::string &s1, const std::string &s2) noexcept
{
	for (size_t i = 0; i < s1.size(); ++i)
	{
		if (std::tolower(s1[i]) != tolower(s2[i]))
			return false;
	}
	return true;
}
