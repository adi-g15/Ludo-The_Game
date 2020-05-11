#include "global.hpp"

#ifndef CUSTOM_UTIL_FUNC
	#include "util_func.hpp"
#endif
#ifndef _CUSTOM_SIMPLETUPLE
    #include "simpleTuple.hpp"
#endif
#ifndef _CUSTOM_EXCEPTIONS
    #include "exceptions.hpp"
#endif
#ifndef _CUSTOM_LUDO_GOTI
    #include "ludo_goti.hpp"
#endif

#ifndef _GLIBCXX_IOSTREAM
    #include<iostream>
#endif
#ifndef _GLIBCXX_VECTOR
    #include<vector>
#endif
#ifndef _GLIBCXX_MAP
    #include<map>
#endif
#ifndef _STL_PAIR_H
	#include<bits/stl_pair.h>	//FUTURE - REMOVE IT
#endif

using namespace std;

coord_class::coord_class(){
		
		outer_corners.insert({
				make_pair(intTuple(0,6), NORTH),
				make_pair(intTuple(0,8), EAST),
				make_pair(intTuple(14,6), WEST),
				make_pair(intTuple(14,8), SOUTH),
				make_pair(intTuple(6,0), NORTH),
				make_pair(intTuple(8,0), WEST),
				make_pair(intTuple(6,14), EAST),
				make_pair(intTuple(8,14), SOUTH)
		});
		inner_turns.insert({
				make_pair(intTuple(6,6), WEST),
				make_pair(intTuple(6,8), NORTH),
				make_pair(intTuple(8,6), SOUTH),
				make_pair(intTuple(8,8), EAST)
		});

		stops.insert( stops.begin(), {
			
		});

		start_coords.insert({			
			make_pair(ColourLAAL, intTuple(6,1)),
			make_pair(ColourHARA, intTuple(1,8)),
			make_pair(ColourNEELA, intTuple(8,13)),
			make_pair(ColourPEELA, intTuple(13,6)),
		});
}

inline intTuple get_initCoords(colours gotiColour){
	for(auto &&i : publicCoords.start_coords){
		if( i.first == gotiColour )
			return i.second;
	}
	
	return intTuple(0,0);
}

void updateDisplay(){
	int boxlen = 0;
	intTuple tmpDimen(0,0);

	do{	
		tmpDimen = getTerminalDimen();
		boxlen = (min(tmpDimen[0],tmpDimen[1]) - 16)/15;

	}while( min(tmpDimen[0],tmpDimen[1]) < 16 || max(tmpDimen[0],tmpDimen[1]) < 31 );

	for (size_t i = 0; i <(boxlen+1)*15 + 1; i++)
	{
		cout<<'-';
	}
	
	_boardprinterVar.type1(boxlen,0);
	_boardprinterVar.type2(boxlen,1);
	_boardprinterVar.type3(boxlen,2);
	_boardprinterVar.type1(boxlen,3);
	_boardprinterVar.type2(boxlen,4);
	_boardprinterVar.type4(boxlen,5);

	_boardprinterVar.type5(boxlen,6);
	_boardprinterVar.type6(boxlen,7);
	_boardprinterVar.type5(boxlen,8);

	_boardprinterVar.type4(boxlen,9);
	_boardprinterVar.type2(boxlen,10);
	_boardprinterVar.type1(boxlen,11);
	_boardprinterVar.type3(boxlen,12);
	_boardprinterVar.type2(boxlen,13);
	_boardprinterVar.type1(boxlen,14);

	cout<<"\n\nCONSOLE\n";
	for (size_t i = 0; i < tmpDimen[1]; i++)
	{
		cout<<'-';
	}
	cout<<"\nPress Enter to roll the die...";

}

