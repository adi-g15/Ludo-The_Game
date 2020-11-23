#pragma once

#include "ludo_box.hpp"
#include "rang.hpp"
#include "util/terminal.hpp"

#include <iostream>
#include <array>
#include <string>
#include <cstdarg>

class _BoardPrinter{ //! @info Only for use by updateDisplay() & takeIntro()
	private:
	/*@note Also clears the screen, so that the titlebar is at top
	  @returns Terminalstd::pair<int,int> as pair<int,int>*/

	const std::vector<std::vector<ludo_box>>& board;	//board that this printer links to

	static void titleBar(int width);
	static void titleBar(); /*@brief Simply just calls titleBar with (terminalDimen().y)*/

	static void msgScreen(std::string_view msg);
	static void msgScreen(int n, ...);	//NOTE - The later arguments must be char*
	static void msgScreen(va_list args, int n);

	static void errorScreen(std::string_view errMsg);
	static void errorScreen(int n, ...);
	static void errorScreen(va_list args, int n);

	static void finishedScreen();
	/*FUTURE - It can be modified to show who's the 1st and who's the 4th, BUT FOR NOW I WILL NOT IMPLEMENT IT HERE, SINCE I HAVE FURTHER PENDING PLANS FOR GUI*/

	void row_type1(int nrow);
	void row_type2(int nrow);
	void row_type3(int nrow);
	void row_type4(int nrow);

	void inter_type1();
	void inter_type2();
	void inter_type3();
	void inter_type4();
	void inter_type5();

	void refresh_Dimensions();

	friend class game;

	public:
	unsigned int boxlen;
	int termWidth;
	explicit _BoardPrinter(const std::vector<std::vector<ludo_box>>&);	//! Links to the board
	_BoardPrinter() = delete;
};

//Defintions Start
_BoardPrinter::_BoardPrinter(const std::vector<std::vector<ludo_box>>& boardToLink) : board(boardToLink){
	this->boxlen = 0;
	this->termWidth = util::getTerminalDimen().x;
}

void _BoardPrinter::refresh_Dimensions(){
	this->termWidth = util::getTerminalDimen().x;
}

//	DEFINITIONS	start//
void _BoardPrinter::msgScreen(std::string_view msg){
	return _BoardPrinter::msgScreen(1, msg.data());
}

void _BoardPrinter::msgScreen(va_list args, int n){
	std::string msg;

	while( n-- ){
		try{
			msg.append(va_arg(args, char*));
		} catch( std::bad_alloc& e ){
			return _BoardPrinter::errorScreen(e.what());
		}
	}
	va_end(args);

	titleBar();
	coord termDimen = util::getTerminalDimen();
	std::cout << '\n';

	for( auto i = 0; i < (termDimen.y - 3) / 2; ++i )	std::cout << '\n';
	util::align_text_center(termDimen.x, msg);
	for( auto i = 0; i < termDimen.y - (termDimen.y - 3) / 2; ++i )	std::cout << '\n';
}

void _BoardPrinter::msgScreen(int n, ...){
	va_list args;
	va_start(args, n);
	_BoardPrinter::msgScreen(args, n);
	va_end(args);
}

void _BoardPrinter::errorScreen(std::string_view errMsg){
	_BoardPrinter::errorScreen(1, errMsg.data());
}

void _BoardPrinter::errorScreen(va_list args, int n){
	std::cout << rang::Fg::red << rang::Style::bold;

	_BoardPrinter::msgScreen(args, n);

	std::cout << rang::Fg::reset << rang::Style::reset;
}

void _BoardPrinter::errorScreen(int n, ...){
	va_list args;
	va_start(args, n);
	_BoardPrinter::errorScreen(args, n);
	va_end(args);
}

void _BoardPrinter::finishedScreen(){
	_BoardPrinter::msgScreen("Khelne ke liye Dhanyawaad :D ");
}

