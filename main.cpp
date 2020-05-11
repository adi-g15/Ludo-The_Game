/*
Programmer - Aditya Gupta (Techy15)
Language - C++
Program - LudO - The Game
*/

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<functional>
#include<unordered_set>

// #include "util/header_includer.hpp"

#include<string>
#include<exception>

#include "util/header_includer.hpp"


using namespace std;

// class ludo_box;
// class ludo_goti;

// 	std::vector<std::vector<ludo_box>> board(15);
// 	std::unordered_set<player> activePlayers;	//Actually order is necessary, but that will be managed by 'to be' friend takeIntro()
// 	std::map<colours, player> Coloursmap;
// 	std::map<player, std::pair< std::string,std::pair<colours ,std::vector< std::reference_wrapper<ludo_goti> >> >> playerMap;
// /*  @guide
// 	PLAYER std::map
// 	======================
// 	PlayerID(enum)  ----> std::string PlayerName
// 						  	colour GotiColour
// 						  	std::vector of Gotis

// 	Similarly, a more complex Colourstd::map MAYBE implemented, but for now the 'players' and 'colours' enum correspond
// */

// template <class T1, class T2>
// class simpleTuple{
// protected:
//   T1 key;
//   T2 value;

// public:
//   void setVal(T1 K, T2 V);
//   const T1 getKey() const;
//   const T2 getVal() const;

//   void operator=(const simpleTuple<T1,T2>&);
//   bool operator==(const simpleTuple<T1,T2>&) const;
//   bool operator!=(const simpleTuple<T1,T2>&) const;

//   simpleTuple(T1 K, T2 V);

// };

// template<class T>
// class homoTuple : public simpleTuple<T,T>{
//   public:
//     typedef simpleTuple<T,T> super;

//     T&   operator[](unsigned int); //[NOTE] - Whenever const access is required, use the overloaded methods, else use the operator
//     void operator=(const simpleTuple<T,T>&);
//     bool operator==(const homoTuple<T>&) const;
//     bool operator==(const simpleTuple<T,T>&) const;
//     bool operator!=(const homoTuple<T>&) const;
//     bool operator!=(const simpleTuple<T,T>&) const;

//     homoTuple(T key, T value) : super(key,value){}
// };

// //##    TYPEDEF    ##
// typedef homoTuple<int> intTuple;
// typedef homoTuple<char> charTuple;
// typedef homoTuple<float> floatTuple;
// typedef homoTuple<double> doubleTuple;
// //##  / TYPEDEF    ##

// 	int goti_per_user = 4;
// 	int num_laalGoti = 0; 
// 	int num_hariGoti = 0; 
// 	int num_peeliGoti = 0; 
// 	int num_neeliGoti = 0; 

// 	void global_destructor(); //... iterator through whole board, and 'delete ludo_goti initialised with startGoti()'

// class coord_class{
// public:
// 	std::vector<intTuple> stops;
// 	std::map<intTuple, direction> outer_corners, inner_turns;
// 	std::map<intTuple, direction> home_turns;
// 	std::map<colours,intTuple> start_coords;

// 	coord_class();
// }publicCoords;

// class _BoardPrinter{ //@info Only for use by updateDisplay() function
//     void type1(int,int) const;
//     void type2(int,int) const;
//     void type3(int,int) const;
//     void type4(int,int) const;
//     void type5(int,int) const;
//     void type6(int,int) const;

//     friend void updateDisplay();
// }_boardprinterVar;

// inline intTuple get_initCoords(colours);
// bool dir_to_turn_ifCoord_in_CornerCoordsVec(const intTuple& curr_coords, const std::pair<intTuple, direction> &cornerCoord);
// void updateDisplay();
// void player_move(std::vector<int>&);
// void moveGoti(ludo_goti&, unsigned int);
// void startGoti(const colours);
// ludo_box& getBox(const ludo_goti&);	//May make this a 'private' function in a class
// void takeIntro();
// bool isPlayerPlaying(player);
// int rolldie();


// intTuple getTerminalDimen();
// inline bool align_text_center(int max_length,const std::string&);
// inline std::string trimString(const std::string&);   //@returns trimmed std::string, but doesnt affect original std::string



// typedef intTuple coordinates;


// class ludo_goti{
// protected:
// 	colours gotiColour;
// 	direction curr_direction;
// 	intTuple curr_coords;	

