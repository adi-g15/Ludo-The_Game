#include "game.h"

#include <iostream>
#include <utility>
#include <algorithm>
#include <set>
#include <sstream>
#include "boardPrinter.hpp"
#include "exceptions.h"

using namespace std;

//GAME_CLASS_DEFINTIONS START
/*Return values - -1 : Can't Move
				   0 : Normal
				   1 : Goti pung gayi*/
short game::moveGoti(std::shared_ptr<ludo_goti> the_goti, unsigned int dist){
	std::pair<int,int> increment_coord( {0,0} );
	std::pair<int,int> updated_coords( the_goti->curr_coords );

	direction turnDirection = NO_TURN;

	while(dist--){

		increment_coord = {0,0};
		turnDirection = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.outer_corners);	//! For Outer Corners
		if(turnDirection != NO_TURN){	//! ie. a turn will happen to go to next box

			the_goti->curr_direction = turnDirection;
			if(turnDirection == NORTH){
				increment_coord = {-1,0};
			}
			else if(turnDirection == EAST){
				increment_coord = {0,1};
			}
			else if(turnDirection == WEST){
				increment_coord = {0,-1};
			}
			else if(turnDirection == SOUTH){
				increment_coord = {1,0};
			}
		}
		else{
			turnDirection = _ludo_coords.turnAtCorner(updated_coords, _ludo_coords.inner_turns); //For Inner Turns
			if (turnDirection != NO_TURN)
			{
				the_goti->curr_direction = turnDirection;
				if(turnDirection == NORTH){
					increment_coord = {-1,1};
				}
				else if(turnDirection == EAST){
					increment_coord = {1,1};
				}
				else if(turnDirection == WEST){
					increment_coord = {-1,-1};
				}
				else if(turnDirection == SOUTH){
					increment_coord = {1,-1};
				}
			}
			else{
				//Checking for Home_Turns
				if( updated_coords == _ludo_coords.home_turns[the_goti->gotiColour].first )
				{
					turnDirection = _ludo_coords.home_turns[the_goti->gotiColour].second;
					the_goti->curr_direction = turnDirection;
				}

				//! ie. needs to 'go straight' on its current path
				if(the_goti->get_curr_direction() == NORTH){
					increment_coord = {-1,0};
				}
				else if(the_goti->get_curr_direction() == EAST){
					increment_coord = {0,1};
				}
				else if(the_goti->get_curr_direction() == WEST){
					increment_coord = {0,-1};
				}
				else if(the_goti->get_curr_direction() == SOUTH){
					increment_coord = {1,0};
				}
			}
		}
		updated_coords.first += increment_coord.first;
		updated_coords.second += increment_coord.second;

		if( getBoardBox(updated_coords).box_type == _boxHOME_END ){	//! ie. value of dist causes it to go beyond end position
			if( dist != 0 )return -1;
			else{
				auto lockBox = getBoardBox( getEmptyLocks(currentGotiColour) );
				lockBox.content = ":D";
				lockBox.box_type = _boxUNUSABLE;
				getBoardBox(the_goti->curr_coords).removeGoti( the_goti );	//Reaches here only if move possible
				the_goti->curr_coords = updated_coords;
				++numfinishedGotis[currentGotiColour];
				return 1;
			}
		}
	}

	getBoardBox(the_goti->curr_coords).removeGoti( the_goti );	//Reaches here only if move possible
	the_goti->curr_coords = updated_coords;
	getBoardBox(the_goti->curr_coords).appendGoti( the_goti );

	return 0;
}

