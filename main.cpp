
/*
Programmer - Aditya Gupta (Techy15)
Language - C++
Program - LudO - The Game
*/

/* PLANS- Add functionality for name of player */

//IMP NOTE - See all notes, written in this form "//NOTE..."

//NOTE- Mark 'R','G','B','Y' for gotis, ignore doubling for now
//NOTE- Settings will have options: change default gameplay order(ie RBYG), change/give names to each colour
//NOTE- Add code in case of attacks
//NOTE- Add stops, and ways to show them distinct from others {may require grahics.h}

#include<iostream>

/* #include<fstream>
   To be used in case save & resume to be used*/

#include<cstdio>
#include<cstdlib>
using namespace std;

// PLace within appropriate block ...
	short diethrow(){
	rethrow:
	char temp=new char;
	long double location=&temp;	/*PROblEM - Actually location of temp will be a hexadecimal location, how do we store hexadecimal 						locations*/	
					//NOTE- then convert this hexadecimal number to a decimal number (say decnum)
	...
	decnum%=7; /*now decnum is any number between 0 to 6*/
	if(decnum!=0) return decnum;
	else goto rethrow;
}
*/

int i=0,j=0;
struct box{
		box *next,*prev;
		char curr, temp;	/* temp stores previous state of that box */
					//NOTE- Initialize curr with NULL (or equivalent character that works as NULL pointer)
		short flagp, flagu;	/* flagp for 'if any goti present'; flagu is an unified flag */
					/* Unified Flag for 'kind of use' (flagu) - 
						0 if 'not' usable (greyed out regions)
						1 if 'normal' block (the usual path)
						2 if 'locked home' (the place where we need 6 to get out)
						3 if 'home path' (the goti colour specific path)*/
					//NOTE- Maybe flagp can be used in case of knowing 'dual/multiple goti presence also'
					//NOTE- flagr and temp collectively used to display to user who removed whom
		bool flagr;		/*flagr for 'if previous was removed/attacked' ; */	
		box(){ next=prev=NULL; curr=//NOTE- Something that is equavalent to NULL' in pointers 
			temp=//null character
			flagr=0; flagp=0;	//NOTE- If locked home position is causing problem this maybe the error, ie. initializing them also with flagp=0
			flagu=1;
			}
		~box();
	};

struct dice{
		short flag; /*'1' if not 6, '2' to store 2 numbers, '3' to store 3...*/ 
		short out[10]; /*stores max 10 die throw result*/
				//NOTE- Initialize all elements of out[] with 0
		char user; //either of RGBY
				//NOTE- Add conditions for 3 sixes... for eg. if(out[3]!=0) then directly consider out[3] onwards...BUT this will not always be true... think
	};

class game{
	box Board[15][15]; //BOARD WILL BE AN ARRAY OF ALL BOXES
	public:
		~game();
		game(){
			/*Setting unified flag as 0*/
			for(i=0; i<6; i++)
				for(j=0; j<6; j++) Board[i][j]->flagu=0;
			for(i=0; i>8,i<15; i++)
				for(j=0; j<6; j++) Board[i][j]->flagu=0;
			for(i=0; i<6; i++)
				for(j=0; j>8,j<15; j++) Board[i][j]->flagu=0;
			for(i=0; i>8,i<15; i++)
				for(j=0; j>8,j<15; j++) Board[i][j]->flagu=0;
			for(i=0; i>5,i<9; i++)
				for(j=0; j>5,j<9; j++) Board[i][j]->flagu=0;			

			/*Setting unified flag as 2*/
			for(i=1; i<5; i+=3)
				for(j=1;j<5;j+=3) Bonus[i][j]->flagu=2;
			for(i=10; i<14; i+=3)
				for(j=1; j<5; j+=3) Bonus[i][j]->flagu=2;		
			for(i=1; i<5; i+=2)
				for(j=10;j<14;j+=3) Bonus[i][j]->flagu=2;
			for(i=10; i<14; i++=2)
				for(j=10; j<14; j+=3) Bonus[i][j]->flagu=2;

			/*Setting unified flag as 3*/
			
			}		
};

//PIECES OF GAME... (that have to be arranged later)
1. Way to input: //Check Page 3-bottom and page 4 (for explained)
	Show the results of die throw (ie. basically list all non zero entries in out[])
	And, then list locations of all gotis (of current player), then let the user chose among them
		a. If 'only' out[0]!=0 (ie. all others are zeroes... ie. no six, no attack, no finish), move selected one
		b. In case of multiple non zero entries take two inputs goti number(choice from list) and distance by which it is to be moved then check if sum of any possible permutations of the die results equals the distance input... then move, and then give list again

2. RGBY classes:

	class R{
			box *goti1,*goti2,*goti3,*goti4;
			short locknum; /*number of locked beads (used to replace beads in locked home {by default in anti-clockwise})*/
			short disthpath[4], distnextturn[4]; /*both distances excludes current tile; distance from homepath is from last tile before home'path'*/
							     /*can be used to calculate distance from finish*/

					//NOTE - SEE PG. 11 & 12-up; For ways to prevent gotis from reprogressing again at normal path instead of home path
			public:
				R(){ goti1=&board[4][4]; goti2=&board[1][4]; goti3=&board[1][1]; goti4=&board[4][1]; locknum=4; }
				~R(){}
		};
/*
//NOTE- USE THIS in Actual Program (locked beads link to NULL, better and wont cause problems since locknum will be used to show the locked beads)
	class R{
			box *goti1,*goti2,*goti3,*goti4;
			short locknum; 
			public:
				class gotiloc{ box *front, *rear; 
						public:
							gotiloc(){front=rear=NULL; createlist(); }
							void createlist(); };
				R(){ goti1=NULL; goti2=NULL; goti3=NULL; goti4=NULL; locknum=4; } //QUESTION- NULL is a pointer or a location? ie *ptr=NULL, ptr=&NULL or ptr=NULL??? Which one's correct??
				~R(){}
void R::gotiloc::createlist()
{
	front=goti1; goti1->prev=front; goti2->prev=goti1; goti3->prev=goti2; goti4->prev=goti3; goti1->next=goti2; goti2->next=goti3; goti3->next=goti4; rear=goti4;
}
		};*/
	class B{
			box *goti1,*goti2,*goti3,*goti4;
			short locknum; /*number of locked beads (used to replace beads in locked home {by default in anti-clockwise})*/
			public:
				B(){ goti1=&board[13][4]; goti2=&board[10][4]; goti3=&board[10][1]; goti4=&board[13][1]; locknum=4; }
				~B(){}
		};
	class Y{
			box *goti1,*goti2,*goti3,*goti4;
			short locknum; /*number of locked beads (used to replace beads in locked home {by default in anti-clockwise})*/
			public:
				Y(){ goti1=&board[13][13]; goti2=&board[10][13]; goti3=&board[10][10]; goti4=&board[13][10]; locknum=4; }
				~Y(){}
		};
	class G{
			box *goti1,*goti2,*goti3,*goti4;
			short locknum; /*number of locked beads (used to replace beads in locked home {by default in anti-clockwise})*/
			public:
				G(){ goti1=&board[4][13]; goti2=&board[1][13]; goti3=&board[1][10]; goti4=&board[4][10]; locknum=4; }
				~G(){}
		};
