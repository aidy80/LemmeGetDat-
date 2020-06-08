#pragma once

#include "pch.h"

static constexpr unsigned int a = 214013;
static constexpr unsigned int b = 2531011;

/*Simple, efficient random number generator*/
class Random {
private:
	unsigned int currRand;

public:
	/*Initialize with either a time(NULL) seed or a specific passed seed*/
	Random();
	Random(unsigned int initSeed);

	/*Get the next random number in the specified range of [0, range)*/
	unsigned int getRandomNum(int range);

	/*Manually reset the seed to the generator*/
	void changeSeed(int newSeed);
};

inline unsigned int Random::getRandomNum(int range)
{
	currRand = a * currRand + b;
	return (currRand % range);
}

inline void Random::changeSeed(int newSeed) 
{
	currRand = newSeed;
}