// public:
// 	const intTuple getCoords() const;
//     direction get_curr_direction() const;
//     colours get_gotiColour(void) const;

// 	friend void moveGoti(ludo_goti&, unsigned int);

// 	ludo_goti();
// };

// class laalGoti : public ludo_goti{
// 	laalGoti();
// };
// class hariGoti : public ludo_goti{
// 	hariGoti();
// };
// class peeliGoti : public ludo_goti{
// 	peeliGoti();
// };
// class neeliGoti : public ludo_goti{
// 	neeliGoti();
// };


// class ludo_box{
// 	std::vector<std::reference_wrapper<ludo_goti>> inBoxGotis;
// 	std::string content;

// 	//Corner specification
// 	bool isOuterCorner;
// 	bool isRectCorner;
// 	direction turnDir;

// public:
// 	std::reference_wrapper<ludo_goti> getGoti(colours);
//     bool removeGoti(colours);
// 	bool appendGoti(ludo_goti&);
// 	std::string get_box_content();

// 	ludo_box();
// };

int main(int argc, char const *argv[]) {

	takeIntro();	//Initialises the PlayerMap

	//Initialising Board
	board.reserve(15);
	for (size_t i = 0; i < 15; i++) {
		board.emplace_back(vector<ludo_box>());
		for (size_t j = 0; j < 15; j++) {
			board[i].emplace_back(ludo_box());
		}
	}

	player currentPlayer = *activePlayers.begin();
	colours currentGotiColour = playerMap[currentPlayer].second.first;
	vector<int> dieNumber;

	//GamePlay Starts
	takeIntro();
	while (true){

		currentGotiColour = playerMap[currentPlayer].second.first;
		
		updateDisplay();
		cin.get();
		dieNumber.push_back(rolldie());
		cout<<"\nRoll Output - ";
		for (auto &&i : dieNumber)
		{
			cout<<i<<' ';
		}
		
		if(dieNumber.back() == 6){
			continue;
		}
		startGoti(currentGotiColour);
		moveGoti(board[6][1].getGoti(ColourLAAL), 5);

		// player_move(dieNumber);	//then calls moveGoti()

		
		///Update current Player here
		
	}

	return 0;
}


// //ONLY HEADER IMPLEMENTATIONS BELOW [PLEASE]
// coord_class::coord_class(){
		
// 		outer_corners.insert({
// 				make_pair(intTuple(0,6), NORTH),
// 				make_pair(intTuple(0,8), EAST),
// 				make_pair(intTuple(14,6), WEST),
// 				make_pair(intTuple(14,8), SOUTH),
// 				make_pair(intTuple(6,0), NORTH),
// 				make_pair(intTuple(8,0), WEST),
// 				make_pair(intTuple(6,14), EAST),
// 				make_pair(intTuple(8,14), SOUTH)
// 		});
// 		inner_turns.insert({
// 				make_pair(intTuple(6,6), WEST),
// 				make_pair(intTuple(6,8), NORTH),
// 				make_pair(intTuple(8,6), SOUTH),
// 				make_pair(intTuple(8,8), EAST)
// 		});

// 		stops.insert( stops.begin(), {
			
// 		});

// 		start_coords.insert({			
// 			make_pair(ColourLAAL, intTuple(6,1)),
// 			make_pair(ColourHARA, intTuple(1,8)),
// 			make_pair(ColourNEELA, intTuple(8,13)),
// 			make_pair(ColourPEELA, intTuple(13,6)),
// 		});
// }

// inline intTuple get_initCoords(colours gotiColour){
// 	for(auto &&i : publicCoords.start_coords){
// 		if( i.first == gotiColour )
// 			return i.second;
// 	}
	
// 	return intTuple(0,0);
// }

// void updateDisplay(){
// 	int boxlen = 0;
// 	intTuple tmpDimen(0,0);

// 	do{	
// 		tmpDimen = getTerminalDimen();
// 		boxlen = (min(tmpDimen[0],tmpDimen[1]) - 16)/15;

// 	}while( min(tmpDimen[0],tmpDimen[1]) < 16 || max(tmpDimen[0],tmpDimen[1]) < 31 );

// 	for (size_t i = 0; i <(boxlen+1)*15 + 1; i++)
// 	{
// 		cout<<'-';
// 	}
	
