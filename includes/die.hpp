#pragma once

#define USE_UNIFORM_DISTRIB true
	// @note - By using a uniform distribution, the user can be assured that aall die outputs from 1 to 6 will be almost EQUAL PROBABILITY
	//		   Change this to FALSE, to be TRULY RANDOM

#include <random>
#include <array>
#include <vector>

typedef unsigned short _dieVal;

namespace Die{
	static std::random_device dev_engine;
	static std::mt19937 mt_gen{ dev_engine() };	// used when -> USE_UNIFORM_DISTRIB == false
	static std::uniform_int_distribution distrib{ 1, 6 };	// used when USE_UNIFORM_DISTRIB == true

	int rollDie();	// returns a random number of 1-6
	std::vector<_dieVal>&& getDieResult();	//! Undelete, if YOU like that version of rollDie
	void getDieResult(std::vector<_dieVal>&);

}
