#pragma once

#define CUSTOM_BOARD 1

//! -> Use this syntax for giving info of the code

// #ifndef CUSTOM_LUDO_GOTI
//     #include "./ludo_goti.hpp"
// #endif

#include "ludo_box.hpp"
#include "ludo_coords.hpp"
// #include<enumerations.hpp>
// #include "ludo_goti.hpp"
// #include<simpleTuple.hpp>
#include<unordered_set>
#include<map>
#include<array>
#include<memory>
#include<functional>
#include<random>
// #include<vector>

#define DEBUG_PRINTBOARD cout<<"------------DEBUG-------------";\
							for(auto &i : board) {\
							std::cout<<"\n|";\
							for(auto &j : i) {\
								std::cout<<j.get_box_content()<<' ';\
							}\
							std::cout<<'|';\
						 }\
						 cout<<"\n------------DEBUG-------------\n";

//TODO - Make it visible only in the files that include this, 'static' not working, try it
enum rowLockStatus{ //Used in updateDisplay()
	_ROW_NOT_LOCKED = 0,
	_ROW_CONTAINS_LOCKED_GOTIS = 1
};

namespace Die{
	//[SOLVED]Bug - When non-static, these show mulitple 'definitions'
	static std::random_device dev_engine;	//[LEARNT] We use random_device to seed the 'random generator' named 'mt'
	static std::array<std::mt19937, 4> mt = { std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()) };
	//[LEARNT] random_device is slower than mt19937, but doesnt matter much, since its used to seed only once
	//the Mersenn(-_-) twister is about 10x faster than default C random generator
	static std::array<std::uniform_int_distribution<int>, 4> dist = { std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6) };	//uniform distribution from [1,6]
}

class game{

	/*POSSIBLE POINTS FOR CHECKING - 
		playerMap uses statically generated pairs, check whether goti is being prepared there
		Be cautius that goti are created ONLY ONCE AT CONSTRUCTION, later only pass references or pointers, not ANY ludo_goti object*/

public:	//TODO - Organize into private and public
	std::array<std::array<ludo_box,15>,15> board;

	//TODO - Append gotis to these locked Positions, using shared_ptr<ludo_goti>
	std::map<colours, std::vector<std::reference_wrapper<ludo_box>>> lockedPositions;
	// std::map<colours, std::array< std::reference_wrapper<ludo_box>,4 >> lockedPositions; //QUESTION - How to create an array of reference_wrappers??
	// std::map<colours, std::array< std::weak_ptr<ludo_box>,4 >> lockedPositions;	//QUESTION - Which pointer to use?

	// std::map<colours,std::unordered_set<ludo_goti>> trial;
	// std::map<colours, std::unordered_set<std::unique_ptr<ludo_goti>>> lockedGotis;	//@brief can't explain, but the declaration says it all... "It is a map 'of colours' to an 'unordered_set of gotis'"
	std::map<colours, std::vector<std::shared_ptr<ludo_goti>>> movingGotis;

	/*SharedPtr Users  -movingGotis, and playersMap*/

	std::map<colours, player> coloursMap;
	std::map<player, std::pair< std::string, colours >> activePlayerMap;
/*  @guide
	PLAYER std::map
	======================
	PlayerID(enum)  ---->
						1. std::string PlayerName
						2. colour GotiColour

	Similarly, a more complex Colourstd::map MAYBE implemented, but for now the 'players' and 'colours' enum correspond
*/

	//Think - It maybe a good idea to have the below as member vars, instead of sharing and creating these vars when passed b/w methods
	player currentPlayer;
	colours currentGotiColour;
	std::map<colours, unsigned short> numfinishedGotis;
	std::map<colours, unsigned short> numlockedGotis = {{ColourLAAL, 4},{ColourHARA, 4},{ColourPEELA, 4},{ColourNEELA, 4}};
	
	int number_of_GameRuns;
	int min_boxlen;
	int goti_per_user;
		//NOTE - To get num_goti of any colour, instead use "movingGotis[gotiColour].size()"
		//@note - their difference with 4 is the num_locked goti
	

	ludo_coords _ludo_coords;
	std::array<colours,4> colourOrder;	//@brief 1st in this order is the one at bottom left, and next ones anti-clockwise to this

