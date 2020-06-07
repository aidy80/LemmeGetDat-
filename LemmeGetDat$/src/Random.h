#pragma once

#include "Card.h"

constexpr unsigned int a = 214013;
constexpr unsigned int b = 2531011;

class Random {
private:
	unsigned int currRand;

public:
	Random();
	Random(unsigned int initSeed);

	unsigned int getRandomNum(int range);
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
