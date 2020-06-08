#pragma once

#include "Deck.h"
#include "PokerCards.h"

constexpr int MAX_HANDS = 6;

constexpr char P1 = 0x1;
constexpr char P2 = 0x2;
constexpr char P3 = 0x4;
constexpr char P4 = 0x8;
constexpr char P5 = 0x10;
constexpr char P6 = 0x20;

constexpr char PLAYER_BIT_PACK[6] = { P1, P2, P3, P4, P5, P6 };

/*Representation of the current phase of a game*/
enum class Phase
{
	PREFLOP, FLOP, TURN, RIVER
};

/*Implementation of the "Table". 
Holds everything: the hands, the players stacks, 
and the current board state at any time*/
class Table
{
private:
	Hand* hands;
	bool* hasFolded;

	int currTurn;
	int dealer;
	unsigned short* stacks;
	unsigned short* bets;
	unsigned short pot;

	Pool pool;
	Deck deck;

	Random generator;

	Phase phase;
public:
};
