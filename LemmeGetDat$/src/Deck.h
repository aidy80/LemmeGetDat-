#pragma once

#include "Card.h"
#include "Random.h"

class Deck 
{
private:
	Card cards[52];
	unsigned char numCardsLeft;
public:
	Deck();
	~Deck();

	Card getNextCard();
	void resetDeck();
};

inline Card randomCard()
{
	static unsigned char numCalls = 0;
	static unsigned int currRand;
	if ((numCalls & 0x3) == 0x0) {
	//if ((numCalls & 0xF) == 0x0) {
		currRand = fast_rand();
		//std::cout << "Replace\n";
	}
	else {
		currRand = currRand >> 6;
		//currRand = currRand >> 1;
	}

	//std::cout << "currRand: " << currRand << "\n";

	numCalls++;
	return (Card)(currRand % 52);
}

inline void createRandomHand(Hand& hand)
{
	hand.cards[0] = randomCard();
	hand.cards[1] = randomCard();
}

inline void createRandomPoolCards(Pool& pool)
{
	for (int i = 0; i < NUM_POOL_CARDS; i++)
	{
		pool.cards[i] = randomCard();
	}
}


