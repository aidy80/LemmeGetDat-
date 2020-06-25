#include "PokerCards.h"

void getPoolID(Pool& pool, Phase currPhase, std::bitset<INFO_KEY_SIZE>& infoKey) 
{
	int numPoolCards = numCardsInPhase(currPhase);

	int numHistSize = sizeof(int) * NUM_CARD_NUMBERS;
	int* numHist = (int*)_malloca(numHistSize);
	memset(numHist, 0, numHistSize);

	int suitHistSize = sizeof(int) * NUM_SUITS;
	int* suitHist = (int*)_malloca(suitHistSize);
	memset(suitHist, 0, suitHistSize);

	int maxSuit = 0;
	for (int i = 0; i < numPoolCards; i++) 
	{
		numHist[pool.cards[i].number]++;
		suitHist[pool.cards[i].suit]++;
		if (maxSuit < suitHist[pool.cards[i].suit])
		{
			maxSuit = suitHist[pool.cards[i].suit];
		}
	}

	int bitPos = 0;
	for (int i = 0; i < NUM_CARD_NUMBERS; i++, bitPos++)
	{
		while (numHist[i] != 0) 
		{
			infoKey[bitPos] = 1;
			bitPos++;
			numHist[i]--;
		}
	}

	if (currPhase == Phase::FLOP) 
	{
		infoKey[NUM_CARD_NUMBERS + numPoolCards] =
			maxSuit > 1 ? 1 : 0;
	} else if (currPhase == Phase::TURN) 
	{
		infoKey[NUM_CARD_NUMBERS + numPoolCards] =
			maxSuit > 2 ? 1 : 0;
	} 
	else 
	{
		infoKey[NUM_CARD_NUMBERS + numPoolCards] =
			maxSuit > 2 ? 1 : 0;
	}
	
	_freea(numHist);
	_freea(suitHist);
}

constexpr int sigma0(int end) 
{
#ifdef _DEBUG
	assert(end > -1);
#endif
	int sum = 0;
	for (int i = 1; i <= end; i++) 
	{
		sum += i;
	}
	return sum;
}

HandID getHandID(Hand& currHand) 
{
	int handID;
	int card1Num = currHand.cards[0].number <= currHand.cards[1].number ? currHand.cards[0].number : currHand.cards[1].number;
	int card2Num = card1Num == currHand.cards[0].number ? currHand.cards[1].number : currHand.cards[0].number;
	
	handID = card1Num * NUM_CARD_NUMBERS - sigma0(card1Num);
	handID += card2Num;

	if (currHand.cards[0].suit == currHand.cards[1].suit) 
	{
		handID += sigma0(13) - (card1Num + 1);
	}

	return (HandID)handID;
}

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
	std::cout << getCardsString(hand.cards[1]) << std::endl;;
}
