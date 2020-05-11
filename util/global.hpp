#define TMP 1

#ifndef CUSTOM_EXCEPTIONS
    #include "exceptions.hpp"
#endif
#ifndef CUSTOM_ENUMERATIONS
    #include "enumerations.hpp"
#endif
// #ifndef CUSTOM_LUDO_BOX
//     #include "ludo_box.hpp"
// #endif
#ifndef CUSTOM_SIMPLETUPLE
	#include "simpleTuple.hpp"
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
#ifndef _GLIBCXX_UNORDERED_SET
    #include<unordered_set>
#endif

class ludo_box;
class ludo_goti;

	std::vector<std::vector<ludo_box>> board(15);
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

	int goti_per_user = 4;
	int num_laalGoti = 0; 
	int num_hariGoti = 0; 
	int num_peeliGoti = 0; 
	int num_neeliGoti = 0; 

	void global_destructor(); //... iterator through whole board, and 'delete ludo_goti initialised with startGoti()'

class coord_class{
public:
	std::vector<intTuple> stops;
	std::map<intTuple, direction> outer_corners, inner_turns;
	std::map<intTuple, direction> home_turns;
	std::map<colours,intTuple> start_coords;

	coord_class();
}publicCoords;

class _BoardPrinter{ //@info Only for use by updateDisplay() function
    void type1(int,int) const;
    void type2(int,int) const;
    void type3(int,int) const;
    void type4(int,int) const;
    void type5(int,int) const;
    void type6(int,int) const;

    friend void updateDisplay();
}_boardprinterVar;

inline intTuple get_initCoords(colours);
bool dir_to_turn_ifCoord_in_CornerCoordsVec(const intTuple& curr_coords, const std::map<intTuple, direction> &cornerCoord);
void updateDisplay();
void player_move(std::vector<int>&);
void moveGoti(ludo_goti&, unsigned int);
void startGoti(const colours);
ludo_box& getBox(const ludo_goti&);	//May make this a 'private' function in a class
void takeIntro();
bool isPlayerPlaying(player);
int rolldie();

//  FUTURE+ERRORS  //
	//[FUN][FUTURE] - if(number of that colour gotis = 0 earlier), then
	//Congratulate THE USER "Chal yaar, ek ludo_goti to nikli ;-)"
//[ERROR]: [at declaration of 'coord_class::stops'] Accessing these gives error: this declaration has no storage class or type specifier