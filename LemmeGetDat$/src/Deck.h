#pragma once

#include "Card.h"
#include "Random.h"

#define UNSHUFFLED_DECK   CardEnum::TWD, CardEnum::THD, CardEnum::FOD, CardEnum::FID, CardEnum::SID, CardEnum::SED, CardEnum::EID, \
						  CardEnum::NID, CardEnum::TED, CardEnum::JAD, CardEnum::QUD, CardEnum::KID, CardEnum::ACD,\
						  CardEnum::TWH, CardEnum::THH, CardEnum::FOH, CardEnum::FIH, CardEnum::SIH, CardEnum::SEH, CardEnum::EIH, \
						  CardEnum::NIH, CardEnum::TEH, CardEnum::JAH, CardEnum::QUH, CardEnum::KIH, CardEnum::ACH, \
						  CardEnum::TWC, CardEnum::THC, CardEnum::FOC, CardEnum::FIC, CardEnum::SIC, CardEnum::SEC, CardEnum::EIC, \
						  CardEnum::NIC, CardEnum::TEC, CardEnum::JAC, CardEnum::QUC, CardEnum::KIC, CardEnum::ACC, \
						  CardEnum::TWS, CardEnum::THS, CardEnum::FOS, CardEnum::FIS, CardEnum::SIS, CardEnum::SES, CardEnum::EIS, \
						  CardEnum::NIS, CardEnum::TES, CardEnum::JAS, CardEnum::QUS, CardEnum::KIS, CardEnum::ACS 

constexpr unsigned char NUM_CARDS_IN_DECK = 52;

class Deck 
{
private:
	CardEnum cardsLeft[NUM_CARDS_IN_DECK];
	unsigned char numCardsLeft;
	unsigned char numCardsAfterHands;
	Random generator;

	CardEnum getNextCard();
public:
	Deck();
	//Deck(const Deck&);
	Deck(unsigned int initSeed);

	void changeSeed(int newSeed);
	void resetEntireDeck();
	void resetPool();
	Card getNextHandCard();
	Card getNextPoolCard();

	void printDeck();
};

inline void Deck::changeSeed(int newSeed) 
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
