#pragma once

#include "Card.h"
#include "Random.h"

/*An unshuffed deck in CardEnum form - used to initialize deck*/
#define UNSHUFFLED_DECK   CardEnum::TWD, CardEnum::THD, CardEnum::FOD, CardEnum::FID, CardEnum::SID, CardEnum::SED, CardEnum::EID, \
						  CardEnum::NID, CardEnum::TED, CardEnum::JAD, CardEnum::QUD, CardEnum::KID, CardEnum::ACD,\
						  CardEnum::TWH, CardEnum::THH, CardEnum::FOH, CardEnum::FIH, CardEnum::SIH, CardEnum::SEH, CardEnum::EIH, \
						  CardEnum::NIH, CardEnum::TEH, CardEnum::JAH, CardEnum::QUH, CardEnum::KIH, CardEnum::ACH, \
						  CardEnum::TWC, CardEnum::THC, CardEnum::FOC, CardEnum::FIC, CardEnum::SIC, CardEnum::SEC, CardEnum::EIC, \
						  CardEnum::NIC, CardEnum::TEC, CardEnum::JAC, CardEnum::QUC, CardEnum::KIC, CardEnum::ACC, \
						  CardEnum::TWS, CardEnum::THS, CardEnum::FOS, CardEnum::FIS, CardEnum::SIS, CardEnum::SES, CardEnum::EIS, \
						  CardEnum::NIS, CardEnum::TES, CardEnum::JAS, CardEnum::QUS, CardEnum::KIS, CardEnum::ACS 

constexpr int NUM_CARDS_IN_DECK = 52;

class Deck 
{
private:
	CardEnum cardsLeft[NUM_CARDS_IN_DECK];
	int numCardsLeft;
	int numCardsAfterHands;

	/*Generator used to grab a random card from those remaining*/
	Random generator;

	/*Used to get the next card in the remaining deck randomly*/
	CardEnum getNextCard();
public:
	/*Initialize deck and random number generator to either time null or a desired seed*/
	Deck();
	Deck(uint32_t initSeed);

	/*Change the RNG seed manually*/
	void changeSeed(uint32_t newSeed);

	/*Bring all of the cards back into the deck*/
	void resetEntireDeck();

	/*Bring all of the pool cards back into the deck*/
	void resetPool();

	/*Deal out a hand card*/
	Card getNextHandCard();

	/*Deal out a pool card*/
	Card getNextPoolCard();

	/*Print the cards in the deck*/
	void printDeck();
};

inline void Deck::changeSeed(uint32_t newSeed) 
{
	generator.changeSeed(newSeed);
}

inline Card Deck::getNextPoolCard() {
	CardEnum nextCard = getNextCard();
	Card newCard(nextCard);
	return newCard;
}

inline Card Deck::getNextHandCard() {
	numCardsAfterHands--;
	CardEnum nextCard = getNextCard();
	Card newCard(nextCard);
	return newCard;
}

inline void Deck::resetEntireDeck() 
{
	numCardsLeft = NUM_CARDS_IN_DECK;
	numCardsAfterHands = NUM_CARDS_IN_DECK;
}

inline void Deck::resetPool()
{
	numCardsLeft = numCardsAfterHands;
}
