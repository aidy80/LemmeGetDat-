#pragma once

#include "pch.h"
#include "Table.h"
#include "TwoDimArray.h"

/*Represents the type of hand that a given player has. 
If one of the first 5 fields have a non-value, 
the kicker values refer to that hand*/
struct PokerHand
{
	bool straightFlush = false;
	bool flush = false;
	bool straight = false;
	char triple = -1;
	char pair[2] = {-1, -2};

	char kicker1 = -1;
	char kicker2 = -1;
};

inline void initPokerHand(PokerHand& pokerhand) {
	pokerhand.straightFlush = false;
	pokerhand.flush = false;
	pokerhand.straight = false;
	pokerhand.triple = -1;
	pokerhand.pair[0] = -1;
	pokerhand.pair[1] = -2;
	pokerhand.kicker1 = -1;
	pokerhand.kicker2 = -1;
}

enum class HandType 
{
	HIGH, PAIR, TWOPAIR, TRIPLE, STRAIGHT, FLUSH, HOUSE, QUADS, STRFLUSH
};

/*Given an array of hands, and a pool, obtain the winning poker hand(s).

Params: pool - the pool cards in the game (all the way to the river)
		allHands - an array of allHands on the table
		bestHands - a two dimensional array used to specify which players 
					are included in the pot and/or subpots. Each row in this 
					array designates a "pot". The players in the pot should 
					be included in the row. So if players 1 and 4 are in a pot, the first 
					row should look like
						1 4 -1 -1 -1 -1 
					Then, additional rows function in the same way. That is, each row will be 
					evaluated to determine who has the best hand. 
					
Return - The best hand(s) will be outputted in the bestHands 2Darray (the input array will be altered). 
		 Each row of the 2D array will contain the winning hands from that pot. So, if players 1 and 4 
		 tied for the first pot, but player 3 won the subpot, we would see the output as
				1 4 -1 -1 -1 -1
				3 -1 -1 -1 -1 -1
						*/
void getBestHands(Pool& pool, Hand* allHands, TwoDimArray& bestHands);