void startGoti(colours gotiColour){
	if(num_laalGoti >= goti_per_user || num_hariGoti >= goti_per_user || num_peeliGoti >= goti_per_user || num_neeliGoti >= goti_per_user){
		cerr<<"Goti request makes the number of ludo_goti of RedColour more than "<<goti_per_user<<endl;
		return;
	}
	else if (gotiColour == UnknownColour)
	{
		cerr<<"startGoti() request made for Unknown Colour"<<endl;
		return;
	}	
	else{
		board[publicCoords.start_coords[gotiColour][0]][publicCoords.start_coords[gotiColour][1]];
	}
}

ludo_box& getBox(const ludo_goti& inGoti){
	if(inGoti.getCoords().getKey() < 15 || inGoti.getCoords().getVal() < 15 || inGoti.getCoords().getKey() >= 0 || inGoti.getCoords().getVal() >= 0){
		throw OutOfBoundException("Board");
	}
	return board[inGoti.getCoords().getKey()][inGoti.getCoords().getVal()];
}

direction dir_to_turn_ifCoord_in_CornerCoordsVec(const intTuple& curr_coords, const std::map<intTuple, direction> &corner_vec){
	for ( auto corner : corner_vec ){
		if(curr_coords == corner.first){
			return corner.second;
		}
	}
	return NO_TURN;
}

void moveGoti(ludo_goti& the_goti, unsigned int dist){
	int x_increment=0, y_increment=0;	//[NOTE] - Can store these in the ludo_box class itself, besides the corner specification
	direction turnDirection;

	board[the_goti.curr_coords.getKey()][the_goti.curr_coords.getVal()].removeGoti(the_goti.gotiColour);

	while(dist--){

		x_increment = y_increment = 0;

		turnDirection = dir_to_turn_ifCoord_in_CornerCoordsVec(the_goti.getCoords(), publicCoords.inner_turns);
		if(turnDirection != NO_TURN){
			if(turnDirection == NORTH){
				x_increment = 2;
				y_increment = 1;
			}
			else if(turnDirection == EAST){
				x_increment = 1;
				y_increment = -2;
			}
			else if(turnDirection == WEST){
				x_increment = -1;
				y_increment = 2;
			}
			else if(turnDirection == SOUTH){
				x_increment = -2;
				y_increment = -1;
			}
		}
		else{

			turnDirection = dir_to_turn_ifCoord_in_CornerCoordsVec(the_goti.getCoords(), publicCoords.outer_corners);
			if(turnDirection != NO_TURN){
				if(turnDirection == NORTH){
					x_increment = 0;
					y_increment = 1;
				}
				else if(turnDirection == EAST){
					x_increment = 1;
					y_increment = 0;
				}
				else if(turnDirection == WEST){
					x_increment = -1;
					y_increment = 0;
				}
				else if(turnDirection == SOUTH){
					x_increment = 0;
					y_increment = -1;
				}
			}

			else{
				if(the_goti.get_curr_direction() == NORTH){
					x_increment = 0;
					y_increment = 1;
				}
				else if(the_goti.get_curr_direction() == EAST){
					x_increment = 1;
					y_increment = 0;
				}
				else if(the_goti.get_curr_direction() == WEST){
					x_increment = -1;
					y_increment = 0;
				}
				else if(the_goti.get_curr_direction() == SOUTH){
					x_increment = 0;
					y_increment = -1;
				}
			}
		}
	}

	board[the_goti.curr_coords[0]][the_goti.curr_coords[1]].appendGoti(the_goti);
	updateDisplay();

}

void takeIntro(){
	intTuple tmpDimen(0,0);

	do{	
		tmpDimen = getTerminalDimen();

	}while( min(tmpDimen[0],tmpDimen[1]) < 16 || max(tmpDimen[0],tmpDimen[1]) < 31 );

	align_text_center(tmpDimen[1], string("Welcome to \"Ludo-The Game\""));
	cout<<endl;
	for (size_t i = 0; i < tmpDimen[1]; i++)
	{
		cout<<'=';
	}
	
	string playerName;
	player i=Player1;
	cout<<"\n\nEnter names of the Players (0 if not playing) : \n";
	
	while(i<=Player4){
		cout<<"Player"<<i<<" - ";
		
		getline(cin, playerName);		
		if(trimString(playerName) != "0"){
			playerMap.insert(make_pair(i,make_pair(playerName,make_pair((colours)i,vector<reference_wrapper<ludo_goti>>()))));
			activePlayers.insert(i);
		}
	}

	cout<<endl<<"\nPress Enter to go to the board...";
	cin.get();
}