bool game::attack(std::shared_ptr<ludo_goti> to_be_removed, std::weak_ptr<ludo_goti>& attacker,const std::pair<int,int>& coords){
	if(getBoardBox(coords).isPresent(*(to_be_removed)) && getBoardBox(coords).isPresent(*(attacker.lock()))){
		getBoardBox(coords).removeGoti(to_be_removed);
		
		auto iter = std::find(movingGotis[to_be_removed->gotiColour].begin(), movingGotis[to_be_removed->gotiColour].end(), to_be_removed);
		auto emptyLockBox = std::find_if(lockedPositions[to_be_removed->gotiColour].begin(), lockedPositions[to_be_removed->gotiColour].end(), [](std::reference_wrapper<ludo_box> &box){
			if(box.get().isEmpty() == 0)	return true;
			else return false;
		});

		emptyLockBox->get().appendGoti( to_be_removed );
		movingGotis[to_be_removed->gotiColour].erase(iter);
		add_to_lockedGoti(to_be_removed);

		return true;
	}
	else return false;
}

ludo_box& game::getBoardBox(const std::pair<int,int>& coords){
	return board[ coords.first ][ coords.second ];
}

bool game::unlockGoti(){
	if (currentGotiColour == UnknownColour)
	{
		cerr<<"unlockGoti() request made for Unknown Colour"<<endl;
		return false;
	}
	else if(movingGotis[currentGotiColour].size() >= goti_per_user){
		cerr<<"Goti request makes the number of ludo_goti of "<<stringify(currentGotiColour)<<" more than "<<goti_per_user<<endl;
		return false;
	}
	else{

		//First std::find_if gives us the raw reference of box
		auto box_iter = std::find_if(lockedPositions[currentGotiColour].begin(), lockedPositions[currentGotiColour].end(), [&](auto &lockedBox){
			return lockedBox.get().numGotis[currentGotiColour] > 0;
		});
				
		if( box_iter == lockedPositions[currentGotiColour].end()){	//no locked goti present
			return false;
		}

		auto goti = box_iter->get().inBoxGotis.back();	//Causing the SEG_Fault
		box_iter->get().removeGoti(goti);

		goti->curr_coords = _ludo_coords.start_coords[currentGotiColour];
		goti->curr_direction = _ludo_coords.start_dir[currentGotiColour];

		getBoardBox( goti->curr_coords ).appendGoti( goti );
		movingGotis[currentGotiColour].push_back( goti );
		
		return true;
	}
}

bool game::add_to_lockedGoti(std::shared_ptr<ludo_goti> goti_to_lock){

	goti_to_lock->curr_coords = {0,0};
	goti_to_lock->curr_direction = NO_TURN;	

	auto emptyBox = std::find_if(lockedPositions[goti_to_lock->gotiColour].begin(), lockedPositions[goti_to_lock->gotiColour].end(), [](std::reference_wrapper<ludo_box>& box){
		return box.get().isEmpty();
	});
	emptyBox->get().appendGoti(goti_to_lock);

}

void game::takeIntro(){
	std::pair<int,int> tmpDimen(0,0);
	tmpDimen = customUtil::getTerminalDimen();

	customUtil::place_v_center("[NOTICE] Please ensure window is at least 62*31");
	cout<<endl;

	do{
		tmpDimen = customUtil::getTerminalDimen();

	}while( tmpDimen.first < 62 || tmpDimen.second < 31 );

	_BoardPrinter::titleBar(tmpDimen.first);

	customUtil::place_v_center(tmpDimen.second - 7, "");
	customUtil::align_text_center(tmpDimen.first, "Enter names of the Players (Leave empty if not playing) : ");
	
	string playerName;
	player p=Player1;
	colours colour;
	
	colour = colourOrder[0];	
	
	while(p <= Player4){
		
		customUtil::place_center(tmpDimen.first, string("Player") + customUtil::intTostring(p) + string(" - "));		
		getline(cin, playerName, '\n');
		playerName = customUtil::trimString(playerName);
		if( playerName != ""){
			if( shortcutsMap.find(playerName) != shortcutsMap.end() ){
				shortcutsMap.find(playerName)->second("takeIntro");	//Calling using the function pointer
			}
			else{
				activePlayerMap.insert({{ p, { customUtil::trimString(playerName), colour} }});
			}
		}

		p = player(p+1);
		colour = colours(colour+1);
	}

	//Setting the board, on basis of given players
	for (auto &&player : activePlayerMap)
	{
		auto gotiColour = player.second.second;
		for(auto&& lockedBox : lockedPositions[gotiColour]){
			lockedBox.get().appendGoti(std::shared_ptr<ludo_goti>(new ludo_goti(gotiColour, getEmptyLocks(gotiColour))));
			lockedBox.get().box_type = _boxLOCK;
		}
		coloursMap.insert({player.second.second, player.first});
		numfinishedGotis.insert({player.second.second, 0});
	}

	for (auto &&stop : _ludo_coords.stops)
	{
		getBoardBox(stop).content = 'X';
	}

}