// 	_boardprinterVar.type1(boxlen,0);
// 	_boardprinterVar.type2(boxlen,1);
// 	_boardprinterVar.type3(boxlen,2);
// 	_boardprinterVar.type1(boxlen,3);
// 	_boardprinterVar.type2(boxlen,4);
// 	_boardprinterVar.type4(boxlen,5);

// 	_boardprinterVar.type5(boxlen,6);
// 	_boardprinterVar.type6(boxlen,7);
// 	_boardprinterVar.type5(boxlen,8);

// 	_boardprinterVar.type4(boxlen,9);
// 	_boardprinterVar.type2(boxlen,10);
// 	_boardprinterVar.type1(boxlen,11);
// 	_boardprinterVar.type3(boxlen,12);
// 	_boardprinterVar.type2(boxlen,13);
// 	_boardprinterVar.type1(boxlen,14);

// 	cout<<"\n\nCONSOLE\n";
// 	for (size_t i = 0; i < tmpDimen[1]; i++)
// 	{
// 		cout<<'-';
// 	}
// 	cout<<"\nPress Enter to roll the die...";

// }

// void startGoti(colours gotiColour){
// 	if(num_laalGoti >= goti_per_user || num_hariGoti >= goti_per_user || num_peeliGoti >= goti_per_user || num_neeliGoti >= goti_per_user){
// 		cerr<<"Goti request makes the number of ludo_goti of RedColour more than "<<goti_per_user<<endl;
// 		return;
// 	}
// 	else if (gotiColour == UnknownColour)
// 	{
// 		cerr<<"startGoti() request made for Unknown Colour"<<endl;
// 		return;
// 	}	
// 	else{
// 		board[publicCoords.start_coords[gotiColour][0]][publicCoords.start_coords[gotiColour][1]];
// 	}
// }

// ludo_box& getBox(const ludo_goti& inGoti){
// 	if(inGoti.getCoords().getKey() < 15 || inGoti.getCoords().getVal() < 15 || inGoti.getCoords().getKey() >= 0 || inGoti.getCoords().getVal() >= 0){
// 		throw OutOfBoundException("Board");
// 	}
// 	return board[inGoti.getCoords().getKey()][inGoti.getCoords().getVal()];
// }

// direction dir_to_turn_ifCoord_in_CornerCoordsVec(const intTuple& curr_coords, const std::map<intTuple, direction> &corner_vec){
// 	for ( auto corner : corner_vec ){
// 		if(curr_coords == corner.first){
// 			return corner.second;
// 		}
// 	}
// 	return NO_TURN;
// }

// void moveGoti(ludo_goti& the_goti, unsigned int dist){
// 	int x_increment=0, y_increment=0;	//[NOTE] - Can store these in the ludo_box class itself, besides the corner specification
// 	direction turnDirection;

// 	board[the_goti.curr_coords.getKey()][the_goti.curr_coords.getVal()].removeGoti(the_goti.gotiColour);

// 	while(dist--){

// 		x_increment = y_increment = 0;

// 		turnDirection = dir_to_turn_ifCoord_in_CornerCoordsVec(the_goti.getCoords(), publicCoords.inner_turns);
// 		if(turnDirection != NO_TURN){
// 			if(turnDirection == NORTH){
// 				x_increment = 2;
// 				y_increment = 1;
// 			}
// 			else if(turnDirection == EAST){
// 				x_increment = 1;
// 				y_increment = -2;
// 			}
// 			else if(turnDirection == WEST){
// 				x_increment = -1;
// 				y_increment = 2;
// 			}
// 			else if(turnDirection == SOUTH){
// 				x_increment = -2;
// 				y_increment = -1;
// 			}
// 		}
// 		else{

// 			turnDirection = dir_to_turn_ifCoord_in_CornerCoordsVec(the_goti.getCoords(), publicCoords.outer_corners);
// 			if(turnDirection != NO_TURN){
// 				if(turnDirection == NORTH){
// 					x_increment = 0;
// 					y_increment = 1;
// 				}
// 				else if(turnDirection == EAST){
// 					x_increment = 1;
// 					y_increment = 0;
// 				}
// 				else if(turnDirection == WEST){
// 					x_increment = -1;
// 					y_increment = 0;
// 				}
// 				else if(turnDirection == SOUTH){
// 					x_increment = 0;
// 					y_increment = -1;
// 				}
// 			}

