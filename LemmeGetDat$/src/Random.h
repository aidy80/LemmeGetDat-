#pragma once

#include "pch.h"

static constexpr uint32_t a = 214013;
static constexpr uint32_t b = 2531011;

/*Simple, efficient random number generator*/
class Random {
private:
	uint32_t currRand;

public:
	/*Initialize with either a time(NULL) seed or a specific passed seed*/
	Random();
	Random(uint32_t initSeed);

	/*Get the next random number in the specified range of [0, range)*/
	uint32_t getRandomNum(uint32_t range);

	/*Manually reset the seed to the generator*/
	void changeSeed(uint32_t newSeed);
};

inline uint32_t Random::getRandomNum(uint32_t range)
{
	currRand = a * currRand + b;
	return (currRand % range);
}

inline void Random::changeSeed(uint32_t newSeed) 
{
	currRand = newSeed;
}
