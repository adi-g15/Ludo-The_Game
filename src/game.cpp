#include "game.h"

#include <iostream>
#include <utility>
#include <algorithm>
#include <set>
#include <sstream>
#include <unordered_set>
#include "boardPrinter.hpp"
#include "exceptions.h"

using namespace std;

//GAME_CLASS_DEFINTIONS START
const _smartMoveData game::isMovePossible(std::shared_ptr<ludo_goti>& the_goti, unsigned int dist){
	_movePossible retVal;
	int moveProfit = 0;	//! To be used in SMART THINKER

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

		if( getBoardBox(updated_coords).box_type == _boxHOME_END && dist!=0 ){	//! Reached finished point, but move still incomplete
			retVal.isPossible = false;
			return {retVal, moveProfit};
		}
	}

	retVal.isPossible = true;
	retVal.finalCoords = updated_coords;

	return {retVal, moveProfit};
}

/*Return values - -1 : Can't Move, or FAILURE
				   0 : Normal
				   1 : Goti pung gayi
				   2 : Attacked (signifies signal to give extra diceRoll to player)*/
short game::moveGoti(std::shared_ptr<ludo_goti> the_goti, unsigned int dist){
	cout<<"[DEBUG] INSIDE MOVE"<<endl;

	auto [isPossible, finalCoords, finalDirection] = isMovePossible(the_goti, dist).getPossibility();
	if( isPossible == false ) return -1;

	the_goti->curr_direction = finalDirection;
	return moveGoti(the_goti, finalCoords);
}

short game::moveGoti(std::shared_ptr<ludo_goti> the_goti, std::pair<int, int> finalCoords){
	cout<<"[DEBUG] INSIDE MOVE"<<endl;
	if( !isValid(the_goti) && !isValid(finalCoords) ) return -1;

	if( getBoardBox(finalCoords).box_type == _boxHOME_END ){	//! ie. value of dist causes it to go beyond end position
		getBoardBox(the_goti->curr_coords).removeGoti( the_goti );	//Reaches here only if move possible

		cout<<"DEBUG HOME START"<<endl;

		//Removing from movingGotis
		auto iter = find( movingGotis[currentGotiColour].begin(), movingGotis[currentGotiColour].end(), the_goti );
		if( iter!=movingGotis[currentGotiColour].end() )
			movingGotis[currentGotiColour].erase(iter);
		else cout<<"Goti not found";

		auto& lockBox = getBoardBox( getEmptyLocks(currentGotiColour) );
			if(board[10][13].content!= ":D" && !board[10][13].content.empty() && board[10][13].content!="B" ){ cout<<"[DEBUG] VERY WRONG, SEE PREVIOUS BOARDS"<<endl; cin.get(); return false; }
			cout<<"GOT LOCKBOX at "<<lockBox.coords.first<<", "<<lockBox.coords.second<<endl;
			if(lockBox.coords == make_pair(0,0)){
				cout<<"Content of lockBoxes are : ";
				for(auto &i : lockedPositions[currentGotiColour]){
					cout<<'\''<<i.get().content<<"'["<<i.get().inBoxGotis.size()<<"], ";
				}
				cout<<"inBoxGotis in the box of the goti "<<getBoardBox(the_goti->curr_coords).inBoxGotis.size();
			}
			cin.get();
		lockBox.content = ":D";
		lockBox.box_type = _boxUNUSABLE;
			cout<<"DEBUG HOME MID and currentGotiColour="<<currentGotiColour<<" and size of numfinishedGotis="<<numfinishedGotis[currentGotiColour]<<endl;

		++numfinishedGotis[currentGotiColour];	//[DEBUG RESULT] This is the culprit
		cout<<"DEBUG HOME END"<<endl;

		return 1;
	}

	getBoardBox(the_goti->curr_coords).removeGoti( the_goti );	//Reaches here only if move possible
	the_goti->curr_coords = finalCoords;
DEBUG_START
	if( getBoardBox(the_goti->curr_coords).appendGoti( the_goti ) == 0){	//appendGoti() signalled an Attack needed
	cout<<"DEBUG MIDWAY"<<endl;
		attack( {}, the_goti );
DEBUG_END

		return 2;
	}
	return 0;
}