bool game::isPlayerPlaying(player p){
	return activePlayerMap.find(p) != activePlayerMap.end();
}

int game::rolldie(player ID){
	return Die::dist[ ID-1 ] (Die::mt[ID-1] );
}

void game::updateDisplay(){
	int boxlen = 0;
	std::pair<int,int> tmpDimen(0,0);

	do{	
		tmpDimen = customUtil::getTerminalDimen();
		boxlen = ( 2*min(tmpDimen.first,tmpDimen.second) - 32 )/15;

	}while( min(tmpDimen.first,tmpDimen.second) < 32 );

	_BoardPrinter::titleBar(tmpDimen.first);

    customUtil::place_center(tmpDimen.first - 15*(boxlen+1) +3 -4 );
	cout<<"  ";
	for (size_t i = 0; i < 10; i++){
		cout<<"  "<<i;
	}
	for (size_t i = 10; i < 15; i++){
		cout<<' '<<i;
	}
	cout<<"\n  ";
    customUtil::place_center(tmpDimen.first - 15*(boxlen+1) +3 -4 );

	for (size_t i = 0; i <(boxlen+1)*15 + 1; i++)	cout<<'-';
	cout<<"\n";

	#define BOARD_CENTER customUtil::place_center(tmpDimen.first - 15*(boxlen+1) +3 -4);

	_BoardPrinter _board_printer(board);
	_board_printer.boxlen = boxlen;

	BOARD_CENTER cout<<0<<' '; _board_printer.row_type1( 0 );	_board_printer.inter_type1();
	BOARD_CENTER cout<<1<<' '; _board_printer.row_type2( 1 );	_board_printer.inter_type2();
	BOARD_CENTER cout<<2<<' '; _board_printer.row_type1( 2 );	_board_printer.inter_type3();
	BOARD_CENTER cout<<3<<' '; _board_printer.row_type1( 3 );	_board_printer.inter_type2();
	BOARD_CENTER cout<<4<<' '; _board_printer.row_type2( 4 );	_board_printer.inter_type2();
	BOARD_CENTER cout<<5<<' '; _board_printer.row_type1( 5 );	_board_printer.inter_type4();


	BOARD_CENTER cout<<6<<' '; _board_printer.row_type3( 6 );	_board_printer.inter_type5();
	BOARD_CENTER cout<<7<<' '; _board_printer.row_type4( 7 );	_board_printer.inter_type5();
	BOARD_CENTER cout<<8<<' '; _board_printer.row_type3( 8 );	_board_printer.inter_type4();


	BOARD_CENTER cout<<9<<' '; _board_printer.row_type1( 9 );	_board_printer.inter_type2();
	BOARD_CENTER cout<<10; _board_printer.row_type2( 10 );	_board_printer.inter_type2();
	BOARD_CENTER cout<<11; _board_printer.row_type1( 11 );	_board_printer.inter_type3();
	BOARD_CENTER cout<<12; _board_printer.row_type1( 12 );	_board_printer.inter_type2();
	BOARD_CENTER cout<<13; _board_printer.row_type2( 13 );	_board_printer.inter_type1();
	BOARD_CENTER cout<<14; _board_printer.row_type1( 14 );

	cout<<"  ";
	BOARD_CENTER for (size_t i = 0; i <(boxlen+1)*15 + 1; i++)	cout<<'-';
	
	cout<<"\n\n";
	customUtil::align_text_center(activePlayerMap[currentPlayer].first);
	cout<<"\n";
	for(int i=0; i<tmpDimen.first; ++i) cout<<'-';
	cout<<'\n';

}

