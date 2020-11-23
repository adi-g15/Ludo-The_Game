#include <iostream>

#include "game.hpp"

static int numGamePlays = 0;

using std::clog;

int main(int argc, char const *argv[])
{
	game newGame;

	//GamePlay starts
	try
	{
		short playConsent = 1;
		do
		{
			if (!newGame.InitGame(playConsent))
			{
				std::cerr << "Couldn't initiate the game... Exiting" << std::endl;
				return -1;
			}

			newGame.play();

			std::cout << "Enter 1 if you want to play again and reset\nEnter 2 if you want to play with same players\nAnything else to say GoodBye :-)\nYour Consent : ";
			playConsent = std::getchar();
		} while (playConsent == 1 || playConsent == 2);
		std::cout << "Khelne ke liye Dhanyawaad :-D" << std::endl;
	}
	catch (endApplication &e)
	{
		std::cerr << e.what() << std::endl;
		return 0;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what();
		return 0;
	}

	return 0;
}