/*POSSIBLE BUG - For example for a random_goti_index the goti isn't movable, then that diceNumber is being discarded. */
bool game::autoMove(colours gotiColour){	//! Return values same as the moveGoti_function
	std::vector<unsigned short> dieNumbers;
	std::unordered_set<unsigned short> triedGotis_indices;	//! Stores the gotis that have been tried to move, FOR A SINGLE DIE_NUMBER
	_movePossible moveSuccessObj;
	bool wasSuccess = false;

	this->currentGotiColour = gotiColour;
	this->currentPlayer = currentPlayer;
	rolldie(dieNumbers);
	cout<<"Roll Output : ";
	for( auto& i : dieNumbers ) cout<<i<<' ';

	if( find(dieNumbers.begin(), dieNumbers.end(), 6) != dieNumbers.end()){
		if( random()%2 == 1 || movingGotis[currentGotiColour].empty() ){ //! 50% chance
			if( getEmptyLocks(currentGotiColour)==make_pair(0,0) ){
				unlockGoti();
				dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
			}
		}
	}
	else if( movingGotis[currentGotiColour].empty() ){
		cout<<"Move isn't possible!";
	}

	while( !dieNumbers.empty() && !movingGotis[currentGotiColour].empty() ){
		updateDisplay();

		triedGotis_indices.clear();
		auto currentRoll = dieNumbers[ random()%(dieNumbers.size()) ];
		auto random_goti_index = random()%(movingGotis[currentGotiColour].size()) ;
		if( currentRoll == 6 && random()%2==1 && (getEmptyLocks(currentGotiColour)==make_pair(0,0)) ){	//50% chance of robot using 6 to unlockGoti, when there's a six
			unlockGoti();
			updateDisplay();
			customUtil::pause(0.6);

			dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
			continue;
		}
		while( moveSuccessObj == false && (triedGotis_indices.size() != movingGotis[currentGotiColour].size()) ){	//! ie. gotis still remaining to try
	DEBUG_START //In b/w these, both SEG_FAULT and OUT_OF_INDEX HAPPENING
			triedGotis_indices.insert(random_goti_index);

			moveSuccessObj = isMovePossible(movingGotis[currentGotiColour][random_goti_index], currentRoll).getPossibility();
			if( moveSuccessObj == true ){
				wasSuccess = true;
				auto retVal = moveGoti(movingGotis[currentGotiColour][random_goti_index], moveSuccessObj.finalCoords);
				if( retVal == 2 ) rolldie(dieNumbers);	//Can't be -1, other ret values can simply be ignored

				customUtil::pause(0.7);
			}

	DEBUG_END
			if(!movingGotis[currentGotiColour].empty()) random_goti_index = random()%(movingGotis[currentGotiColour].size());	//Since, AFTER FINISH, MOVINGGOTIS maybe empty by this time, causing FLOATING POINT EXCEPTION
		}	//! Iterates through all gotis, until a goti is found that can be moved with provided currentRoll

	for(auto &i : dieNumbers) cout<<i<<' ';
	cout<<endl;
	if( find(dieNumbers.begin(), dieNumbers.end(), currentRoll) == dieNumbers.end() ) cout<<"TRUE"<<endl;
		dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), currentRoll));	//[DEBUG RESULT]THIS IS THE PROBLEM
	}

	return wasSuccess;
}

void game::attack(std::vector<colours> coloursToRemove, std::shared_ptr<ludo_goti> attacker){
	cout<<"[DEBUG] ATTACK 2"<<endl;
	if( coloursToRemove.empty() ){
		coloursToRemove.insert(coloursToRemove.begin(), { ColourNEELA, ColourHARA, ColourPEELA, ColourLAAL });
		coloursToRemove.erase( find(coloursToRemove.begin(),coloursToRemove.end(), currentGotiColour) );	//Using currentGotiPlayer and not attacker's colour, since only the currentPlayer should be able to attack
	}

	cout<<"\n[DEBUG] ATTACK 2 END 3 START"<<endl; //ERROR: [DEBUG]Coordinates of Goti are corrupt (0,0)

	ludo_box& currBox = getBoardBox(attacker->curr_coords);

	for( unsigned short i=0; i < currBox.inBoxGotis.size(); ++i ){	//Caution - May be a probable cause for iterator invalidation
	cout<<"[DEBUG] ATTACK inBoxGotis.size()="<<currBox.inBoxGotis.size()<<endl;
		if( std::find(coloursToRemove.begin(), coloursToRemove.end(), currBox.inBoxGotis[i]->gotiColour) != coloursToRemove.end() ){	//! emulating "if gotiColour in colourToRemove"
			lockGoti(currBox.inBoxGotis[i]);
			--i;
		}
	}
	cout<<"[DEBUG] ATTACK 3 END"<<endl;
}

