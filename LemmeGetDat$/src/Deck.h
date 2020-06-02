#pragma once

#include "Card.h"
#include "Random.h"

#define UNSHUFFLED_DECK   Card::TWD, Card::THD, Card::FOD, Card::FID, Card::SID, Card::SED, Card::EID, \
						  Card::NID, Card::TED, Card::JAD, Card::QUD, Card::KID, Card::ACD,\
						  Card::TWH, Card::THH, Card::FOH, Card::FIH, Card::SIH, Card::SEH, Card::EIH, \
						  Card::NIH, Card::TEH, Card::JAH, Card::QUH, Card::KIH, Card::ACH, \
						  Card::TWC, Card::THC, Card::FOC, Card::FIC, Card::SIC, Card::SEC, Card::EIC, \
						  Card::NIC, Card::TEC, Card::JAC, Card::QUC, Card::KIC, Card::ACC, \
						  Card::TWS, Card::THS, Card::FOS, Card::FIS, Card::SIS, Card::SES, Card::EIS, \
						  Card::NIS, Card::TES, Card::JAS, Card::QUS, Card::KIS, Card::ACS 

constexpr unsigned char NUM_CARDS_IN_DECK = 52;

class Deck 
{
private:
	Card cardsLeft[NUM_CARDS_IN_DECK];
	unsigned char numCardsLeft;
	unsigned char numCardsAfterHands;
	Random generator;

	Card getNextCard();
public:
	Deck();
	Deck(unsigned int initSeed);

	void resetEntireDeck();
	void resetPool();
	Card getNextHandCard();
	Card getNextPoolCard();

	void printDeck();
};

inline Card Deck::getNextPoolCard() {
	return getNextCard();
}

inline Card Deck::getNextHandCard() {
	numCardsAfterHands--;
	return getNextCard();
}

inline void Deck::resetEntireDeck() 
{
	numCardsLeft = NUM_CARDS_IN_DECK;
	numCardsAfterHands = 0;
}

inline void Deck::resetPool()
{
	numCardsLeft = numCardsAfterHands;
}
