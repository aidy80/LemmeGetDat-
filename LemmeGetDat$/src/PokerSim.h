#pragma once

#include "pch.h"
#include "Deck.h"

char* getBestHands(Pool& pool, Hand* hands, unsigned char numHands = 6);

std::vector<float> calcPreFlopEquity(Hand* hands, unsigned char numHands = 6);