unsigned short game::getNumGotis(colours gotiColour){
	return movingGotis[gotiColour].size();
}

std::pair<int,int> game::getEmptyLocks(colours gotiColour){
	for (auto &&lockedBox : lockedPositions[gotiColour])
	{
		if(lockedBox.get().isEmpty() && lockedBox.get().box_type == _boxLOCK){
			return lockedBox.get().coords;
		}
	}
	return {0,0};
}

bool game::InitGame(short playerConsent){	//! 1 for complete reset, 2 is with previous playerMap

	if(playerConsent !=1 && playerConsent !=2){
		return false;
	}
	else{
		goti_per_user = 4;

		for (auto &&row : board)
		{
			for (auto &&box : row)
			{
				box.content.clear();
				box.inBoxGotis.clear();
				for (auto &&num_goti : box.numGotis)
				{
					num_goti.second = 0;
				}
			}
		}

		if( playerConsent == 1 ){	//not changed, in case it is 2
			activePlayerMap.clear();
			coloursMap.clear();

			takeIntro();
		}

		//coloursMap and activePlayerMap initialised by takeIntro()		
	}

	++number_of_GameRuns;

	return true;
}

void game::play(bool boolVal){
	if( !boolVal ){
		return;
	}
	else{

		auto lambda_next = [&](){
			std::vector<player> tmpVec;
			for (auto &&i : activePlayerMap)
			{
				tmpVec.push_back(i.first);
			}
			size_t pos = std::find(tmpVec.begin(), tmpVec.end(), currentPlayer) - tmpVec.begin();
			if( pos+1 < tmpVec.size() ){
				currentPlayer = tmpVec[ pos+1 ];
			}
			else currentPlayer = tmpVec[0];
			currentGotiColour = activePlayerMap[currentPlayer].second;
		};

		currentGotiColour = colourOrder[0];
		currentPlayer = coloursMap[currentGotiColour];

		std::vector<unsigned short> dieNumbers;
		std::pair<int,int> currentCoord = _ludo_coords.start_coords[ currentGotiColour ];

		while (true){
			
			updateDisplay();
			cout<<"\nPress Enter to roll the die...";
			cin.get();

			unsigned short currentRoll, enteredRoll;
			string inputStr;	//! Note - To use shortcuts like ":settings", ":rules, and to have 'variable number of inputs'
			unsigned short choice_num;

			do{
				currentRoll = rolldie(currentPlayer);
				dieNumbers.push_back(currentRoll);
			}while( currentRoll == 6 );

			cout<<"\nRoll Output - ";
			for (auto &&outNum : dieNumbers)
			{
				cout<<outNum<<' ';
			}

			auto lambda_isSIXinRoll = [&](){
				for (auto &&i : dieNumbers)
				{
					if(i == 6)
						return true;
				}
				return false;
			};

			if( movingGotis[currentGotiColour].empty() && !lambda_isSIXinRoll() ){	//No movable goti, so continue to next player
				cout<<"\nNo movable goti, for the moves"<<endl;
				
				customUtil::pause(1);

				lambda_next();
				dieNumbers.clear();
				continue;
			}

			while( !dieNumbers.empty() ){
    			updateDisplay();
				unsigned short counter = 1;
				cout<<"\nChose from these gotis : ";
				if( lambda_isSIXinRoll() ) cout<<"\n0. Unlock New Goti (just type 0)\n\n";
				for (auto &&goti_ptr : movingGotis[currentGotiColour])	//Iterating through vector of goti pointers
				{
					cout<<counter<<". ["<<goti_ptr->curr_coords.first<<"]["<<goti_ptr->curr_coords.second<<"]\n";
					++counter;
				}
				cout<<"\nRoll Output - ";
				for (auto &&outNum : dieNumbers)
				{
					cout<<outNum<<' ';
				}
				cout<<"\nEnter GotiNumber and dieNumber : ";
				getline(cin, inputStr);
				//! Fetching choice_num and enteredRoll
				{
					if( shortcutsMap.find(inputStr) != shortcutsMap.end() ){
						shortcutsMap.find(inputStr)->second("play");	//Calling using the function pointer
						return;
					}
					else{
						istringstream stream(inputStr);
						stream>>choice_num;
						if(choice_num == 0){
							if( lambda_isSIXinRoll() ){
								unlockGoti();
								dieNumbers.erase(std::find(dieNumbers.begin(), dieNumbers.end(), 6));
							}
							else{
								cout<<"Chhaka to aaye pehle, tab na goti niklegi yar!\n";
								customUtil::pause(1);
								continue;	//Wrong Input
							}
						}
						else{
							stream>>enteredRoll;
							if(
								std::find(dieNumbers.begin(), dieNumbers.end(), enteredRoll)!=dieNumbers.end()
										&& (choice_num <= counter)	//Verifies Entered values are valid
							){
								short moveRet = moveGoti(movingGotis[currentGotiColour][choice_num-1], enteredRoll);
								if( moveRet == -1){	//Failure
									cout<<"Invalid Move";
									customUtil::pause(1);
									continue;
								}else if( moveRet == 1){
									cout<<"\nShabaash!! Ek goti pung gayi :D\n";
									customUtil::pause(1);
									continue;
								}else{
									dieNumbers.erase(std::find(dieNumbers.begin(), dieNumbers.end(), enteredRoll));
								}
							}
							else continue;
						}
					}
				}
			}
			//! dieNumbers is again Empty
			lambda_next();
		}
	}
}