// 			else{
// 				if(the_goti.get_curr_direction() == NORTH){
// 					x_increment = 0;
// 					y_increment = 1;
// 				}
// 				else if(the_goti.get_curr_direction() == EAST){
// 					x_increment = 1;
// 					y_increment = 0;
// 				}
// 				else if(the_goti.get_curr_direction() == WEST){
// 					x_increment = -1;
// 					y_increment = 0;
// 				}
// 				else if(the_goti.get_curr_direction() == SOUTH){
// 					x_increment = 0;
// 					y_increment = -1;
// 				}
// 			}
// 		}
// 	}

// 	board[the_goti.curr_coords[0]][the_goti.curr_coords[1]].appendGoti(the_goti);
// 	updateDisplay();

// }

// void takeIntro(){
// 	intTuple tmpDimen(0,0);

// 	do{	
// 		tmpDimen = getTerminalDimen();

// 	}while( min(tmpDimen[0],tmpDimen[1]) < 16 || max(tmpDimen[0],tmpDimen[1]) < 31 );

// 	align_text_center(tmpDimen[1], string("Welcome to \"Ludo-The Game\""));
// 	cout<<endl;
// 	for (size_t i = 0; i < tmpDimen[1]; i++)
// 	{
// 		cout<<'=';
// 	}
	
// 	string playerName;
// 	player i=Player1;
// 	cout<<"\n\nEnter names of the Players (0 if not playing) : \n";
	
// 	while(i<=Player4){
// 		cout<<"Player"<<i<<" - ";
		
// 		getline(cin, playerName);		
// 		if(trimString(playerName) != "0"){
// 			playerMap.insert(make_pair(i,make_pair(playerName,make_pair((colours)i,vector<reference_wrapper<ludo_goti>>()))));
// 			activePlayers.insert(i);
// 		}
// 	}

// 	cout<<endl<<"\nPress Enter to go to the board...";
// 	cin.get();
// }

// bool isPlayerPlaying(player p){
// 	return playerMap.find(p) != playerMap.end();
// }

// //  BOARD_PRINTER_METHODS
// void _BoardPrinter::type1(int boxlen, int nrow) const{
// 	string content;

// 	//Actual-Row Start
// 	for(size_t j=0; j<boxlen; ++j){
// 		cout<<'|';
// 		for (size_t i = 0; i < (boxlen+1)*6 - 1; i++)
// 			cout<<'\\';

// 		cout<<'|';

// 		for (size_t i = 0; i < 3; i++){
// 			align_text_center(boxlen, board[nrow][6+i].get_box_content());
// 			cout<<'|';
// 		}

// 		for (size_t i = 0; i < (boxlen+1)*6 - 1; i++)	cout<<'\\';
// 		cout<<"|\n";
// 	}
// 	//Actual-Row End

// 	//Inter-Row line Start
// 	cout<<'|';
// 	for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
// 	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
// 	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
// 	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
// 	for (size_t i = 0; i < boxlen; i++) cout<<'\\';
// 	cout<<'|';

// 	for (size_t i = 0; i < (boxlen+1)*3-1; i++) cout<<'-';

// 	cout<<'|';
// 	for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
// 	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
// 	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
// 	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
// 	for (size_t i = 0; i < boxlen; i++) cout<<'\\';
// 	cout<<"|\n";
// 	//Inter-Row Line end
// }

// void _BoardPrinter::type2(int boxlen, int nrow) const{
// 	//Actual-Row Start
// 	for(size_t j=0; j<boxlen; ++j){
// 		cout<<'|';
// 		for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
// 		for (size_t i = 0; i < boxlen+2; i++) cout<<' ';
// 		for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
// 		for (size_t i = 0; i < boxlen+2; i++) cout<<' ';
// 		for (size_t i = 0; i < boxlen; i++) cout<<'\\';
// 		cout<<'|';


// 		for (size_t i = 0; i < 3; i++){
// 			align_text_center(boxlen, board[nrow][6+i].get_box_content());
// 			cout<<'|';
// 		}

// 		for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
// 		for (size_t i = 0; i < boxlen+2; i++) cout<<' ';
// 		for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
// 		for (size_t i = 0; i < boxlen+2; i++) cout<<' ';
// 		for (size_t i = 0; i < boxlen; i++) cout<<'\\';
// 		cout<<"\n";
// 	}
// 	//Actual-Row End

