#pragma once

#include "Date.h"
#include <random>
#include <vector>

class RandomUtils
{
private:
	std::default_random_engine Generator;
	std::vector<std::string> birdNames;

public:
	RandomUtils(std::default_random_engine g);
	int getRandomInt(int lower_bound, int upper_bound);
	int getRandomSubgroup();
	char getRandomGroup();
	std::string getRandomName();
	Date getRandomTimestamp();
};
