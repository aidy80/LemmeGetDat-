#include "PokerCards.h"

void printPool(const Pool& pool, int numCards) 
{
	std::cout << "Pool cards: ";
	if (numCards == 0) {
		std::cout << "Preflop, so none" << std::endl;
	}
	else {
		for (int i = 0; i < numCards - 1; i++)
		{
			std::cout << getCardsString(pool.cards[i]) << ", ";
		}
		std::cout << getCardsString(pool.cards[numCards - 1]) << std::endl;
	}
}

void printHand(const Hand& hand)
{
	std::cout << "Hand: ";
	std::cout << getCardsString(hand.cards[0]) << ", ";
	std::cout << getCardsString(hand.cards[1]) << std::endl;
}
