#pragma once
#include "pch.h"

#define NUM_FLOP_CARDS 3
#define NUM_POOL_CARDS 5
#define NUM_CARD_NUMBERS 13
#define NUM_SUITS 4
#define MAX_HANDS 6

#define FLOP1_FLAG 0x1
#define FLOP2_FLAG 0x2
#define FLOP3_FLAG 0x4
#define TURN_FLAG 0x8
#define RIVER_FLAG 0x10

#define P1 0x1
#define P2 0x2
#define P3 0x4
#define P4 0x8
#define P5 0x10
#define P6 0x20

const char POOL_BIT_PACK[5] = { FLOP1_FLAG, FLOP2_FLAG, FLOP3_FLAG, TURN_FLAG, RIVER_FLAG };
const char PLAYER_BIT_PACK[6] = { P1, P2, P3, P4, P5, P6 };

//Representation of all cards in the deck
enum class Card
{
	TWD, THD, FOD, FID, SID, SED, EID, NID, TED, JAD, QUD, KID, ACD,
	TWH, THH, FOH, FIH, SIH, SEH, EIH, NIH, TEH, JAH, QUH, KIH, ACH,
	TWC, THC, FOC, FIC, SIC, SEC, EIC, NIC, TEC, JAC, QUC, KIC, ACC,
	TWS, THS, FOS, FIS, SIS, SES, EIS, NIS, TES, JAS, QUS, KIS, ACS,
	NONE
};

enum class Phase
{
	PREFLOP, FLOP, TURN, RIVER
};

//Representation of a given hand
struct Hand
{
	Card cards[2];
	bool folded = false;
};

struct PoolCardNames
{
	Card flop[3];
	Card turn;
	Card river;
};

union Pool
{
	PoolCardNames names;
	Card cards[5];
};

struct SixTable
{
	Hand hands[6];
	Pool pool;
	unsigned int pots[6];
	unsigned char numHands;
	unsigned char dealer;
	Phase phase;
};

struct TwoTable
{
	Hand hands[2];
	unsigned int pots[2];
	unsigned char numHands;
	unsigned char dealer;
	Phase phase;
};

/*
Obtain the number of a passed enum card. A Two will translate to 0, a three to 1, 
..., a king to 11, an ace to 12

Params - card: the enum representation of the card
Return - the card's number
*/
inline unsigned char getCardsNumber(Card card)
{
	return (unsigned char)card % 13;
}

/*
Obtain the suit of a passed enum card. A diamond will translate to 0, a heart to 1, 
club to 2, spade to 3

Params - card: the enum representation of the card
Return - the card's suit
*/
inline char getCardsSuit(Card card)
{
	return (char)card / 13;
}

/*
Print the string representation of the pool

Params - pool: The pool to be printed
*/
inline void printPool(Pool& pool)
{
	std::cout << "Pool cards: ";
	for (int i = 0; i < NUM_FLOP_CARDS + 1; i++)
	{
		std::cout << getCardsString(pool.cards[i]) << ", ";
	}
	std::cout << getCardsString(pool.names.river) << std::endl;
}

/* 
Print a string representation of a given enum card

Params - card: the enum representation of the card to be decoded
Return - The representation of the card in string form
*/
std::string getCardsString(Card card);
