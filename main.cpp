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
#include "./util/terminalDimensions.hpp"
#include "./util/util_func.hpp"
#include<unordered_set>

#define stringify( name ) #name
/*LEARNT - stringify() can turn any name into text, BUT remember, it ignores trailing and front spaces, for eg. in stringify( Hello World ), first and last space ignored*(Verified)/
*/

/*LEARNT - The component type of STL containers like vectors must be 'asignable'.
			References are NOT assignable, ie. we can only initialise them once, and,
			you can't make them reference something else later (you can't assign it to reference any other vairable, and even after equating 2 references, only the values of variable they reference is changed, not the addresses they both refer to)
			Due to same reason, other non-assignable types are also not allowed as components of containers, for eg. vector<const int> not allowed
*/

typedef intTuple coordinates;

using namespace std;

//Replace all 'intTuple' to 'intTuple'
// vector<intTuple> corners, stops;	////[NOTE]: Accessing these gives error: this declaration has no storage class or type specifier
// vector<tmp_startXY_class> start_coords;
// vector<simpleTuple<colours,intTuple>> start_coords;
///Use map for these

class box;
class goti;

	vector<vector<box>> board(15);
	unordered_set<player> activePlayers;	//Actually order is necessary, but that will be managed by 'to be' friend takeIntro()
	map<colours, player> ColourMap;
	map<player, pair< string,pair<colours ,vector< reference_wrapper<goti> >> >> playerMap;
/*
	PLAYER MAP
	======================
	PlayerID(enum)  ----> string PlayerName
						  	colour GotiColour
						  	vector of Gotis

	Similarly, a more complex ColourMap MAYBE implemented, but for now the 'players' and 'colours' enum correspond
*/

	int goti_per_user = 4;
	int num_laalGoti = 0; 
	int num_hariGoti = 0; 
	int num_peeliGoti = 0; 
	int num_neeliGoti = 0; 

	void global_destructor(); //... iterator through whole board, and 'delete goti initialised with startGoti()'

class coord_class{
public:
	vector<intTuple> stops;	//[NOTE]: Accessing these gives error: this declaration has no storage class or type specifier

	vector<pair<intTuple, direction>> outer_corners, inner_turns;
	vector<pair<intTuple, direction>> home_turns;
	// vector<pair<colours,intTuple>> start_coords;
	map<colours,intTuple> start_coords;

	coord_class(){
		
		
		outer_corners.insert( outer_corners.begin(), {	//[NOTE] - This ensures, the game is 'unidirectional'
				pair<intTuple, direction>(intTuple(0,6), NORTH),
				pair<intTuple, direction>(intTuple(0,8), EAST),
				pair<intTuple, direction>(intTuple(14,6), WEST),
				pair<intTuple, direction>(intTuple(14,8), SOUTH),
				pair<intTuple, direction>(intTuple(6,0), NORTH),
				pair<intTuple, direction>(intTuple(8,0), WEST),
				pair<intTuple, direction>(intTuple(6,14), EAST),
				pair<intTuple, direction>(intTuple(8,14), SOUTH)
		});
		inner_turns.insert( inner_turns.begin(), {
				pair<intTuple, direction>(intTuple(6,6), WEST),
				pair<intTuple, direction>(intTuple(6,8), NORTH),
				pair<intTuple, direction>(intTuple(8,6), SOUTH),
				pair<intTuple, direction>(intTuple(8,8), EAST)
		});

		stops.insert( stops.begin(), {
			
		});

		start_coords.insert({			
			pair<colours, intTuple>(ColourLAAL, intTuple(6,1)),
			pair<colours, intTuple>(ColourHARA, intTuple(1,8)),
			pair<colours, intTuple>(ColourNEELA, intTuple(8,13)),
			pair<colours, intTuple>(ColourPEELA, intTuple(13,6)),
		});
	}
}publicCoords;

class _BoardPrinter{ //Only for use by updateDisplay() function

    void type1(int,int) const;
    void type2(int,int) const;
    void type3(int,int) const;
    void type4(int,int) const;
    void type5(int,int) const;
    void type6(int,int) const;

    friend void updateDisplay();

}_boardprinterVar;

class box{
	vector<reference_wrapper<goti>> inBoxGotis;
	string content;