void _BoardPrinter::titleBar(int width){	//Considering sufficient width, to be able to play the game
#ifdef __linux__
	system("clear");
#elif _WIN32
	system("cls");
#endif

	std::cout << rang::Bg::reset;
	std::cout << rang::Fg::green << rang::Style::bold << std::endl;
	util::align_text_center(width, "NAMASTE from \"Ludo - The Game\" :D");
	std::cout << '\n';
	while( width-- ) std::cout << '=';
	std::cout << rang::Fg::reset << rang::Style::reset ;
	// rang::rang_implementation::resetAll();
}

void _BoardPrinter::titleBar(){
	return titleBar(util::getTerminalDimen().x);
}

void _BoardPrinter::row_type1(int nrow){
	//Actual-Row Start
	std::cout << '|';

	for( uint32_t i = 0; i < (boxlen + 1) * 6 - 1; i++ )
		std::cout << '\\';

	std::cout << '|';

	for(auto i = 0; i < 3; i++ ){
		util::align_text_center(boxlen, board[nrow][6 + i].content);
		std::cout << '|';
	}

	for(uint32_t i = 0; i < (boxlen + 1) * 6 - 1; i++ )	std::cout << '\\';
	std::cout << "|\n";
	//Actual-Row End
}

void _BoardPrinter::row_type2(int nrow){
	//!Explanatory comments in _BoardPrinter::row_type1
	std::cout << '|';

	for(uint32_t i = 0; i < boxlen; i++ )	std::cout << '\\';
	util::align_text_center(boxlen + 2, board[nrow][1].content);
	for(uint32_t i = 0; i < (boxlen) * 2 + 1; i++ )	std::cout << '\\';
	util::align_text_center(boxlen + 2, board[nrow][4].content);
	for(uint32_t i = 0; i < boxlen; i++ ) std::cout << '\\';
	std::cout << '|';

	for(auto i = 0; i < 3; i++ ){
		util::align_text_center(boxlen, board[nrow][6 + i].get_box_content());
		std::cout << '|';
	}

	for(uint32_t i = 0; i < boxlen; i++ )	std::cout << '\\';
	util::align_text_center(boxlen + 2, board[nrow][10].get_box_content());
	for(uint32_t i = 0; i < (boxlen) * 2 + 1; i++ )	std::cout << '\\';
	util::align_text_center(boxlen + 2, board[nrow][13].get_box_content());
	for(uint32_t i = 0; i < boxlen; i++ ) std::cout << '\\';
	std::cout << "|\n";
}


void _BoardPrinter::row_type3(int nrow){
	//!Explanatory comments in _BoardPrinter::row_type1
	std::cout << '|';
	for( size_t i = 0; i < 6; i++ ){
		util::align_text_center(boxlen, board[nrow][i].get_box_content());
		std::cout << '|';
	}

	for( size_t i = 0; i < (boxlen + 1) * 3 - 1; i++ ) std::cout << ' ';
	std::cout << '|';

	for( size_t i = 9; i < 15; i++ ){
		util::align_text_center(boxlen, board[nrow][i].get_box_content());
		std::cout << '|';
	}
	std::cout << '\n';
}

void _BoardPrinter::row_type4(int nrow){
	//!Explanatory comments in _BoardPrinter::row_type1
	std::cout << '|';

	util::align_text_center(boxlen, board[nrow][0].get_box_content());	std::cout << '|';
	for( size_t i = 1; i < 5; i++ ){
		util::align_text_center(boxlen, board[nrow][i].get_box_content());
		std::cout << ' ';
	}
	util::align_text_center(boxlen, board[nrow][5].get_box_content());  std::cout << '|';

	for( size_t i = 0; i < (boxlen + 1) * 3 - 1; i++ ) std::cout << ' ';
	std::cout << '|';

	for( size_t i = 9; i < 13; i++ ){
		util::align_text_center(boxlen, board[nrow][i].get_box_content());	std::cout << ' ';
	}
	for( size_t i = 13; i < 15; i++ ){
		util::align_text_center(boxlen, board[nrow][i].get_box_content());
		std::cout << '|';
	}
	std::cout << '\n';
}

