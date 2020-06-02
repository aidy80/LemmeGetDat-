#include "pch.h"
#include "Random.h"

Random::Random() :
	currRand((unsigned int)time(NULL)) {}

Random::Random(unsigned int initSeed) :
	currRand(initSeed) {}

