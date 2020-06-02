#pragma once

#include "pch.h"
#include "Table.h"

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

void getBestHands(Pool& pool, Hand* hands, unsigned char numHands, char* bestHands);
