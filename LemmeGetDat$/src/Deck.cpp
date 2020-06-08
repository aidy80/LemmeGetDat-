#include "Deck.h"

Deck::Deck() :
	generator((unsigned int)time(NULL)), numCardsLeft(NUM_CARDS_IN_DECK), 
	cardsLeft{UNSHUFFLED_DECK}, numCardsAfterHands(NUM_CARDS_IN_DECK) {}

Deck::Deck(unsigned int initSeed) :
	generator(initSeed), numCardsLeft(NUM_CARDS_IN_DECK), 
	cardsLeft{UNSHUFFLED_DECK}, numCardsAfterHands(NUM_CARDS_IN_DECK) {}

CardEnum Deck::getNextCard()
{
	unsigned int randomNum = generator.getRandomNum(numCardsLeft);

#ifdef _DEBUG
	assert(numCardsLeft != 0);
#endif

	CardEnum newCard = cardsLeft[randomNum];
	cardsLeft[randomNum] = cardsLeft[numCardsLeft - 1];
	cardsLeft[numCardsLeft - 1] = newCard;

	numCardsLeft--;
	return newCard;
}

void Deck::printDeck() 
{
	for (int i = 0; i < numCardsLeft; i++) {
		std::cout << (int)cardsLeft[i] << " ";
	}
	std::cout << std::endl;
}