	void player_move(std::vector<int>&);	//TODO - @brief takes input of consequent die roll values, in case of 6s...
	/*IMP_NOTE - 
		Have two more branches - Mode1 and Mode2
			In Mode1, you move after your dieroll results in a non-6, ie. you may have an array of consequent values
				this is 'similar to'(since, that is actually a hybrid of mode1 & 2) how we play at gaon, lie, we say '6 6 3', so we move them all too at times
			In Mode2, you have to move with each throw (like in mobile versions), so no case of vector, nor this function*/
	bool moveGoti(std::shared_ptr<ludo_goti>, unsigned int);

	/*TODO - Lock the weak_ptr before passing the first argument of either attack or moveGoti*/
	bool attack(std::shared_ptr<ludo_goti> to_be_removed, std::weak_ptr<ludo_goti>& attacker, const std::pair<int,int>& coords);
	/* @brief Simply removes the 1st goti, if attack request found valid
	   @returns bool indicating, if he gets an extra die roll
	   NOTE- IT's called by moveGoti if it founds the coordinates are same for two gotis of different colours
		*/

	/*  @brief Simply moves a goti of same colour from the locked goti positions,
			   and std::move the goti to movingGotis, and the std::make_shared to starting box
		@returns bool indicating whether enough locked gotis were available*/
	bool unlockGoti(const colours);
	bool add_to_lockedGoti(std::shared_ptr<ludo_goti>);
	void takeIntro();	//@brief Initializes the PlayerMap
	bool isPlayerPlaying(player);
	int rolldie(player);
	void updateDisplay(player);
	std::pair<int,int> getEmptyLocks(colours);

	//Bool return values are usually for debugging purposes
	bool InitGame(short = 1);	//@brief Starts/Resets the game
	void play(bool = true);
	void automate_play();

	ludo_box& getBoardBox(const std::pair<int,int>& coords);	//@brief operates on 'board' array

	game();
	~game();

	friend class _BoardPrinter;

};

class _BoardPrinter{ //@info Only for use by updateDisplay() & takeIntro()
private:
	/*@brief NOTE 1 -also clears the screen, so that the titlebar is at top
	  @returns Terminalstd::pair<int,int> as pair<int,int>*/
	static std::pair<int,int> titleBar(int width);
	static std::pair<int,int> titleBar(); /*@brief Simply just calls titleBar with (terminalDimen().first)*/
	static void errorScreen(std::string errMsg);
    static void type1(int boxlen,int nrow,const std::array<std::array<ludo_box,15>,15>& board,rowLockStatus = _ROW_NOT_LOCKED);
    static void type2(int,int,const std::array<std::array<ludo_box,15>,15>& board,rowLockStatus = _ROW_CONTAINS_LOCKED_GOTIS);
    static void type3(int,int,const std::array<std::array<ludo_box,15>,15>& board,rowLockStatus = _ROW_NOT_LOCKED);
    static void type4(int,int,const std::array<std::array<ludo_box,15>,15>& board,rowLockStatus = _ROW_NOT_LOCKED);
    static void type5(int,int,const std::array<std::array<ludo_box,15>,15>& board,rowLockStatus = _ROW_NOT_LOCKED);
    static void type6(int,int,const std::array<std::array<ludo_box,15>,15>& board,rowLockStatus = _ROW_NOT_LOCKED);
    static void type7(int,int,const std::array<std::array<ludo_box,15>,15>& board,rowLockStatus = _ROW_NOT_LOCKED); //@brief Similar to type 5, only Inter-Row line different
    static void type8(int,int,const std::array<std::array<ludo_box,15>,15>& board,rowLockStatus = _ROW_NOT_LOCKED); //@brief Similar to type 4, only Inter-Row line different

    friend void game::updateDisplay(player);
    friend void game::takeIntro();
\
};

//[DONE](using smart pointers) iterator through whole board, and 'delete ludo_goti initialised with unlockGoti()'

//  FUTURE+ERRORS  //
//[FUTURE] - Consider the chances of using std::list or std::set in places where std::vector has been used...
	//[FUN][FUTURE] - if(number of that colour gotis = 0 earlier), then
	//Congratulate THE USER "Chal yaar, ek ludo_goti to nikli ;-)"
//[ERROR]: [at declaration of 'coord_class::stops'] Accessing these gives error: this declaration has no storage class or type specifier
