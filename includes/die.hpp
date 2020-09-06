#pragma once

#include <random>
#include <array>
#include <vector>

typedef unsigned short _dieVal;

namespace Die{
	//! Using 4 different generators as well as sequences, randomly for each player
	static std::random_device dev_engine;
	static std::array<std::mt19937, 4> mt = { std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()),std::mt19937(dev_engine()) };
	static std::array<std::uniform_int_distribution<int>, 4> dist = { std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6), std::uniform_int_distribution<int>(1,6) };	//uniform distribution from [1,6]

	std::vector<_dieVal> rolldie(void);	//! Undelete, if YOU like that version of rollDie
	void rolldie(std::vector<_dieVal>&);

}
