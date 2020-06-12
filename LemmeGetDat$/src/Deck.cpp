#include "Deck.h"

Deck::Deck() :
	generator((uint32_t)time(NULL)), numCardsLeft(NUM_CARDS_IN_DECK), 
	cardsLeft{UNSHUFFLED_DECK}, numCardsAfterHands(NUM_CARDS_IN_DECK) {}

Deck::Deck(uint32_t initSeed) :
	generator(initSeed), numCardsLeft(NUM_CARDS_IN_DECK), 
	cardsLeft{UNSHUFFLED_DECK}, numCardsAfterHands(NUM_CARDS_IN_DECK) {}

CardEnum Deck::getNextCard()
{
	uint32_t randomNum = generator.getRandomNum(numCardsLeft);

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