	//Corner specification
	bool isOuterCorner;
	bool isRectCorner;
	direction turnDir;

public:
	reference_wrapper<goti> getGoti(colours);	//Do have null check
					//ALTERNATE NULL CHECK -> For checking if passed goti is valid, check it's colour and/or coords to be valid
	bool removeGoti(colours);
	bool appendGoti(goti&);
	string get_box_content();

	box(){
			content = " ";	//one charachter, or two
		}
};

///Complete this function
bool dir_to_turn_ifCoord_in_CornerCoordsVec(const intTuple& curr_coords, const pair<intTuple, direction> &cornerCoord);
void updateDisplay();
void moveGoti(goti& inGoti, unsigned int dist);
void startGoti(const colours gotiColour);	//use startGoti here
box& getBox(const goti&);	//May make this a 'private' function in a class
void takeIntro();
bool isPlayerPlaying(player);

inline intTuple get_initCoords(colours gotiColour){
	for(auto &&i : publicCoords.start_coords){
		if( i.first == gotiColour )
			return i.second;
	}
	
	return intTuple(0,0);
}

//[LEARNT_KNOW_MORE] - 'abstract' isn't a keyword in C++
class goti{
protected:
	colours gotiColour;	//an enum variable
	direction curr_direction;	//[FUTURE] - Likely chose between having coordinates, or pointer to goti(consider the NULL checks)
	box* curr_box;
	intTuple curr_coords;	//ERROR_SOLVED - Showing error : expected identifier before numeric constant intTuple coords(0,0);
									/*LEARNT - We can't call the constructor of a datatype right at declaration, "in a class outside any method"
											But, we can braces{} instead of () to initialize right at declaration, BUT... by passing a constructed object
												vector<int> v(5);	//Wrong
												vector<int> v{vector<int>(5)}; //Right
												
											So, first declare then, call their constructors in constructor(See this class's constructor)*/

public:	//public Getters maybe used to ensure that private mamber can't be modified from outside
	const intTuple getCoords() const{
		if(curr_coords.getKey() == 0 && curr_coords.getVal()==0){
			cerr<<"Coordinates of Goti are corrupt (0,0)\n"<<endl;
		}
		return curr_coords;
	}
	direction get_curr_direction() const{
		return curr_direction;
	}
	//[DONE][NOTE] - Improve security of this function, else coordinates being private wont be useful... Solution used... made moveGoti a friend function
	
	goti() : curr_coords(0,0) {	//Initializing using constructor
		gotiColour = UnknownColour;
	}

	colours get_gotiColour(void) const{
		return gotiColour;
	}

	// friend void moveGoti(reference_wrapper<goti>, unsigned int);
	friend void moveGoti(goti&, unsigned int);

};	//Have abstract functions in it too

class laalGoti : public goti {
public:
	laalGoti(){
		gotiColour = ColourLAAL;
	}
};

class hariGoti : public goti{
public:
	hariGoti(){
		gotiColour = ColourHARA;
		// this->curr_coords = get_initCoords(ColourHARA);
	}
};

class peeliGoti : public goti{
public:
	peeliGoti(){
		gotiColour = ColourPEELA;
		// this->curr_coord = get_initCoords(ColourPEELA);
	}
};

class neeliGoti : public goti{
public:
	neeliGoti(){
		gotiColour = ColourNEELA;
		// this->curr_coord = get_initCoords(ColourNEELA);
	}
};

int rolldie(){
	bool *b= new bool;
	uintptr_t retval = reinterpret_cast<uintptr_t>(b);
	delete b;	//Should statically be deleted as well, without 'delete'

	return (retval%6)+1;
}

int main(int argc, char const *argv[]) {

	takeIntro();	//Initialises the PlayerMap

	//Initialising Board
	board.reserve(15);
	for (size_t i = 0; i < 15; i++) {
		board.emplace_back(vector<box>());
		for (size_t j = 0; j < 15; j++) {
			board[i].emplace_back(box());
		}
	}

	player currentPlayer = *activePlayers.begin();
	colours currentGotiColour = playerMap[currentPlayer].second.first;
	int dieNumber;

	//GamePlay Starts
	takeIntro();
	while (true){

		currentGotiColour = playerMap[currentPlayer].second.first;
		
		updateDisplay();
		cin.get();

		startGoti(currentGotiColour);	//ie. it was a 6
		
		///Update current Player here
		
		moveGoti(board[1][6].getGoti(currentGotiColour),5);

	}

	return 0;
}
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