bool game::isValid(const std::pair<int,int>& coords) const{
	if( coords.first >=0 && coords.first < 15 && coords.second >=0 && coords.second < 15 ){
		return true;
	}

	cout<<"\n\n[DEBUG] - iNVALID COORDS, NEED TO DEBUG"<<endl;
	cin.get();
	return false;
}

bool game::isValid(const shared_ptr<ludo_goti>& goti) const{
	if( goti->gotiColour == currentGotiColour && isValid(goti->curr_coords)){
		return true;
	}
	cerr<<"GOTI COLOUR DOESN'T RELATE TO CURRENT_PLAYER"<<endl;
	cin.get();	//DEBUG
	return false;
}


ludo_box& game::getBoardBox(const std::pair<int,int>& coords){
	if( isValid(coords) )
		return board[ coords.first ][ coords.second ];
	else throw exception();
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

bool game::lockGoti(std::shared_ptr<ludo_goti> goti_to_lock){	//wrong gotiColour goti called to be removed, currentPlayer was different
	if( !isValid(goti_to_lock->curr_coords) ) return false;

	goti_to_lock->curr_direction = NO_TURN;

	getBoardBox(goti_to_lock->curr_coords).removeGoti(goti_to_lock);

	auto&& emptyBox = std::find_if(lockedPositions[goti_to_lock->gotiColour].begin(), lockedPositions[goti_to_lock->gotiColour].end(), [](std::reference_wrapper<ludo_box>& box){
		return box.get().isEmpty();
	});
	cout<<"[DEBUG] LOCK GOTI START and got real_lockBox = "<<emptyBox->get().coords.first<<", "<<emptyBox->get().coords.second<<endl;	

	emptyBox->get().appendGoti(goti_to_lock);
		//! coords of goti to be edited by appendGoti
cout<<"[DEBUG] LOCK GOTI END"<<endl;	

	return true;
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
	customUtil::align_text_center(tmpDimen.first, "Enter names of the Players (Leave empty if not playing, or type \"ROBOT\") : ");
	
	string playerName;
	colours colour;
	auto p=Player1;
	auto numRobots = 0;

	colour = colourOrder[0];	
	
	while(p <= Player4){
		
		customUtil::place_center(tmpDimen.first, string("Player") + customUtil::intTostring(p) + string(" - "));		

		playerName = "ROBOT";
		//UNCOMMENT AFTER DEBUG
		// getline(cin, playerName, '\n');
		playerName = customUtil::trimString(playerName);
		if( playerName != ""){
			if( shortcutsMap.find(playerName) != shortcutsMap.end() ){
				shortcutsMap.find(playerName)->second("takeIntro");	//Calling using the function pointer
			}
			else if( playerName.compare("ROBOT")==0 || playerName.compare("Robot")==0 || playerName.compare("robot")==0 ){
				++numRobots;
				activePlayerMap.insert({{ p, { "ROBOT " + customUtil::intTostring(numRobots), colour} }});
				robotPlayers.insert(p);
			}else{
				activePlayerMap.insert({{ p, { customUtil::trimString(playerName), colour} }});
			}
		}

		p = player(p+1);
		colour = colours(colour+1);
	}

	//Setting the board, on basis of given players
	for (auto &&player : activePlayerMap){
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

bool game::gameisFinished(){
	for( auto &p : activePlayerMap ){
		if( numfinishedGotis[ p.second.second ] != 4 ) return false;
	}
	return true;
}

bool game::isPlayerPlaying(player p){
	return activePlayerMap.find(p) != activePlayerMap.end();
}

void game::rolldie(vector<unsigned short>& Vec){
	std::vector<unsigned short> tmpVec;

	// Vec.push_back(6); Vec.push_back(6); Vec.push_back(4); return;	//DEBUG - REMOVE THESE LINE LATER

	unsigned short dieNum = Die::dist[ random()%4 ](Die::mt[ random()%4 ] );
	try{
		if( dieNum!=6 ){	//To prevent cases like, dieNumbers = {4, 6} (Think! It IS possible in the do-while)
			Vec.push_back(dieNum);
			return;
		}
		do{
			tmpVec.push_back(dieNum);
			dieNum = Die::dist[ random()%4 ](Die::mt[ random()%4 ] );
		}while(dieNum == 6);
		tmpVec.push_back(dieNum);	//To insert the last left non-6
	}
	catch(...){	//Catch std::bad_alloc here
		tmpVec = {6,6};	//giving away two sixes, since user already got too many to cause an std::bad_alloc
	}

	//! Main logic is above only, below are some cleansing and optimisations
	std::vector<size_t> sixPos;	//Temporary set, used in 'Cleaning cases of 3 sixes'
	for (size_t i = 0; i < tmpVec.size(); ++i)
	{
		if( tmpVec[i] == 6 ) sixPos.push_back(i);
	}
	while(sixPos.size() > 3){	//ie. more than 3 sixes existing
		tmpVec.erase( tmpVec.begin() + sixPos[sixPos.size()-1] );	//Removing from end positions
		tmpVec.erase( tmpVec.begin() + sixPos[sixPos.size()-2] );
		tmpVec.erase( tmpVec.begin() + sixPos[sixPos.size()-3] );
		sixPos.erase(sixPos.begin() + sixPos.size()-3, sixPos.end());
	}

	Vec.insert(Vec.end(), tmpVec.begin(), tmpVec.end());
}

void game::updateDisplay(){
	unsigned int boxlen = 0;
	std::pair<int,int> tmpDimen(0,0);

	do{	
		tmpDimen = customUtil::getTerminalDimen();
		boxlen = ( 2*min(tmpDimen.first,tmpDimen.second) - 32 )/15;

	}while( min(tmpDimen.first,tmpDimen.second) < 32 );

	_BoardPrinter::titleBar(tmpDimen.first);

    customUtil::place_center(tmpDimen.first - 15*(boxlen+1) +3 -4 );
	cout<<"  ";
	for (size_t i = 0; i < 10; i++){
		for( auto j=boxlen; j-->0; )	//NOTE - '-->' is a combination of the 2 operators '--' and '>' ;D
			cout<<' ';
		cout<<i;
	}
	for (size_t i = 10; i < 15; i++){
		for( auto j=boxlen-1; j-->0; )	//NOTE - '-->' is a combination of the 2 operators '--' and '>' ;D
			cout<<' ';
		cout<<i;
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
	for (auto const &lockedBox : lockedPositions[gotiColour])
	{
		if(lockedBox.get().isEmpty() && lockedBox.get().box_type == _boxLOCK){
			return lockedBox.get().coords;
		}
	}
	return {0,0};	//ie. no locked gotis of this colour
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

/*[DONE]TODO - When there's only a single goti, and no 6, move the goti automatically*/
/*TODO - [For functionality for directly entering dieNumber if only 1 'movable' goti available] Also inside when loop for checking if choice_num is correct, add another condition, to check if there is only one goti available to move(not matters whether 6inRoll or not), if thats true, and the choice_num is in dieNumbers, considering enteredRoll=choice_num and choice_num=1*/
void game::play(bool boolVal){
	if( !boolVal ) return;

	currentGotiColour = colourOrder[0];
	currentPlayer = coloursMap[currentGotiColour];

	std::vector<unsigned short> dieNumbers;

		//Lambda Defintions
	auto lambda_next = [&](){
		std::vector<player> tmpVec;
		for (auto &&i : activePlayerMap)
		{
			tmpVec.push_back(i.first);
		}
		size_t pos = std::find(tmpVec.begin(), tmpVec.end(), currentPlayer) - tmpVec.begin();
		do{
			if( pos+1 < tmpVec.size() )	++pos;
			else	pos=0;
			currentPlayer = tmpVec[pos];

			currentGotiColour = activePlayerMap[currentPlayer].second;			
			cout<<"[DEBUG] Still looping, pos="<<pos<<endl;

		}while( this->numfinishedGotis[currentGotiColour] == this->goti_per_user );
	
		dieNumbers.clear();
	};
	auto lambda_isSIXinRoll = [&](){
		for (auto &&i : dieNumbers)
		{
			if(i == 6)
				return true;
		}
		return false;
	};
		//Lambda Defintions

	while ( ! this->gameisFinished() ){	//The gameplay loop		
		unsigned short enteredRoll, choice_num;
		bool isRobot = robotPlayers.find(currentPlayer) != robotPlayers.end();
		string inputStr;	//! Note - To use shortcuts like ":settings", ":rules, and to have 'variable number of inputs'
		updateDisplay();
		if( isRobot ){	//ie. It is a Robot Player
			autoMove(currentGotiColour);
			lambda_next();
			continue;
		}

		cout<<"\nPress Enter to roll the die...";
		cin.get();
		rolldie(dieNumbers);
		cout<<"\nRoll Output - ";
		for (auto &&outNum : dieNumbers)
		{
			cout<<outNum<<' ';
		}

			//This whole if-else nested block is for "AUTOMATIC DECISIONS", eg. moving a single goti possible to move
		//Automating Condition #1
		if( movingGotis[currentGotiColour].empty()){
			if( !lambda_isSIXinRoll() ){	//No movable goti, so continue to next player
				cout<<"\nNo movable goti, for the moves"<<endl;
				customUtil::pause(1);

				lambda_next();
				continue;
			}
			else{	//No movable gotis currently, hence automatically open one, since 6 is available
				customUtil::pause(0.4);
				unlockGoti();
				updateDisplay();
				cout<<"Goti unlocked!"<<endl;
				customUtil::pause(0.8);
				dieNumbers.erase(find(dieNumbers.begin(), dieNumbers.end(), 6));
			}
		}

			//! Actual logic to move each player is this nested while loop
		while( !dieNumbers.empty() ){

			updateDisplay();
			unsigned short counter = 1;
			cout<<"\nChose from these gotis : ";
			if( lambda_isSIXinRoll() ) cout<<"\n0. Unlock New Goti (just type 0)\n\n";
			for (auto &&goti_ptr : movingGotis[currentGotiColour]){
				cout<<counter<<". ["<<goti_ptr->curr_coords.first<<"]["<<goti_ptr->curr_coords.second<<"]\n"; 
				++counter;
			}
			cout<<"\nRoll Output - ";
			for (auto &&outNum : dieNumbers)
			{
				cout<<outNum<<' ';
			}

				//The Automating Condition #2
			auto continue_OuterWhile = false;	//To break out of outer loop, from inside an inner loop, BCZ coninue INSIDE THIS WILL NOT SHIFT TO THE NEXT PLAYER
			while( movingGotis[currentGotiColour].size() == 1 && dieNumbers.size() == 1 ){
				auto [isPossible, finalCoords, finalDirection] = isMovePossible(movingGotis[currentGotiColour][0], dieNumbers[0]).getPossibility();
				if( !isPossible ){
					cout<<"Move not possible"<<endl;
					customUtil::pause(0.6);

					lambda_next();
					continue;
				}
				else{ 			//goti has been moved
					movingGotis[currentGotiColour][0]->curr_direction = finalDirection;
					auto moveRet = moveGoti( movingGotis[currentGotiColour][0], finalCoords);
					if( moveRet == 1 ) cout<<"Shabaash!! Ek goti pung gayi :D"<<endl;
					else if(moveRet == 2){	//Attack happened
						cout<<"\nPress enter to roll the die once more...";
						cin.get();
						rolldie(dieNumbers);
					}

					updateDisplay();
					customUtil::pause(1);
					dieNumbers.erase(dieNumbers.begin());	//! Removing the used goti number (which is actually dieNumbers[0])
				}
				customUtil::pause(1);
				continue_OuterWhile = true;
			}
			if( continue_OuterWhile ) continue;

			cout<<"\nEnter Goti and dieNumber : ";
			do{
				getline(cin, inputStr);
			}while( customUtil::trimString(inputStr).empty() );

			//! This is just a simple block, for separating the process, of Fetching choice_num and enteredRoll
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
								cout<<"[UNLOCK_DEBUG] PASSING THIS"<<endl;
							unlockGoti();
								cout<<"[UNLOCK_DEBUG] PASSED"<<endl;
							dieNumbers.erase(std::find(dieNumbers.begin(), dieNumbers.end(), 6));
						}
						else{
							cout<<"Chhaka to aaye pehle, tab na goti niklegi yar!\n";
							customUtil::pause(1);
							continue;	//Wrong Input
						}
					}
					else{	//! Usual Case - No six, nor any cheat/shortcut used
						cout<<"[DEBUG] PASSING THIS"<<endl;
						stream>>enteredRoll;

						if(
							std::find(dieNumbers.begin(), dieNumbers.end(), enteredRoll)!=dieNumbers.end()
									&& (choice_num <= counter)	//Verifies Entered values are valid
						){
								cout<<"[DEBUG] INSIDE IF and movingGotis length for this colour is "<<movingGotis[currentGotiColour].size()<<endl;
							short moveRet = moveGoti(movingGotis[currentGotiColour][choice_num-1], enteredRoll);
								cout<<"[DEBUG] PASSED"<<endl;
							if( moveRet == -1){	//Failure
								cout<<"Invalid Move"<<endl;
								customUtil::pause(1);
							}
							else{ 				//goti has been moved
								if( moveRet == 1 ) cout<<"\nShabaash!! Ek goti pung gayi :D\n";
								else if(moveRet == 2){	//Attack happened
									cout<<"Achha khel rahe ho "<<activePlayerMap[currentPlayer].first
										<<"\nPress enter to roll the die once more...";
									cin.get();
									rolldie( dieNumbers );
								}
								dieNumbers.erase(std::find(dieNumbers.begin(), dieNumbers.end(), enteredRoll));	//! Removing the use goti number from the dieNumbers vector
								customUtil::pause(0.5);
							}
						}
					}
				}
			}
			updateDisplay();
		}
		//! dieNumbers is again Empty
		lambda_next();
	}
	if( gameisFinished() ) _BoardPrinter::finishedScreen();
}

//! Source is the name of function from which it was called
	//TODO - Complete Logic for the options (Will implement after i have made a gui, or web version)
void game::settingsMenu(const string& source){
	auto termDimen = customUtil::getTerminalDimen();
	unsigned short choice = 11;
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
	board[6][7].box_type = board[8][7].box_type = board[7][6].box_type = board[7][8].box_type = _boxHOME_END; 


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
//GAME_CLASS_DEFINTIONS END)

//BELOW ARE THE 'REMAINS' of the original Recursive version of random autoMove() I had thought...
//I am leaving them here, since i don't think there will be much point in commiting it, since it's going to be needed soon maybe... in a recursive THINKING ALGORITHMIC ROBOT player (Big name, no? ;D )
// bool game::autoMove(set<pair<unsigned short, unsigned short> >& triedCombinations, std::vector<unsigned short> dieNumbers){

// 	cout<<"DEBUG START 1"<<endl;
// 	unsigned probabilityOfUnlock = 50;	//in percentage, Change to change probability of robot using 6 for unlocking a goti
// 	if( find(dieNumbers.begin(), dieNumbers.end(), 6) != dieNumbers.end() && (random()%(100/probabilityOfUnlock))%2 == 1 ){
// 		//By default, a 50% chance of robots using 6, to unlock goti
	
// 	}

// 	if( dieNumbers.empty() )	return false;
// 	else if( triedCombinations.size() == (movingGotis[currentGotiColour].size())*(dieNumbers.size()) ){	//The condition to get out of reursion
// 		return false;
// 	}
// 	cout<<"DEBUG END 1 START 2"<<endl;

// 	unsigned short gotiIndex = random()%(movingGotis[currentGotiColour].size());	//! But, this is also a 'pseudo-random number', AND IT GIVES EXACTLY 'SAME SEQUENCE'(ie. nth printed number is exactly same every time) OF NUMBERS! eVEN IF IT COMPILED AGAIN AND AGAIN
// 	unsigned short currentMoveVal = dieNumbers[ random()%dieNumbers.size() ];

// 	if( std::find(triedCombinations.begin(), triedCombinations.end(), std::make_pair(gotiIndex, currentMoveVal)) != triedCombinations.end() ){
// 		return autoMove( triedCombinations, dieNumbers );	//Goti, and move combination already tried
// 	}
// 	cout<<"DEBUG END 2 START 3";
// 	// triedCombinations.insert( std::make_pair(gotiIndex, currentMoveVal) );	//QUESTION - WHY not acceptinjg to be valid call to insert?
// 	auto tmpCombination = {std::make_pair(gotiIndex, currentMoveVal)};
// 	triedCombinations.insert( tmpCombination.begin(), tmpCombination.end() );

// 	auto [isPossible, finalCoords] = isMovePossible(movingGotis[currentGotiColour][gotiIndex], currentMoveVal);

// 	if( isPossible ){
// 		auto retVal = moveGoti(movingGotis[currentGotiColour][gotiIndex], finalCoords);
// 		if( retVal == 2 ){	//ie. Attacked Someone Successfully
// 			auto coords = movingGotis[currentGotiColour][gotiIndex]->curr_coords;
// 			cout<<activePlayerMap[currentPlayer].first<<" removed Opponent at "<<coords.first<<", "<<coords.second<<endl;
// 			rolldie(dieNumbers);
// 		}

// 		dieNumbers.erase( find(dieNumbers.begin(), dieNumbers.end(), currentMoveVal) );
// 		return true;
// 	}
// 	cout<<"DEBUG END 2 START 3";
// 	return autoMove(triedCombinations, dieNumbers);	//Causes recursion
// }
