#include<iostream>
#include <chrono>

#include "game.h"

static int numGamePlays = 0;

int main(int argc, char const *argv[]) {

	game newGame;
	short playConsent = 1;

	//GamePlay starts
	
	try{

		do{
			// for(; numGamePlays<1000; ++numGamePlays)
{			if( !newGame.InitGame(playConsent) ){
				std::cerr<<"Couldn't initiate the game... Exiting"<<std::endl;
				return -1;		
			}

				newGame.play();
}
			std::cout<<"Enter 1 if you want to play again and reset\nEnter 2 if you want to play with same players\nAnything else to say GoodBye :-)\nYour Consent : ";
			std::cin>>playConsent;

		}while ( playConsent == 1 || playConsent == 2 );
		std::cout<<"Khelne ke liye Dhanyawaad :-D"<<std::endl;
	}
	catch(endApplication& e){
		std::cout<<"That was the "<<numGamePlays+1<<"th run"<<std::endl;
		std::cerr<<e.what()<<std::endl;;
		return 0;
	}
	catch(std::exception& e){
		std::cerr<<e.what();
		return 0;
	}
	
	return 0;
}

/*[LEARNT] - In C++ if an object is const, then it is totally CONSTANT, not like Java, where we can change contents of object, even with const reference - 
* In C++, 'const' is like a promise to be kept, but can be broken using pointers sometimes, as needed
    const Class* n;   //Can't change the 'content' of what the pointer points to (ie. members 'shouldn't' be modified), but can make it point to a different location
    Class const* n; //Same as above, just the astericks is at different position    

    Class* const n; //Java like, Can modfify the content, but can't make it point to something else
    const Class* const n;   //TOTALLY Constant by every mean    
*/