bool isPlayerPlaying(player p){
	return playerMap.find(p) != playerMap.end();
}

//  BOARD_PRINTER_METHODS
void _BoardPrinter::type1(int boxlen, int nrow) const{
	string content;

	//Actual-Row Start
	for(size_t j=0; j<boxlen; ++j){
		cout<<'|';
		for (size_t i = 0; i < (boxlen+1)*6 - 1; i++)
			cout<<'\\';

		cout<<'|';

		for (size_t i = 0; i < 3; i++){
			align_text_center(boxlen, board[nrow][6+i].get_box_content());
			cout<<'|';
		}

		for (size_t i = 0; i < (boxlen+1)*6 - 1; i++)	cout<<'\\';
		cout<<"|\n";
	}
	//Actual-Row End

	//Inter-Row line Start
	cout<<'|';
	for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
	for (size_t i = 0; i < boxlen; i++) cout<<'\\';
	cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*3-1; i++) cout<<'-';

	cout<<'|';
	for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
	for (size_t i = 0; i < boxlen; i++) cout<<'\\';
	cout<<"|\n";
	//Inter-Row Line end
}

void _BoardPrinter::type2(int boxlen, int nrow) const{
	//Actual-Row Start
	for(size_t j=0; j<boxlen; ++j){
		cout<<'|';
		for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
		for (size_t i = 0; i < boxlen+2; i++) cout<<' ';
		for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
		for (size_t i = 0; i < boxlen+2; i++) cout<<' ';
		for (size_t i = 0; i < boxlen; i++) cout<<'\\';
		cout<<'|';


		for (size_t i = 0; i < 3; i++){
			align_text_center(boxlen, board[nrow][6+i].get_box_content());
			cout<<'|';
		}

		for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
		for (size_t i = 0; i < boxlen+2; i++) cout<<' ';
		for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
		for (size_t i = 0; i < boxlen+2; i++) cout<<' ';
		for (size_t i = 0; i < boxlen; i++) cout<<'\\';
		cout<<"\n";
	}
	//Actual-Row End

	//Inter-Row line Start
	cout<<'|';
	for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
	for (size_t i = 0; i < boxlen; i++) cout<<'\\';
	cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*3-1; i++) cout<<'-';

	cout<<'|';
	for (size_t i = 0; i < boxlen; i++)	cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
	for (size_t i = 0; i < (boxlen)*2 + 1; i++)	cout<<'\\';
	for (size_t i = 0; i < boxlen+2; i++) cout<<'-';
	for (size_t i = 0; i < boxlen; i++) cout<<'\\';
	cout<<"|\n";
	//Inter-Row Line end

}

void _BoardPrinter::type3(int boxlen, int nrow) const{
	//Actual-Row Start
	for(size_t j=0; j<boxlen; ++j){
		cout<<'|';
		for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
		cout<<'|';


		for (size_t i = 0; i < 3; i++){
			align_text_center(boxlen, board[nrow][6+i].get_box_content());
			cout<<'|';
		}

		for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
		cout<<"|\n";
	}
	//Actual-Row End

	//Inter-Row line Start
	cout<<'|';
	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
	cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*3-1; i++) cout<<'-';

	cout<<'|';
	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
	cout<<"|\n";
	//Inter-Row Line end
}

void _BoardPrinter::type4(int boxlen, int nrow) const{
	//Actual-Row Start
	for(size_t j=0; j<boxlen; ++j){
		cout<<'|';
		for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
		cout<<'|';


		for (size_t i = 0; i < 3; i++){
			align_text_center(boxlen, board[nrow][6+i].get_box_content());
			cout<<'|';
		}

		for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'\\';
		cout<<"|\n";
	}
	//Actual-Row End

	//Inter-Row line Start
	cout<<'|';
	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'-';

	for (size_t i = 0; i < (boxlen+1)*3-1+2; i++) cout<<'-';

	for (size_t i = 0; i < (boxlen+1)*6-1; i++)	cout<<'-';
	cout<<"|\n";
	//Inter-Row Line end
}

