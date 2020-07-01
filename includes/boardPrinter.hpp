#pragma once

#include <iostream>
#include <array>
#include "ludo_box.h"
#include "utilities.hpp"

class _BoardPrinter{ //! @info Only for use by updateDisplay() & takeIntro()
private:
	/*@note Also clears the screen, so that the titlebar is at top
	  @returns Terminalstd::pair<int,int> as pair<int,int>*/

	//Common data that will be used
	const std::vector<std::vector<ludo_box>> &board;

	static void titleBar(int width);
	static void titleBar(); /*@brief Simply just calls titleBar with (terminalDimen().first)*/
	static void msgScreen(const std::string& msg);
	static void errorScreen(const std::string& errMsg);
    static void finishedScreen(void);
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
	explicit _BoardPrinter( const std::vector<std::vector<ludo_box>>& );	//! Links to the board
	_BoardPrinter() = delete;

};

//Defintions Start
_BoardPrinter::_BoardPrinter( const std::vector<std::vector<ludo_box>>& boardToLink ) : board(boardToLink){
	boxlen = 0;
	termWidth = customUtil::getTerminalDimen().first;
}

void _BoardPrinter::refresh_Dimensions(){
	termWidth = customUtil::getTerminalDimen().first;
}

//	DEFINITIONS	start//
void _BoardPrinter::msgScreen(const std::string& msg){
	titleBar();
	std::pair<int,int> termDimen = customUtil::getTerminalDimen();
	std::cout<<'\n';

	for(int i=0; i<(termDimen.second-3)/2; ++i)	std::cout<<'\n';
	customUtil::align_text_center(customUtil::getTerminalDimen().first, msg);
	for(int i=0; i<termDimen.second - (termDimen.second-3)/2; ++i)	std::cout<<'\n';
}

void _BoardPrinter::errorScreen(const std::string& errMsg){
	_BoardPrinter::msgScreen(errMsg);
}

void _BoardPrinter::finishedScreen(){
	_BoardPrinter::msgScreen("Khelne ke liye Dhanyawaad :D ");
}

void _BoardPrinter::titleBar(int width){	//Considering sufficient width, to be able to play the game

	 #ifdef __linux__
	 	system("clear");
	 #elif _WIN32
	 	system("cls")
	 #endif

	std::cout<<std::endl;
	customUtil::align_text_center(width, "NAMASTE from \"Ludo - The Game\" :D");
	std::cout<<'\n';
	while (width--) std::cout<<'=';
}

void _BoardPrinter::titleBar(){

	return titleBar( customUtil::getTerminalDimen().first );
}

void _BoardPrinter::row_type1(int nrow){
	//Actual-Row Start
	std::cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*6 - 1; i++)
		std::cout<<'\\';

	std::cout<<'|';

	for (size_t i = 0; i < 3; i++){
		customUtil::align_text_center(boxlen, board[nrow][6+i].content);
		std::cout<<'|';
	}

	for (size_t i = 0; i < (boxlen+1)*6 - 1; i++)	std::cout<<'\\';
	std::cout<<"|\n";
	//Actual-Row End
}

void _BoardPrinter::row_type2(int nrow){
	//!Explanatory comments in _BoardPrinter::row_type1
	std::cout<<'|';

	for (size_t i = 0; i < boxlen; i++)	std::cout<<'\\';	
	customUtil::align_text_center(boxlen+2, board[nrow][1].content);
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	std::cout<<'\\';
	customUtil::align_text_center(boxlen+2, board[nrow][4].content);
	for (size_t i = 0; i < boxlen; i++) std::cout<<'\\';
	std::cout<<'|';

	for (size_t i = 0; i < 3; i++){
		customUtil::align_text_center(boxlen, board[nrow][6+i].get_box_content());
		std::cout<<'|';
	}

	for (size_t i = 0; i < boxlen; i++)	std::cout<<'\\';
	customUtil::align_text_center(boxlen+2, board[nrow][10].get_box_content());
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	std::cout<<'\\';
	customUtil::align_text_center(boxlen+2, board[nrow][13].get_box_content());
	for (size_t i = 0; i < boxlen; i++) std::cout<<'\\';
	std::cout<<"|\n";
}


void _BoardPrinter::row_type3(int nrow){
	//!Explanatory comments in _BoardPrinter::row_type1
	std::cout<<'|';
	for (size_t i = 0; i < 6; i++){
		customUtil::align_text_center(boxlen, board[nrow][i].get_box_content());
		std::cout<<'|';
	}

	for (size_t i = 0; i < (boxlen+1)*3 -1; i++) std::cout<<' ';	
	std::cout<<'|';

	for (size_t i = 9; i < 15; i++){
		customUtil::align_text_center(boxlen, board[nrow][i].get_box_content());
		std::cout<<'|';
	}
	std::cout<<'\n';
}

