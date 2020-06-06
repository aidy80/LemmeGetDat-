#pragma once

#include "Deck.h"

#define NUM_FLOP_CARDS 3
#define NUM_POOL_CARDS 5
#define NUM_CARD_NUMBERS 13
#define NUM_SUITS 4
#define MAX_HANDS 6
#define NUM_CARDS_IN_DECK 52

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

enum class Phase
{
	PREFLOP, FLOP, TURN, RIVER
};

//Representation of a given hand
struct Hand
{
	Card cards[2];
	bool folded;

	Hand(Deck& deck) : folded(false), cards{deck.getNextHandCard(), deck.getNextHandCard()} {}

	Hand(Card* initHand) : folded(false), cards{ initHand[0], initHand[1] } {}
	Hand(CardEnum* initHand) : folded(false), cards{ { getCardsNumber(initHand[0]), getCardsSuit(initHand[0]) },
												     { getCardsNumber(initHand[1]), getCardsSuit(initHand[1]) } } {}

	Hand() : folded(false), cards() {}

	void setHand(Card* newHand) {
		cards[0] = newHand[0];
		cards[1] = newHand[1];
	}

	void setHand(CardEnum* newHand) {
		cards[0].number = getCardsNumber(newHand[0]);
		cards[1].number = getCardsNumber(newHand[1]);

		cards[0].suit = getCardsSuit(newHand[0]);
		cards[1].suit = getCardsSuit(newHand[1]);
	}
	
	void setHand(Card newCard1, Card newCard2) {
		cards[0] = newCard1;
		cards[1] = newCard2;
	}

	void newHand(Deck& deck) 
	{
		//Make sure that resetEntireDeck was called before this
		cards[0] = deck.getNextHandCard();
		cards[0] = deck.getNextHandCard();
	}
};

struct PoolCardNames
{
	Card flop[3];
	Card turn;
	Card river;
};

struct Pool {
	union
	{
		PoolCardNames names;
		Card cards[5];
	};

	Pool(Deck& d) : cards{ d.getNextPoolCard(), d.getNextPoolCard(),
						  d.getNextPoolCard(), d.getNextPoolCard(), d.getNextPoolCard() } {}

	Pool(Card* initCards) : cards{initCards[0], initCards[1], initCards[2], initCards[3], initCards[4] } {}
	Pool(CardEnum* initCards) 
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i].number = getCardsNumber(initCards[i]);
			cards[i].suit = getCardsSuit(initCards[i]);
		}
	}

	Pool(const Pool& obj) 
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = obj.cards[i];
		}
	}

	Pool& operator=(const Pool& obj) 
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = obj.cards[i];
		}
		return *this;
	}

	Pool() : cards() {}

	void newPool(Deck& deck) {
		deck.resetPool();
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = deck.getNextPoolCard();
		}
	}

	void setCards(CardEnum* newCards) {
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i].number = getCardsNumber(newCards[i]);
			cards[i].suit = getCardsSuit(newCards[i]);
		}
	}

	void setCards(Card* newCards) {
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = newCards[i];
		}
	}

};

struct Player {
	unsigned int id;
	unsigned int stack;
};

class Table
{
private:
	Deck deck;
	Player* players;
	Phase phase;
	Pool pool;
	Hand* hands;

public:
};

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

inline void printHand(Hand& hand)
{
	std::cout << "Hand: ";
	std::cout << getCardsString(hand.cards[0]) << ", ";
	std::cout << getCardsString(hand.cards[1]) << std::endl;;
}