void _BoardPrinter::inter_type1(){
	//Inter-Row line Start
	util::place_center(termWidth - 15 * (boxlen + 1) + 3 - 4);
	std::cout << "  |";
	for( size_t i = 0; i < boxlen; i++ )	std::cout << '\\';
	for( size_t i = 0; i < boxlen + 2; i++ ) std::cout << '-';
	for( size_t i = 0; i < (boxlen) * 2 + 1; i++ )	std::cout << '\\';
	for( size_t i = 0; i < boxlen + 2; i++ ) std::cout << '-';
	for( size_t i = 0; i < boxlen; i++ ) std::cout << '\\';
	std::cout << '|';

	for( int j = 0; j < 3; ++j ){
		for( size_t i = 0; i < (boxlen); i++ ) std::cout << '-';
		std::cout << '|';
	}

	for( size_t i = 0; i < boxlen; i++ )	std::cout << '\\';
	for( size_t i = 0; i < boxlen + 2; i++ ) std::cout << '-';
	for( size_t i = 0; i < (boxlen) * 2 + 1; i++ )	std::cout << '\\';
	for( size_t i = 0; i < boxlen + 2; i++ ) std::cout << '-';
	for( size_t i = 0; i < boxlen; i++ ) std::cout << '\\';
	std::cout << "|\n";
	//Inter-Row Line end
}

void _BoardPrinter::inter_type2(){
	util::place_center(termWidth - 15 * (boxlen + 1) + 3 - 4);
	std::cout << "  |";
	for( size_t i = 0; i < boxlen; i++ )	std::cout << '\\';
	for( size_t i = 0; i < boxlen + 2; i++ ) std::cout << '-';
	for( size_t i = 0; i < (boxlen) * 2 + 1; i++ )	std::cout << '\\';
	for( size_t i = 0; i < boxlen + 2; i++ ) std::cout << '-';
	for( size_t i = 0; i < boxlen; i++ ) std::cout << '\\';
	std::cout << '|';

	for( size_t i = 0; i < (boxlen); i++ ) std::cout << '-';
	std::cout << '|';
	for( size_t i = 0; i < (boxlen); i++ ) std::cout << ' ';
	std::cout << '|';
	for( size_t i = 0; i < (boxlen); i++ ) std::cout << '-';
	std::cout << '|';

	for( size_t i = 0; i < boxlen; i++ )	std::cout << '\\';
	for( size_t i = 0; i < boxlen + 2; i++ ) std::cout << '-';
	for( size_t i = 0; i < (boxlen) * 2 + 1; i++ )	std::cout << '\\';
	for( size_t i = 0; i < boxlen + 2; i++ ) std::cout << '-';
	for( size_t i = 0; i < boxlen; i++ ) std::cout << '\\';
	std::cout << "|\n";
}

void _BoardPrinter::inter_type3(){
	util::place_center(termWidth - 15 * (boxlen + 1) + 3 - 4);	std::cout << "  |";
	for( size_t i = 0; i < (boxlen + 1) * 6 - 1; i++ )	std::cout << '\\';
	std::cout << '|';

	for( size_t i = 0; i < (boxlen); i++ ) std::cout << '-';
	std::cout << '|';
	for( size_t i = 0; i < (boxlen); i++ ) std::cout << ' ';
	std::cout << '|';
	for( size_t i = 0; i < (boxlen); i++ ) std::cout << '-';
	std::cout << '|';

	for( size_t i = 0; i < (boxlen + 1) * 6 - 1; i++ )	std::cout << '\\';
	std::cout << "|\n";
}

void _BoardPrinter::inter_type4(){
	util::place_center(termWidth - 15 * (boxlen + 1) + 3 - 4);
	std::cout << "  |";
	for( size_t i = 0; i < (boxlen + 1) * 15 - 1; i++ )	std::cout << '-';
	std::cout << "|\n";
}

void _BoardPrinter::inter_type5(){
	util::place_center(termWidth - 15 * (boxlen + 1) + 3 - 4);
	std::cout << "  |";
	for( size_t i = 0; i < (boxlen + 1) * 6 - 1; i++ ) std::cout << '-';
	std::cout << '|';
	for( size_t i = 0; i < (boxlen + 1) * 3 - 1; i++ ) std::cout << ' ';
	std::cout << '|';
	for( size_t i = 0; i < (boxlen + 1) * 6 - 1; i++ ) std::cout << '-';
	std::cout << "|\n";
}