// 	//Inter-Row line Start
// 	cout<<'|';
// 	for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
// 	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
// 	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
// 	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
// 	for (size_t i = 0; i < boxlen; i++) cout<<'\\';
// 	cout<<'|';

// 	for (size_t i = 0; i < (boxlen+1)*3-1; i++) cout<<'-';

// 	cout<<'|';
// 	for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
// 	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
// 	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
// 	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
// 	for (size_t i = 0; i < boxlen; i++) cout<<'\\';
// 	cout<<"|\n";
// 	//Inter-Row Line end

// }

// void _BoardPrinter::type3(int boxlen, int nrow) const{
// 	//Actual-Row Start
// 	for(size_t j=0; j<boxlen; ++j){
// 		cout<<'|';
// 		for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
// 		cout<<'|';


// 		for (size_t i = 0; i < 3; i++){
// 			align_text_center(boxlen, board[nrow][6+i].get_box_content());
// 			cout<<'|';
// 		}

// 		for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
// 		cout<<"|\n";
// 	}
// 	//Actual-Row End

// 	//Inter-Row line Start
// 	cout<<'|';
// 	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
// 	cout<<'|';

// 	for (size_t i = 0; i < (boxlen+1)*3-1; i++) cout<<'-';

// 	cout<<'|';
// 	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
// 	cout<<"|\n";
// 	//Inter-Row Line end
// }

// void _BoardPrinter::type4(int boxlen, int nrow) const{
// 	//Actual-Row Start
// 	for(size_t j=0; j<boxlen; ++j){
// 		cout<<'|';
// 		for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
// 		cout<<'|';


// 		for (size_t i = 0; i < 3; i++){
// 			align_text_center(boxlen, board[nrow][6+i].get_box_content());
// 			cout<<'|';
// 		}

// 		for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
// 		cout<<"|\n";
// 	}
// 	//Actual-Row End

// 	//Inter-Row line Start
// 	cout<<'|';
// 	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'-';

// 	for (size_t i = 0; i < (boxlen+1)*3-1+2; i++) cout<<'-';

// 	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'-';
// 	cout<<"|\n";
// 	//Inter-Row Line end
// }

// void _BoardPrinter::type5(int boxlen, int nrow) const{
// 	//Actual-Row Start
// 	for(size_t j=0; j<boxlen; ++j){
// 		cout<<'|';
// 		for (size_t i = 0; i < 6; i++){
// 			align_text_center(boxlen, board[nrow][6+i].get_box_content());
// 			cout<<'|';
// 		}

// 		for (size_t i = 0; i < (boxlen+1)*3 -1; i++) cout<<' ';	
// 		cout<<'|';

// 		for (size_t i = 0; i < 6; i++){
// 			align_text_center(boxlen, board[nrow][6+i].get_box_content());
// 			cout<<'|';
// 		}
// 	}
// 	//Actual-Row End

// 	//Inter-Row line Start
// 	cout<<'|';
// 	for (size_t i = 0; i < (boxlen+1)*6-1; i++) cout<<'-';
// 	cout<<'|';

// 	for (size_t i = 0; i < (boxlen+1)*3 -1; i++) cout<<' ';	
// 	cout<<'|';

// 	for (size_t i = 0; i < (boxlen+1)*6-1; i++) cout<<'-';
// 	cout<<'|';
// 	//Inter-Row Line end
// }

// void _BoardPrinter::type6(int boxlen, int nrow) const{
// 	//Actual-Row Start
// 	for(size_t j=0; j<boxlen; ++j){
// 		cout<<'|';
// 			align_text_center(boxlen, board[nrow][6].get_box_content());
// 			cout<<'|';
// 		for (size_t i = 0; i < 5; i++){
// 			align_text_center(boxlen, board[nrow][7+i].get_box_content());
// 			cout<<' ';
// 		}

// 		for (size_t i = 0; i < (boxlen+1)*3 -1; i++) cout<<' ';	
// 		cout<<'|';

// 		for (size_t i = 0; i < 4; i++){
// 			align_text_center(boxlen, board[nrow][6+i].get_box_content());
// 			cout<<' ';
// 		}
// 		for (size_t i = 0; i < 2; i++){
// 			align_text_center(boxlen, board[nrow][6+i].get_box_content());
// 			cout<<'|';
// 		}

