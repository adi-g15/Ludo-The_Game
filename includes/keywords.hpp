/*This is to generalise many content shown, similar to how we use 'string resources' in Android, instead of hard-coded strings*/
/*CONTRIBUTION - 
	This list isn't quite complete now, SUPPORT by making issues/PRs for more generalisation
	For example, you may even set variables here too, for example, "size_t maxPlayers = 4;", even try that, i will merge the PR, and a thanks to you! :D
*/
#pragma once

#include <string>
#include <map>
#include "enumerations.hpp"

static std::string robot_keyword = "ROBOT";

static std::string thinker_keyword = "Jai Hind";

static std::map<colours, char> colourCodes = {{colours::ColourLAAL, 'R'}, {colours::ColourHARA, 'G'}, {colours::ColourPEELA, 'Y'}, {colours::ColourNEELA, 'B'}};