void updateDisplay(){
	// reUpdate:	//[NOTE] - To prevent this function from 'recursing' too much, due to the while loop
		//used do-while loop instead of this
	int boxlen = 0;
	intTuple tmpDimen(0,0);

	do{	
		tmpDimen = getTerminalDimen();
		boxlen = (min(tmpDimen[0],tmpDimen[1]) - 16)/15;

	}while( min(tmpDimen[0],tmpDimen[1]) < 16 || max(tmpDimen[0],tmpDimen[1]) < 31 );
		//QUestion - Why didnt this lambda work?
	// }while ( (tmpDimen.getKey() < 31 || tmpDimen.getVal() < 31) ? true : []{	//Lamda Function (used to declare this 'local' function)
	// 	cerr<<"INSUFFICIENT HEIGHT/WIDTH FOR DISPLAY!"<<endl;
	// 	return false;
	// } );

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

	cout<<'\n\nCONSOLE\n';
	for (size_t i = 0; i < tmpDimen[1]; i++)
	{
		cout<<'-';
	}
	cout<<"\nPress Enter to roll the die...";

}

void startGoti(colours gotiColour){

	//[FUN][FUTURE] - if(number of that colour gotis = 0 earlier), then
	//Congratulate THE USER "Chal yaar, ek goti to nikli ;-)"

	if(num_laalGoti >= goti_per_user || num_hariGoti >= goti_per_user || num_peeliGoti >= goti_per_user || num_neeliGoti >= goti_per_user){
		cerr<<"Goti request makes the number of goti of RedColour more than "<<goti_per_user<<endl;
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

box& getBox(const goti& inGoti){
	if(inGoti.getCoords().getKey() < 15 || inGoti.getCoords().getVal() < 15 || inGoti.getCoords().getKey() >= 0 || inGoti.getCoords().getVal() >= 0){
		throw OutOfBoundException("Board");
	}
	return board[inGoti.getCoords().getKey()][inGoti.getCoords().getVal()];
}

direction dir_to_turn_ifCoord_in_CornerCoordsVec(const intTuple& curr_coords, const vector<pair<intTuple, direction>> &corner_vec){
	for ( auto corner : corner_vec ){
		if(curr_coords == corner.first){
			return corner.second;
		}
	}
	return NO_TURN;
}

// void moveGoti(reference_wrapper<goti> the_goti, unsigned int dist){
void moveGoti(goti& the_goti, unsigned int dist){
	int x_increment=0, y_increment=0;	//[NOTE] - Can store these in the box class itself, besides the corner specification
	direction turnDirection;

	the_goti.curr_box = NULL;	//Removing from box
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

reference_wrapper<goti> box::getGoti(colours gotiColour){
	
	for (auto &&i : inBoxGotis)
	{
		if(i.get.get_gotiColour() == gotiColour){
			return i;
		}
	}		

	throw GotiNotAvailableException(gotiColour);
}

/*[LEARNT] - The error "pointer to incomplete class type is not allowed", generally related to header files, when "I forward declared the class in the header, and failed to include the full header for the class"
			AND, "An 'incomplete class' is one that is declared but not defined"
			ALSO, If your class is defined as a typedef: "typedef struct struct{};", and then try to refer to it as 'struct myclass' anywhere else, you'll get many such errors, to solve it, remove 'class/struct' from variable declarations, ie. 'mystruct *var = value;' instead of 'struct mystruct *var=value'*/

bool box::removeGoti(colours gotiColour){
	vector<reference_wrapper<goti>>::const_iterator i = inBoxGotis.begin();
	for ( ; i < inBoxGotis.end(); ++i)
	{
		if( i->get.get_gotiColour() == gotiColour){
			inBoxGotis.erase(i);
			return true;
		}
	}

	return false;
}

bool box::appendGoti(goti& in_goti){
	if( in_goti.get_gotiColour() != UnknownColour && in_goti.getCoords() != intTuple(0,0) ){
		inBoxGotis.push_back(ref(in_goti));
		return true;
	}
	cerr<<"Can't append Goti, invalid parameters"<<endl;
	return false;
}

string box::get_box_content(){
	return content;
}


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
			playerMap.insert(make_pair(i,make_pair(playerName,make_pair((colours)i,vector<reference_wrapper<goti>>()))));
			activePlayers.insert(i);
		}
	}

	cout<<endl<<"\nPress Enter to go to the board...";
	cin.get();
}

bool isPlayerPlaying(player p){
	return playerMap.find(p) != playerMap.end();
}