// 	}
// 	//Actual-Row End
// 	//Inter-Row line Start
// 	cout<<'|';
// 	for (size_t i = 0; i < (boxlen+1)*6-1; i++) cout<<'-';
// 	cout<<'|';

// 	for (size_t i = 0; i < (boxlen+1)*3 -1; i++) cout<<' ';	
// 	cout<<'|';

// 	for (size_t i = 0; i < (boxlen+1)*6-1; i++) cout<<'-';
// 	cout<<'|';
// 	//Inter-Row Line end	
// }

// int rolldie(){
// 	bool *b= new bool;
// 	uintptr_t retval = reinterpret_cast<uintptr_t>(b);
// 	delete b;

// 	return (retval%6)+1;
// }

// void player_move(std::vector<int> &dieOutputs){

// }

// #ifdef __linux__
//     #include<sys/ioctl.h>
//     #include<unistd.h>
// #elif __MINGW32
//     #include<windows.h>
// #elif __CYGWIN32
//     #include<windows.h>
// #endif
// #ifndef _GLIBCXX_ALGORITHM
//     #include<algorithm>
// #endif

// intTuple getTerminalDimen(){
//   intTuple retTuple(0,0);
//   #ifdef __linux__
//     winsize windowsSize;
//     ioctl (STDOUT_FILENO, TIOCGWINSZ, &windowsSize);
//       retTuple[0] = windowsSize.ws_row;
//       retTuple[1] = windowsSize.ws_col;

//   #else //Windows API
//     {
//       CONSOLE_SCREEN_BUFFER_INFO csbi;
//       GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &csbi );
//         retTuple[0] = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
//         retTuple[1] = csbi.srWindow.Right - csbi.srWindow.Left + 1;
//     }

//   #endif

//   return retTuple;
// }

// inline bool align_text_center(int max_length,const std::string& str){    //@returns bool indicating, if max_len is enough or not
//     if(max_length < str.size()){
//         return false;
//     }
//     else
//     {
//         for (size_t i = 0; i < (max_length-str.size())/2; i++)  std::cout<<' ';
//         std::cout<<str;
//         for (size_t i = 0; i < (max_length-str.size()) - (max_length-str.size())/2; i++)  std::cout<<' ';
//     }
    
// }

// inline std::string trimString(const std::string &s){
//     std::string str_out(s);
//     str_out.erase(std::find_if(str_out.rbegin(),str_out.rend(),[](char ch){
//         return ! isspace(ch);
//     }).base(), str_out.end());

//     str_out.erase(std::find_if(str_out.begin(),str_out.end(),[](char ch){
//         return ! isspace(ch);
//     }), str_out.rend().base());
    
// }


// const intTuple ludo_goti::getCoords() const{
//     if(curr_coords.getKey() == 0 && curr_coords.getVal()==0){
//         cerr<<"Coordinates of Goti are corrupt (0,0)\n"<<endl;
//     }
//     return curr_coords;
// }
// direction ludo_goti::get_curr_direction() const{
//     return curr_direction;
// }

// ludo_goti::ludo_goti() : curr_coords(0,0) {
//     gotiColour = UnknownColour;
// }

// colours ludo_goti::get_gotiColour(void) const{
//     return gotiColour;
// }

// laalGoti::laalGoti(){
//     this->gotiColour = ColourLAAL;
//     this->curr_coords = get_initCoords(ColourNEELA);
// }

// hariGoti::hariGoti(){
//     this->gotiColour = ColourHARA;
//     this->curr_coords = get_initCoords(ColourNEELA);
// }

// peeliGoti::peeliGoti(){
//     this->gotiColour = ColourPEELA;
//     this->curr_coords = get_initCoords(ColourNEELA);
// }

// neeliGoti::neeliGoti(){
//     this->gotiColour = ColourNEELA;
//     this->curr_coords = get_initCoords(ColourNEELA);
// }



// ludo_box::ludo_box(){
//     content = " ";
// }

// reference_wrapper<ludo_goti> ludo_box::getGoti(colours gotiColour){
// 	for (auto &&i : inBoxGotis)
// 	{
// 		if(i.get().get_gotiColour() == gotiColour){
// 			return i;
// 		}
// 	}		

