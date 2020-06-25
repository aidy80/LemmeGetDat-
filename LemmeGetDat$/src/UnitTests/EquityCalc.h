#pragma once

#include "../pch.h"
#include "../Table.h"
#include "../BestHand.h"
#include "../Ranged2DArray.h"

/*Calcuate the preflop equatily for a "numHands" number of hands. 
Requires a deck with the hands already dealt to generate random pool cards*/
std::vector<float> calcPreFlopEquity(Hand* hands, int numHands, Deck& deck);
