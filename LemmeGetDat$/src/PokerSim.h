#pragma once

#include "pch.h"
#include "Table.h"
#include "BestHand.h"

std::vector<float> calcPreFlopEquity(Hand* hands, unsigned char numHands, Deck& deck);
