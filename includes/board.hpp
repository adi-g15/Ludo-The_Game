#pragma once

#define CUSTOM_BOARD 1

// #ifndef CUSTOM_LUDO_GOTI
//     #include "./ludo_goti.hpp"
// #endif

#include<utilities.hpp>
#include "ludo_box.hpp"
#include<enumerations.hpp>
#include<ludo_goti.hpp>
// #include<simpleTuple.hpp>
#include<unordered_set>
#include<map>
#include<functional>
#include<vector>

class game{
public:	//TODO - Organize into private and public
	std::vector<std::vector<ludo_box>> board;
	std::unordered_set<player> activePlayers;	//Actually order is necessary, but that will be managed by 'to be' friend takeIntro()
	std::map<colours, player> Coloursmap;
	std::map<player, std::pair< std::string,std::pair<colours ,std::vector< std::reference_wrapper<ludo_goti> >> >> playerMap;
/*  @guide
	PLAYER std::map
	======================
	PlayerID(enum)  ----> std::string PlayerName
						  	colour GotiColour
						  	std::vector of Gotis

	Similarly, a more complex Colourstd::map MAYBE implemented, but for now the 'players' and 'colours' enum correspond
*/

	player currentPlayer;
	colours currentGotiColour;
	bool firstRun;
	int goti_per_user;
	int num_laalGoti; 
	int num_hariGoti; 
	int num_peeliGoti; 
	int num_neeliGoti;

	void player_move(std::vector<int>&);	//TODO - @brief takes input of consequent die roll values, in case of 6s...
	/*IMP_NOTE - 
		Have two more branches - Mode1 and Mode2
			In Mode1, you move after your dieroll results in a non-6, ie. you may have an array of consequent values
				this is 'similar to'(since, that is actually a hybrid of mode1 & 2) how we play at gaon, lie, we say '6 6 3', so we move them all too at times
			In Mode2, you have to move with each throw (like in mobile versions), so no case of vector, nor this function*/
	void moveGoti(ludo_goti&, unsigned int);
	void startGoti(const colours);
	ludo_box& getBox(const ludo_goti&);	//May make this a 'private' function in a class
	void takeIntro();	//@brief Initializes the PlayerMap
	bool isPlayerPlaying(player);
	int rolldie();
	void updateDisplay();

	bool InitGame(short = 1);	//@brief Starts/Resets the game
	void play(bool = true);

	game();
	~game();	//@brief iterator through whole board, and 'delete ludo_goti initialised with startGoti()'

	friend class _BoardPrinter;

};

class _BoardPrinter{ //@info Only for use by updateDisplay() function
private:
    static void type1(int,int,const std::vector<std::vector<ludo_box>>& board);
    static void type2(int,int,const std::vector<std::vector<ludo_box>>& board);
    static void type3(int,int,const std::vector<std::vector<ludo_box>>& board);
    static void type4(int,int,const std::vector<std::vector<ludo_box>>& board);
    static void type5(int,int,const std::vector<std::vector<ludo_box>>& board);
    static void type6(int,int,const std::vector<std::vector<ludo_box>>& board);

    friend void game::updateDisplay();

};

class ludo_coords{
public:
	static std::vector<intTuple> stops;
	static std::map<intTuple, direction> outer_corners, inner_turns;
	static std::map<intTuple, direction> home_turns;
	static std::map<colours,intTuple> start_coords;

	static void InitCoords(void);	//NOTE - This must be called to be able to use the coords
	static intTuple get_startCoords(colours);

};

// namespace ludo_coords{	//ERROR : It showed linking error of multiple 'definitions' in board.o,main.o and ludo_goti.o,main.o pairs

// 	std::vector<intTuple> stops;
// 	std::map<intTuple, direction> outer_corners, inner_turns;
// 	std::map<intTuple, direction> home_turns;
// 	std::map<colours,intTuple> start_coords;

// 	void InitCoords(void);	//NOTE - This must be called to be able to use the coords
// 	intTuple get_startCoords(colours);

// };

	/*ERROR - 
	This namespace is causing these linking errors, due to being included into two exclusive files (board.cpp, ludo_goti.cpp and main.cpp):
	g++ build/main.o -Llibs -lutil -lludo_static -o build/game
libs/libludo_static.a(board.o):(.bss+0x0): multiple definition of `ludo_coords::stops'
build/main.o:(.bss+0x0): first defined here
libs/libludo_static.a(board.o):(.bss+0x20): multiple definition of `ludo_coords::outer_corners'
build/main.o:(.bss+0x20): first defined here
libs/libludo_static.a(board.o):(.bss+0x60): multiple definition of `ludo_coords::inner_turns'
build/main.o:(.bss+0x60): first defined here
libs/libludo_static.a(board.o):(.bss+0xa0): multiple definition of `ludo_coords::home_turns'
build/main.o:(.bss+0xa0): first defined here
libs/libludo_static.a(board.o):(.bss+0xe0): multiple definition of `ludo_coords::start_coords'
build/main.o:(.bss+0xe0): first defined here
libs/libludo_static.a(ludo_goti.o):(.bss+0x0): multiple definition of `ludo_coords::stops'
build/main.o:(.bss+0x0): first defined here
libs/libludo_static.a(ludo_goti.o):(.bss+0x20): multiple definition of `ludo_coords::outer_corners'
build/main.o:(.bss+0x20): first defined here
libs/libludo_static.a(ludo_goti.o):(.bss+0x60): multiple definition of `ludo_coords::inner_turns'
build/main.o:(.bss+0x60): first defined here
libs/libludo_static.a(ludo_goti.o):(.bss+0xa0): multiple definition of `ludo_coords::home_turns'
build/main.o:(.bss+0xa0): first defined here
libs/libludo_static.a(ludo_goti.o):(.bss+0xe0): multiple definition of `ludo_coords::start_coords'
build/main.o:(.bss+0xe0): first defined here
*/

direction dir_to_turn_ifCoord_in_CornerCoordsVec(const intTuple& curr_coords, const std::map<intTuple, direction> &cornerCoord);

//FUTURE - Shift to std::unordered_map from std::map

//  FUTURE+ERRORS  //
	//[FUN][FUTURE] - if(number of that colour gotis = 0 earlier), then
	//Congratulate THE USER "Chal yaar, ek ludo_goti to nikli ;-)"
//[ERROR]: [at declaration of 'coord_class::stops'] Accessing these gives error: this declaration has no storage class or type specifier