//! Source is the name of function from which it was called
	//TODO - Complete Logic for the options (Will implement after i have made a gui, or web version)
void game::settingsMenu(const string& source){
	auto termDimen = customUtil::getTerminalDimen();
	unsigned short choice = 11, inputNum = 0;
	std::string inputStr;

	do{
		_BoardPrinter::titleBar(termDimen.first);

		customUtil::place_v_center(termDimen.second - 2*(2 +11 +4));	//height of 2 taken by titleBar, 11 by Choices, and 4 by Input

		customUtil::align_text_center(termDimen.first, "1. Change 'GamePlay' Order (PlayerWise)"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "2. Change 'Number of gotis_per_user'"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "3. Which goti should 'Start' First?"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "4. Assign Colours to Players"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "5. Remove 'stops'"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "6. Change Title shown at Top"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "7. Change Player Names"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "8. Reset to Original Positions"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "9. Use different Random_Sequence for each (y/n)"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "10. Let's Cheat... ;D (Can remove opponents gotis)"); cout<<'\n';
		customUtil::align_text_center(termDimen.first, "[ 11. Alwida... ]"); cout<<"\n\n";

		customUtil::align_text_center(termDimen.first, "----------------------------");
		customUtil::align_text_center(termDimen.first - 2, "Enter Choice : "); cin>>choice;
	}while( choice <= 11 );

	switch(choice){
		case 1 : {
					_BoardPrinter::titleBar(termDimen.first);
					customUtil::place_v_center(termDimen.second - 2*(2 +1));	//height of 2 taken by titleBar, 11 by Choices, and 4 by Input
					
					cout<<"Type playerNumbers in order (for eg. \"1432\") : ";
					cin>>inputStr;
					// inputNum = std::stoi(customUtil::stripOff(inputStr, ' '));
					
					//TODO

					break;
				}

		case 2 : notYetImplementedScr(); break;
		case 3 : notYetImplementedScr(); break;
		case 4 : notYetImplementedScr(); break;
		case 5 : notYetImplementedScr(); break;
		case 6 : notYetImplementedScr(); break;
		case 7 : notYetImplementedScr(); break;
		case 8 : notYetImplementedScr(); break;
		case 9 : notYetImplementedScr(); break;
		case 10 : notYetImplementedScr(); break;
		case 11 : notYetImplementedScr(); break;
	}
}

