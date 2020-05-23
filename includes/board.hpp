#pragma once

#include "ludo_box.hpp"
#include "ludo_coords.hpp"
#include<unordered_set>
#include<array>
#include<functional>
#include<random>

#define DEBUG_PRINTBOARD cout<<"------------DEBUG-------------";\
							for(auto &i : board) {\
							std::cout<<"\n|";\
							for(auto &j : i) {\
								std::cout<<j.get_box_content()<<' ';\
							}\
							std::cout<<'|';\
						 }\
						 cout<<"\n------------DEBUG-------------\n";


namespace Die{
	//! Using 4 different generators as well as sequences, one for each player
	static std::random_device dev_engine;
	static std::array<std::mt19937, 4> mt = { std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()) };
	static std::array<std::uniform_int_distribution<int>, 4> dist = { std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6) };	//uniform distribution from [1,6]
}

class game{
private:
	std::array<std::array<ludo_box,15>,15> board;
	std::map<colours, std::vector<std::reference_wrapper<ludo_box>>> lockedPositions;
	std::map<colours, std::vector<std::shared_ptr<ludo_goti>>> movingGotis;
	std::map<colours, unsigned short> numfinishedGotis;

	/*@brief 1st in this order is the one at bottom left, and next ones anti-clockwise to this*/
	std::array<colours,4> colourOrder;

	//!IMP_NOTE - These have been used in many member functions, instead of exchanging between them, this also ensures that they all are in sync, for eg. updateDisplay(), unlockGoti()
	player currentPlayer;
	colours currentGotiColour;
	int number_of_GameRuns;
	int min_boxlen;
	int goti_per_user;

	ludo_coords _ludo_coords;	//! An object to make the ludo_coords available to us

	bool unlockGoti();
	bool add_to_lockedGoti(std::shared_ptr<ludo_goti>);
	void takeIntro();	//! Initializes the PlayerMap

public:	
	std::map<colours, player> coloursMap;
/*  @guide
	PLAYER std::map
	======================
	PlayerID(enum)  ---->
						1. std::string PlayerName
						2. colour GotiColour

*/
	std::map<player, std::pair< std::string, colours >> activePlayerMap;

	short moveGoti(std::shared_ptr<ludo_goti>, unsigned int);

	/* @brief Simply removes the 1st goti, if attack request found valid
	   @returns bool indicating, if he gets an extra die roll
	   NOTE- IT's called by moveGoti if it founds the coordinates are same for two gotis of different colours
	*/
	bool attack(std::shared_ptr<ludo_goti> to_be_removed, std::weak_ptr<ludo_goti>& attacker, const std::pair<int,int>& coords);

	/*  @brief Simply moves a goti of same colour from the locked goti positions,
			   and std::move the goti to movingGotis, and the std::make_shared to starting box
		@returns bool indicating whether enough locked gotis were available*/
	bool isPlayerPlaying(player);
	int rolldie(player);
	void updateDisplay();
	unsigned short getNumGotis(colours);
	std::pair<int,int> getEmptyLocks(colours);

	//!Bool return values are usually for debugging purposes
	bool InitGame(short = 1);	//! Starts/Resets the game
	void play(bool = true);
	ludo_box& getBoardBox(const std::pair<int,int>& coords);

	game();
	~game();

	friend class _BoardPrinter;

};

class _BoardPrinter{ //! @info Only for use by updateDisplay() & takeIntro()
private:
	/*@note Also clears the screen, so that the titlebar is at top
	  @returns Terminalstd::pair<int,int> as pair<int,int>*/
	static void titleBar(int width);
	static void titleBar(); /*@brief Simply just calls titleBar with (terminalDimen().first)*/
	static void errorScreen(std::string errMsg);
    static void type1(int boxlen,int nrow,const std::array<std::array<ludo_box,15>,15>& board);
    static void type1_2(int,int,const std::array<std::array<ludo_box,15>,15>& board);
    static void type2(int,int,const std::array<std::array<ludo_box,15>,15>& board);
    static void type2_2(int,int,const std::array<std::array<ludo_box,15>,15>& board);
    static void type3(int,int,const std::array<std::array<ludo_box,15>,15>& board);
    static void type4(int,int,const std::array<std::array<ludo_box,15>,15>& board);
    static void type5(int,int,const std::array<std::array<ludo_box,15>,15>& board);
    static void type6(int,int,const std::array<std::array<ludo_box,15>,15>& board);
    static void type7(int,int,const std::array<std::array<ludo_box,15>,15>& board); //@brief Similar to type 5, only Inter-Row line different
    static void type8(int,int,const std::array<std::array<ludo_box,15>,15>& board); //@brief Similar to type 4, only Inter-Row line different

    friend void game::updateDisplay();
    friend void game::takeIntro();

};