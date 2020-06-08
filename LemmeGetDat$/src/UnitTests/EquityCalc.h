#pragma once

#include "../pch.h"
#include "../Table.h"
#include "../BestHand.h"
#include "../TwoDimArray.h"

std::vector<float> calcPreFlopEquity(Hand* hands, int numHands, Deck& deck);
