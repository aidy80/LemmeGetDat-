#pragma once
#include "pch.h"



//Representation of all cards in the deck
enum class Card
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
inline unsigned char getCardsNumber(Card card)
{
	return (unsigned char)card % 13;
}

/*
Obtain the suit of a passed enum card. A diamond will translate to 0, a heart to 1, 
club to 2, spade to 3

Params - card: the enum representation of the card
Return - the card's suit
*/
inline char getCardsSuit(Card card)
{
	return (char)card / 13;
}

/* 
Print a string representation of a given enum card

Params - card: the enum representation of the card to be decoded
Return - The representation of the card in string form
*/
std::string getCardsString(Card card);