void _BoardPrinter::type5(int boxlen, int nrow) const{
	//Actual-Row Start
	for(size_t j=0; j<boxlen; ++j){
		cout<<'|';
		for (size_t i = 0; i < 6; i++){
			align_text_center(boxlen, board[nrow][6+i].get_box_content());
			cout<<'|';
		}

		for (size_t i = 0; i < (boxlen+1)*3 -1; i++) cout<<' ';	
		cout<<'|';

		for (size_t i = 0; i < 6; i++){
			align_text_center(boxlen, board[nrow][6+i].get_box_content());
			cout<<'|';
		}
	}
	//Actual-Row End

	//Inter-Row line Start
	cout<<'|';
	for (size_t i = 0; i < (boxlen+1)*6-1; i++) cout<<'-';
	cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*3 -1; i++) cout<<' ';	
	cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*6-1; i++) cout<<'-';
	cout<<'|';
	//Inter-Row Line end
}

void _BoardPrinter::type6(int boxlen, int nrow) const{
	//Actual-Row Start
	for(size_t j=0; j<boxlen; ++j){
		cout<<'|';
			align_text_center(boxlen, board[nrow][6].get_box_content());
			cout<<'|';
		for (size_t i = 0; i < 5; i++){
			align_text_center(boxlen, board[nrow][7+i].get_box_content());
			cout<<' ';
		}

		for (size_t i = 0; i < (boxlen+1)*3 -1; i++) cout<<' ';	
		cout<<'|';

		for (size_t i = 0; i < 4; i++){
			align_text_center(boxlen, board[nrow][6+i].get_box_content());
			cout<<' ';
		}
		for (size_t i = 0; i < 2; i++){
			align_text_center(boxlen, board[nrow][6+i].get_box_content());
			cout<<'|';
		}

	}
	//Actual-Row End
	//Inter-Row line Start
	cout<<'|';
	for (size_t i = 0; i < (boxlen+1)*6-1; i++) cout<<'-';
	cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*3 -1; i++) cout<<' ';	
	cout<<'|';

	for (size_t i = 0; i < (boxlen+1)*6-1; i++) cout<<'-';
	cout<<'|';
	//Inter-Row Line end	
}

int rolldie(){
	bool *b= new bool;
	uintptr_t retval = reinterpret_cast<uintptr_t>(b);
	delete b;

	return (retval%6)+1;
}

void player_move(std::vector<int> &dieOutputs){

}

//  BOARD_PRINTER_METHODS

//  LEARNT|+QUESTIONS  //
/*[LEARNT] - Nesting functions (ie. function declaration inside function) is not supported by standards in C/C++
				 But, gcc supports it as a 'non-standard' extension
				 BUT, in C++ mode, it's not allowed, 'Local Functions' aren't supported
				 BUT, FROM C++11, IT IS SUPPORTED, using LAMDAs {Imp. - Lambdas have a semicolon at end}*/
/*LAMBDAs - 1. Have a ';' at end
			2. '[]' is the "capture clause", also called 'lambda-introducer'
			3. parameter list '()' is optional
			4. These are the parts:
					[] () mutable throw() -> int {//body//}
					 |  |   |        |       |    
		capture clause  |  optional  |  trailing-return-type(optional)
					parameters   exception specification(optional)
*/
		//QUestion - Why didnt this lambda work?
	// }while ( (tmpDimen.getKey() < 31 || tmpDimen.getVal() < 31) ? true : []{	//Lamda Function (used to declare this 'local' function)
	// 	cerr<<"INSUFFICIENT HEIGHT/WIDTH FOR DISPLAY!"<<endl;
	// 	return false;
	// } );