void game::notYetImplementedScr(){

	_BoardPrinter::titleBar();

	customUtil::place_v_center();
	customUtil::align_text_center("This feature has not yet been implemented !");

}

game::game() : colourOrder({ColourLAAL, ColourNEELA, ColourPEELA, ColourHARA}){
	number_of_GameRuns = 0;
	min_boxlen = 1;
	goti_per_user = 4;

	// shortcutsMap["trial"] = &game::updateDisplay;
	// shortcutsMap.insert({"shortcuts", &game::settingsMenu });
	// _shortcutsMap.emplace({{"shortcuts", &game::settingsMenu}});

	unsigned short i, j;
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			board[i][j].coords = {i,j};
		}
	}

	//! Marking the LockRooms and the HomePath
	for (i = 0; i < 6; i++){
		for (j = 0; j < 6; j++){
			board[i][j].box_type = _boxUNUSABLE;
			board[7][j].box_type = _boxHOMEPATH;
			board[7][14-j].box_type = _boxHOMEPATH;
		}
		board[i][7].box_type = _boxHOMEPATH;
		board[14-i][7].box_type = _boxHOMEPATH;
	}
	board[0][7].box_type = board[14][7].box_type = _boxNORMAL;
	board[7][0].box_type = board[7][14].box_type = _boxNORMAL;
	for (i = 0; i < 6; i++)	for (j = 9; j < 15; j++)	board[i][j].box_type = _boxUNUSABLE;
	for (i = 9; i < 15; i++)	for (j = 0; j < 6; j++)	board[i][j].box_type = _boxUNUSABLE;
	for (i = 9; i < 15; i++)	for (j = 9; j < 15; j++)	board[i][j].box_type = _boxUNUSABLE;
	//! Marking the HomeArea
	for (i = 6; i < 9; i++)	for (j = 6; j < 9; j++)	board[i][j].box_type = _boxUNUSABLE;
	//! Marking the Stops
	for( auto& i : _ludo_coords.stops ){
		getBoardBox(i).box_type = _boxSTOP;
	}


	colourOrder = { ColourLAAL, ColourHARA, ColourPEELA, ColourNEELA };

	//! Storing references in order, starting from the starting positon of that colour, then anti-clockwise
	lockedPositions = {
		{ ColourLAAL, { ref(board[13][4]), ref(board[13][1]), ref(board[10][1]), ref(board[10][4]) } },
		{ ColourHARA, { ref(board[4][1]), ref(board[1][1]), ref(board[1][4]), ref(board[4][4]) } },
		{ ColourPEELA, { ref(board[1][10]), ref(board[1][13]), ref(board[4][13]), ref(board[4][10]) } },
		{ ColourNEELA, { ref(board[10][13]), ref(board[13][13]), ref(board[13][10]), ref(board[10][10]) } }
	};
	//! Marking the LockedPositions
	for (auto &lockArea : lockedPositions)
	{
		for( auto& lock : lockArea.second ) lock.get().box_type = _boxLOCK;
	}

	coloursMap = {
		{ ColourLAAL, Player1},
		{ ColourHARA, Player1},
		{ ColourPEELA, Player3},
		{ ColourNEELA, Player4}
	};

	currentPlayer = Player1;
	currentGotiColour = ColourLAAL;
	number_of_GameRuns = 0;

	for (auto &&stop : _ludo_coords.stops)
	{
		getBoardBox(stop).content = 'X';
	}

}

game::~game(){}
//GAME_CLASS_DEFINTIONS END