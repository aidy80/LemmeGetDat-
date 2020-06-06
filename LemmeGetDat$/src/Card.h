#pragma once
#include "pch.h"

//Representation of all cards in the deck
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
inline char getCardsNumber(CardEnum card)
{
	return (char)card % 13;
}

/*
Obtain the suit of a passed enum card. A diamond will translate to 0, a heart to 1, 
club to 2, spade to 3

Params - card: the enum representation of the card
Return - the card's suit
*/
inline char getCardsSuit(CardEnum card)
{
	return (char)card / 13;
}

/* 
Print a string representation of a given enum card

Params - card: the enum representation of the card to be decoded
Return - The representation of the card in string form
*/
std::string getCardsString(CardEnum card);


struct Card 
{
	char number;
	char suit;
	static const Card NULL_CARD;

	Card(CardEnum card) :
		number(getCardsNumber(card)), suit(getCardsSuit(card)) {}

	Card(char initNum, char initSuit) :
		number(initNum), suit(initSuit) {}

	Card() : number(-1), suit(-1) {}

	bool operator!=(Card other) 
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

inline CardEnum CardToEnum(Card card) 
{
	return (CardEnum)(card.number + card.suit * 13);
}
