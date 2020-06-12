#pragma once
#include "pch.h"

constexpr int NUM_CARD_NUMBERS = 13;
constexpr int NUM_SUITS = 4;

/*Representation of all cards in the deck as an enum. Used for cases of random number 
generation to pick a random card*/
enum class CardEnum
{
	TWD, THD, FOD, FID, SID, SED, EID, NID, TED, JAD, QUD, KID, ACD,
	TWH, THH, FOH, FIH, SIH, SEH, EIH, NIH, TEH, JAH, QUH, KIH, ACH,
	TWC, THC, FOC, FIC, SIC, SEC, EIC, NIC, TEC, JAC, QUC, KIC, ACC,
	TWS, THS, FOS, FIS, SIS, SES, EIS, NIS, TES, JAS, QUS, KIS, ACS,
	NONE
};

/*
Obtain the number of a passed enum card. A Two will translate to 0, a three to 1, 
..., a king to 11, an ace to 12

Params - card: the enum representation of the card
Return - the card's number
*/
inline int getCardsNumber(CardEnum card)
{
	return (int)(card) % 13;
}

/*
Obtain the suit of a passed enum card. A diamond will translate to 0, a heart to 1, 
club to 2, spade to 3

Params - card: the enum representation of the card
Return - the card's suit
*/
inline int getCardsSuit(CardEnum card)
{
	return (int)(card) / 13;
}

/* 
Print a string representation of a given enum card

Params - card: the enum representation of the card to be decoded
Return - The representation of the card in string form
*/
std::string getCardsString(CardEnum card);

/*Representation of a card using its number and suit in two separate fields. 
Better than CardEnum in that mathematical operations arent needed to extract num and suit*/
struct Card 
{
	/*Represents the cards number. 0 is a "2 of _" and 12 is the "Ace of _"*/
	int number;
	
	/*Represents the cards suit. 0 is diamonds, 1 is hearts, 2 is clubs, 3 is spades*/
	int suit;

	/*Represents when there is no card present*/
	static const Card NULL_CARD;

	Card(CardEnum card) :
		number(getCardsNumber(card)), suit(getCardsSuit(card)) {}

	Card(int initNum, int initSuit) :
		number(initNum), suit(initSuit) {}

	Card() : number(-1), suit(-1) {}

	bool operator!=(const Card other) const
	{
		return (number != other.number) || (suit != other.suit);
	}
};

/* 
Print a string representation of a given card

Params - card: the card struct representation of the card to be decoded
Return - The representation of the card in string form
*/
std::string getCardsString(Card card);

/*Convert from a Card representation to a CardEnum representation*/
inline CardEnum CardToEnum(Card card) 
{
	return (CardEnum)(card.number + card.suit * NUM_CARD_NUMBERS);
}