// 	throw GotiNotAvailableException(gotiColour);
// }

// bool ludo_box::removeGoti(colours gotiColour){
// 	vector<reference_wrapper<ludo_goti>>::const_iterator i = inBoxGotis.begin();
// 	for ( ; i < inBoxGotis.end(); ++i)
// 	{
// 		if( i->get().get_gotiColour() == gotiColour){
// 			inBoxGotis.erase(i);
// 			return true;
// 		}
// 	}

// 	return false;
// }

// bool ludo_box::appendGoti(ludo_goti& in_goti){
// 	if( in_goti.get_gotiColour() != UnknownColour && in_goti.getCoords() != intTuple(0,0) ){
// 		inBoxGotis.push_back(ref(in_goti));
// 		return true;
// 	}
// 	cerr<<"Can't append Goti, invalid parameters"<<endl;
// 	return false;
// }

// string ludo_box::get_box_content(){
// 	return content;
// }

// //OutOfBoundException START
// const char* OutOfBoundException::what(){
//     return (source + " Index out of Bound").c_str();
// }

// OutOfBoundException::OutOfBoundException(const char* source){
//     this->source = source;
// }
// //OutOfBoundException END


// //GotiNotAvailableException START
// const char* GotiNotAvailableException::what(){
//     if(Colour == UnknownColour){
//     return "INVALID getGoti() REQUEST!!\n";
//     }else if (Colour == ColourLAAL)
//     {
//     return "LAALgoti not in the ludo_box... INVALID getGoti() REQUEST!!\n";
//     }else if (Colour == ColourHARA)
//     {
//     return "HARAgoti not in the ludo_box... INVALID getGoti() REQUEST!!\n";
//     }else if (Colour == ColourPEELA)
//     {
//     return "PEELAgoti not in the ludo_box... INVALID getGoti() REQUEST!!\n";
//     }else if (Colour == ColourNEELA)
//     {
//     return "NEELAgoti not in the ludo_box... INVALID getGoti() REQUEST!!\n";
//     }
// }

// GotiNotAvailableException::GotiNotAvailableException(colours gotiColour) : Colour(gotiColour){}
// //GotiNotAvailableException END

// //##    SIMPLETUPLE_METHODS   ##
// template <class T1, class T2>
// simpleTuple<T1,T2>::simpleTuple(T1 K, T2 V){
//     key = K;
//     value = V;
// }

// template <class T1, class T2>
// void simpleTuple<T1,T2>::setVal(T1 K, T2 V){
//     key = K;
//     value = V;
//   }

// template <class T1, class T2>
// const T1 simpleTuple<T1,T2>::getKey() const{
//     return key;
// }

// template <class T1, class T2>
// const T2 simpleTuple<T1,T2>::getVal() const{
//     return value;
// }
// //##    SIMPLETUPLE_METHODS   ##



// //##    HOMOTUPLE_METHODS   ##
// template<typename T>
// bool homoTuple<T>::operator!=(const homoTuple<T>& tuple2) const{
//   if( this->operator==(tuple2) == false){
//     return true;
//   }
//   else return false;
// }

// template<typename T>
// bool homoTuple<T>::operator==(const homoTuple<T>& tuple2) const{
//   if(this->key == tuple2.key && this->value == tuple2.value){
//     return true;
//   }
//   else return false;
// }

// template<typename T>
// bool homoTuple<T>::operator!=(const simpleTuple<T,T>& tuple2) const{
//   if( this->operator==(tuple2) == false){
//     return true;
//   }
//   else return false;
// }

// template<typename T>
// bool homoTuple<T>::operator==(const simpleTuple<T,T>& tuple2) const{
//   if(this->key == tuple2.getKey() && this->value == tuple2.getVal()){
//     return true;
//   }
//   else return false;
// }

// template<typename T>
// void homoTuple<T>::operator=(const simpleTuple<T,T> &parentTuple){
//   this->key = parentTuple.getKey();
//   this->value = parentTuple.getVal();
// }

// template<typename T>
// T& homoTuple<T>::operator[](unsigned int index){
//   if(index == 0){
//     return this->key;
//     }
//   else if(index == 1){
//     return this->value;
//   }
//   else{
//     throw OutOfBoundException("Tuple");
//   }
// }
// //##  / HOMOTUPLE_METHODS   ##

