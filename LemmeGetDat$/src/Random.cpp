#include "Random.h"

Random::Random() :
	currRand((unsigned int)time(NULL)) {}

Random::Random(uint32_t initSeed) :
	currRand(initSeed) {}

