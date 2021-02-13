/*This is to generalise many content shown, similar to how we use 'string resources' in Android, instead of hard-coded strings*/
/*CONTRIBUTION -
	This list isn't quite complete now, SUPPORT by making issues/PRs for more generalisation
	For example, you may even set variables here too, for example, "size_t maxPlayers = 4;", even try that, i will merge the PR, and a thanks to you! :D
*/
#pragma once

#include "enumerations.hpp"

#include <string_view>
#include <map>

static const char* robot_keyword = "ROBOT";

static const char* thinker_keyword = "Jai Hind";

static std::map<Colour, std::string_view> colourNames{
	{Colour::LAAL, "RED"},
	{Colour::HARA, "HARA"},
	{Colour::PEELA, "PEELA"},
	{Colour::NEELA, "BLUE"}
};

static std::map<Direction, std::string_view> dirNames{
	{ Direction::NO_TURN, "No_Turn"},
	{ Direction::UP, "UP" },
	{ Direction::LEFT, "RIGHT" },
	{ Direction::RIGHT, "LEFT" },
	{ Direction::DOWN, "DOWN" }
};

static std::map<Colour, char> colourCodes{
	{Colour::LAAL, 'R'},
	{Colour::HARA, 'G'},
	{Colour::PEELA, 'Y'},
	{Colour::NEELA, 'B'}
};

static std::map<Player, std::string_view> playerId{
	{ Player::NotDefined, "NULL" },
	{ Player::_1, "1" },
	{ Player::_2, "2" },
	{ Player::_3, "3" },
	{ Player::_4, "4" },
};

static std::ostream& operator<<(std::ostream& stream, const Colour c){
	stream << colourNames[c];
	return stream;
}

static std::ostream& operator<<(std::ostream& stream, const Direction d){
	stream << dirNames[d];
	return stream;
}
