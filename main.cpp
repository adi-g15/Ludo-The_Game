/*
Programmer - Aditya Gupta (Techy15)
Language - C++
Program - LudO - The Game
*/

#include<iostream>
#include "board.hpp"

int main(int argc, char const *argv[]) {

	game newGame;
	short playConsent = 1;

	//GamePlay starts
	do{
		if( !newGame.InitGame(playConsent) ){
			std::cerr<<"Couldn't initiate the game... Exiting"<<std::endl;
			return -1;		
		}

		newGame.play();

		std::cout<<"Enter 1 if you want to play again and reset\nEnter 2 if you want to play with same players\nAnything else to say GoodBye :-)\nYour Consent : ";
		std::cin>>playConsent;

	}while ( playConsent == 1 || playConsent == 2 );

	std::cout<<"Khelne ke liye Dhanyawaad :-D"<<std::endl;

	return 0;
}

//[LEARNT] - In C++ if an object is constant, then it is totally CONSTANT, not like Java, where we can change contents of object, even with const reference