void _BoardPrinter::row_type4(int nrow){
	//!Explanatory comments in _BoardPrinter::row_type1
	std::cout<<'|';
		
	customUtil::align_text_center(boxlen, board[nrow][0].get_box_content());	std::cout<<'|';
	for (size_t i = 1; i < 5; i++){
		customUtil::align_text_center(boxlen, board[nrow][i].get_box_content());
		std::cout<<' ';
	}
	customUtil::align_text_center(boxlen, board[nrow][5].get_box_content());  std::cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*3 -1; i++) std::cout<<' ';	
	std::cout<<'|';

	for (size_t i = 9; i < 13; i++){
		customUtil::align_text_center(boxlen, board[nrow][i].get_box_content());	std::cout<<' ';
	}
	for (size_t i = 13; i < 15; i++){
		customUtil::align_text_center(boxlen, board[nrow][i].get_box_content());
		std::cout<<'|';
	}
	std::cout<<'\n';
}

void _BoardPrinter::inter_type1(){
		//Inter-Row line Start
    customUtil::place_center(termWidth - 15*(boxlen+1) +3 -4);
	std::cout<<"  |";
	for (size_t i = 0; i < boxlen; i++)	std::cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) std::cout<<'-';
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	std::cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) std::cout<<'-';
	for (size_t i = 0; i < boxlen; i++) std::cout<<'\\';
	std::cout<<'|';

	for(int j=0; j<3; ++j){
		for (size_t i = 0; i < (boxlen); i++) std::cout<<'-';
        std::cout<<'|';
	}

	for (size_t i = 0; i < boxlen; i++)	std::cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) std::cout<<'-';
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	std::cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) std::cout<<'-';
	for (size_t i = 0; i < boxlen; i++) std::cout<<'\\';
	std::cout<<"|\n";
		//Inter-Row Line end
}

void _BoardPrinter::inter_type2(){
    customUtil::place_center(termWidth - 15*(boxlen+1) +3 -4);
	std::cout<<"  |";
	for (size_t i = 0; i < boxlen; i++)	std::cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) std::cout<<'-';
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	std::cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) std::cout<<'-';
	for (size_t i = 0; i < boxlen; i++) std::cout<<'\\';
	std::cout<<'|';

	for (size_t i = 0; i < (boxlen); i++) std::cout<<'-';
    	std::cout<<'|';
	for (size_t i = 0; i < (boxlen); i++) std::cout<<' ';
    std::cout<<'|';
	for (size_t i = 0; i < (boxlen); i++) std::cout<<'-';
	std::cout<<'|';

	for (size_t i = 0; i < boxlen; i++)	std::cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) std::cout<<'-';
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	std::cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) std::cout<<'-';
	for (size_t i = 0; i < boxlen; i++) std::cout<<'\\';
	std::cout<<"|\n";
}

void _BoardPrinter::inter_type3(){
    customUtil::place_center(termWidth - 15*(boxlen+1) +3 -4);	std::cout<<"  |";
	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	std::cout<<'\\';
	std::cout<<'|';

	for (size_t i = 0; i < (boxlen); i++) std::cout<<'-';
    	std::cout<<'|';
	for (size_t i = 0; i < (boxlen); i++) std::cout<<' ';
    std::cout<<'|';
	for (size_t i = 0; i < (boxlen); i++) std::cout<<'-';
	std::cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	std::cout<<'\\';
	std::cout<<"|\n";
}

void _BoardPrinter::inter_type4(){
    customUtil::place_center(termWidth - 15*(boxlen+1) +3 -4);
	std::cout<<"  |";
	for (size_t i = 0; i < (boxlen+1)*15-1; i++)	std::cout<<'-';
	std::cout<<"|\n";
}

void _BoardPrinter::inter_type5(){
    customUtil::place_center(termWidth - 15*(boxlen+1) +3 -4);
	std::cout<<"  |";
	for (size_t i = 0; i < (boxlen+1)*6-1; i++) std::cout<<'-';
    std::cout<<'|';
	for (size_t i = 0; i < (boxlen+1)*3-1; i++) std::cout<<' ';
    std::cout<<'|';
	for (size_t i = 0; i < (boxlen+1)*6-1; i++) std::cout<<'-';
    std::cout<<"|\n";
}
