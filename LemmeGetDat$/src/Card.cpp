#include "Card.h"

std::string getCardsString(Card card)
{
	char number = getCardsNumber(card);
	char suit = getCardsSuit(card);

	std::string strRep;

	switch (number)
	{
		case 0:
			strRep = "Two of ";
			break;
		case 1:
			strRep = "Three of ";
			break;
		case 2:
			strRep = "Four of ";
			break;
		case 3:
			strRep = "Five of ";
			break;
		case 4:
			strRep = "Six of ";
			break;
		case 5:
			strRep = "Seven of ";
			break;
		case 6:
			strRep = "Eight of ";
			break;
		case 7:
			strRep = "Nine of ";
			break;
		case 8:
			strRep = "Ten of ";
			break;
		case 9:
			strRep = "Jack of ";
			break;
		case 10:
			strRep = "Queen of ";
			break;
		case 11:
			strRep = "King of ";
			break;
		case 12:
			strRep = "Ace of ";
			break;
		default:
			strRep = "Card Not Found";
	}

	switch (suit)
	{
		case 0:
			strRep += "Diamonds";
			break;
		case 1:
			strRep += "Hearts";
			break;
		case 2:
			strRep += "Clubs";
			break;
		case 3:
			strRep += "Spades";
			break;
	}

	return strRep;
}