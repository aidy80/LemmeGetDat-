#include "BestHand.h"

#define CHECK_BEST_HAND(bitmap, handType) \
	if (bitmap) { \
			fillBestHands(bestHands, bitmap, handType, pokerhands, numHands); \
			return; \
	}

//std::cout << "Best hand:" << (int)handType << std::endl; \

inline bool SuitRequirement(Card card, char flushSuit) { 
	return ((flushSuit == -1) || (flushSuit == getCardsSuit(card)));
}

inline bool FitsInLowest(Card usedCard, Card handCard, char lowestCard) {
	return (usedCard != handCard && getCardsNumber(handCard) == (lowestCard - 1));
}

inline bool FitsInHighest(Card usedCard, Card handCard, char highestCard) {
	return (usedCard != handCard && getCardsNumber(handCard) == (highestCard + 1));
}

inline bool isHandCard(unsigned char firstNum, unsigned char secNum, unsigned char cardNum) {
	return (firstNum == cardNum || secNum == cardNum);
}

inline bool isQuads(PokerHand* pokerhands, unsigned char k) {
	return (pokerhands[k].pair[0] == pokerhands[k].pair[1]);
}

inline unsigned char isWinning(unsigned char winningHands, unsigned char k) {
	return (unsigned char)((bool)(winningHands & PLAYER_BIT_PACK[k]));
}

std::pair<char, char> getKickers(Card card1, Card card2, Pool& pool, unsigned char includeMap) 
{
	std::pair<char, char> kickers;

	unsigned char card1Less = 0;
	unsigned char card2Less = 0;

	unsigned char cardNum1 = getCardsNumber(card1);
	unsigned char cardNum2 = getCardsNumber(card2);

	bool card1NotNONE = (card1 != Card::NONE);
	bool card2NotNONE = (card2 != Card::NONE);

	for (int i = 0; i < NUM_POOL_CARDS; i++)
	{	
		if (includeMap & POOL_BIT_PACK[i]) {
			if (card1NotNONE && getCardsNumber(pool.cards[i]) > cardNum1) {
				card1Less++;
			}
			if (card2NotNONE && getCardsNumber(pool.cards[i]) > cardNum2) {
				card2Less++;
			}
		}
	}

	if (card1NotNONE && card2NotNONE)
	{
		if (cardNum1 < cardNum2) {
			kickers.first = (card2Less < 5) ? cardNum2 : -1;
			kickers.second = (++card1Less < 5) ? cardNum1 : -1;
		}
		else if (cardNum1 > cardNum2) {
			kickers.first = (card1Less < 5) ? cardNum1 : -1;
			kickers.second = (++card2Less < 5) ? cardNum2 : -1;
		}
	} else if (card1NotNONE && !card2NotNONE) {
		kickers.first = (card1Less < 5) ? cardNum1 : -1;
		kickers.second = -1;
	} else if (!card1NotNONE && card2NotNONE) {
		kickers.first = (card2Less < 5) ? cardNum2 : -1;
		kickers.second = -1;
	} else {
		kickers.first = -1; kickers.second = -1;
	}

	return kickers;
}

char possibleFlush(Pool& pool, unsigned char* suitHist, unsigned char& includeMap)
{
	unsigned char i;
	char* cardSuits = (char*)alloca(sizeof(char) * NUM_POOL_CARDS);
	for (i = 0; i < NUM_POOL_CARDS; i++)
	{
		cardSuits[i] = getCardsSuit(pool.cards[i]);
		suitHist[cardSuits[i]]++;
	}
	
	char flushSuit = -1;
	for (char i = 0; i < NUM_SUITS; i++)
	{
		if (suitHist[i] > 2)
		{
			flushSuit = i;
			break;
		}
	}
	
	if (flushSuit == -1) {
		return -1;
	}

	for (int i = 0; i < NUM_POOL_CARDS; i++)
	{
		if (cardSuits[i] == flushSuit) 
		{
			includeMap |= POOL_BIT_PACK[i];
		}
	}

	return flushSuit;
}

char checkFlush(Pool& pool, Hand* hands, unsigned char numHands, PokerHand* pokerhands)
{
	unsigned char suitHist[NUM_SUITS];
	for (int i = 0; i < NUM_SUITS; i++) {
		suitHist[i] = 0;
	}

	unsigned char includeMap = 0; 
	char flushSuit = possibleFlush(pool, suitHist, includeMap);

	if (flushSuit != -1) 
	{
		for (unsigned char i = 0; i < numHands; i++)
		{
			unsigned char firstSuit = getCardsSuit(hands[i].cards[0]);
			unsigned char secSuit = getCardsSuit(hands[i].cards[1]);
			
			if ( (suitHist[flushSuit] + (firstSuit == flushSuit) + (secSuit == flushSuit)) > 4 ) 
			{
				pokerhands[i].flush = true;
			
				std::pair<char, char> kickers;

				if (firstSuit == flushSuit) {
					if (secSuit == flushSuit) {
						kickers = getKickers(hands[i].cards[0], hands[i].cards[1], pool, includeMap);
					} else {
						kickers = getKickers(hands[i].cards[0], Card::NONE, pool, includeMap);
					}
				} else if (secSuit == flushSuit) {
					kickers = getKickers(Card::NONE, hands[i].cards[1], pool, includeMap);
				} else {
					kickers = { -1, -1 };
				}

				pokerhands[i].kicker1 = kickers.first;
				pokerhands[i].kicker2 = kickers.second;
			}
			else {
				pokerhands[i].flush = false;
			}
		}			
	}
	return flushSuit;
}

void sortPoolAndHands(Pool& pool, Hand* hands, unsigned char numHands) 
{
	std::sort(pool.cards, pool.cards + NUM_POOL_CARDS,
		[](Card card1, Card card2) {
			return getCardsNumber(card1) < getCardsNumber(card2);
		   });

	for (unsigned char i = 0; i < numHands; i++) 
	{
		if (getCardsNumber(hands[i].cards[0]) > getCardsNumber(hands[i].cards[1])) 
		{
			Card temp = hands[i].cards[0];
			hands[i].cards[0] = hands[i].cards[1];
			hands[i].cards[1] = temp;
		}
	}
}

void testPoolStraight(Pool& pool, unsigned char startCardIndex, unsigned char endCardIndex, Hand* hands, unsigned char numHands, PokerHand* pokerhands, char flushSuit)
{	
	for (int k = 0; k < numHands; k++) {
		bool gotoNext = false;
		if (flushSuit != -1) {
			if (endCardIndex - startCardIndex + 1 + (getCardsSuit(hands[k].cards[0]) == flushSuit)
				+ (getCardsSuit(hands[k].cards[1]) == flushSuit) < 5) 
			{
				gotoNext = true;
			}
		}

		Card usedHandCards[2] = { Card::NONE, Card::NONE };
		unsigned char numUsedHandCards = 0;
		int lowestCard = getCardsNumber((pool).cards[startCardIndex]);
		int highestCard = getCardsNumber((pool).cards[endCardIndex]);
		int numStraightCards = highestCard - lowestCard + 1;

		for (int i = startCardIndex; !gotoNext && i < endCardIndex; i++)
		{
			int poolSpacing = getCardsNumber((pool).cards[i + 1]) - getCardsNumber((pool).cards[i]) - 1;
			for (int j = 0; j < poolSpacing; j++) {
				int neededCard = getCardsNumber((pool).cards[i]) + j + 1;
				if ((getCardsNumber(hands[k].cards[0]) == neededCard) && SuitRequirement(hands[k].cards[0], flushSuit)) {
					usedHandCards[numUsedHandCards] = hands[k].cards[0];
				}
				else if ((getCardsNumber(hands[k].cards[1]) == neededCard) && SuitRequirement(hands[k].cards[1], flushSuit)) {
					usedHandCards[numUsedHandCards] = hands[k].cards[1];
				}
				else {
					gotoNext = true;
					continue;
				}
				numUsedHandCards++;
			}
		}
	
		if (!gotoNext) {
			while (numUsedHandCards < 2)
			{
				if (FitsInLowest(usedHandCards[0], hands[k].cards[0], lowestCard))
				{
					lowestCard = getCardsNumber(hands[k].cards[0]);
					numStraightCards++;
					usedHandCards[0] = hands[k].cards[0];
				} 
				else if (FitsInLowest(usedHandCards[0], hands[k].cards[1], lowestCard)) {
					lowestCard = getCardsNumber(hands[k].cards[1]);
					numStraightCards++;
					usedHandCards[0] = hands[k].cards[1];
				} 
				else if (FitsInHighest(usedHandCards[0], hands[k].cards[0], highestCard)) {
					highestCard = getCardsNumber(hands[k].cards[0]);
					numStraightCards++;
					usedHandCards[0] = hands[k].cards[0];
				} 
				else if (FitsInHighest(usedHandCards[0], hands[k].cards[1], highestCard)) {
					highestCard = getCardsNumber(hands[k].cards[1]);
					numStraightCards++;
					usedHandCards[0] = hands[k].cards[1];
				}
				else {
					break;
				}
				numUsedHandCards++;
			}

			if (numStraightCards > 4) {
				if (flushSuit == -1) {
					pokerhands[k].straight = true;
				}
				else {
					pokerhands[k].straightFlush = true;
				}
				if (pokerhands[k].kicker1 < highestCard) {
					pokerhands[k].kicker1 = highestCard;
				}
			}
		}
	}
}

void checkStraight(Pool& originalPool, Hand* hands, unsigned char numHands, PokerHand* pokerhands, char flushSuit)
{
	unsigned char numPoolCards = 0;
	Pool appliedPool; //Change to card array?
	if (flushSuit != -1) {
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			if (getCardsSuit(originalPool.cards[i]) == flushSuit) {
				appliedPool.cards[numPoolCards] = originalPool.cards[i];
				numPoolCards++;
			}
		}
	}
	else {
		appliedPool.cards[0] = originalPool.cards[0];
		numPoolCards++;
		for (int i = 1; i < NUM_POOL_CARDS; i++) {
			if (getCardsNumber(originalPool.cards[i]) != getCardsNumber(appliedPool.cards[numPoolCards - 1])) {
				appliedPool.cards[numPoolCards] = originalPool.cards[i];
				numPoolCards++;
			}
		}
	}

	unsigned char* poolSpacing = (unsigned char*)alloca(sizeof(unsigned char) * (numPoolCards - 1));
	for (int i = 0; i < numPoolCards - 1; i++) 
	{
		poolSpacing[i] = getCardsNumber(appliedPool.cards[i + 1]) - getCardsNumber(appliedPool.cards[i]);
	}

	 
	for (int i = 0; i < numPoolCards - 2; i++) {
		unsigned char totalPoolSpacing = 0;
		for (int s = 2; s < (numPoolCards - i); s++) {
			for (int j = i; j < i + s; j++) {
				totalPoolSpacing += poolSpacing[j] - 1;
			}
			if (totalPoolSpacing < 3) {
				testPoolStraight(appliedPool, i, i + s, hands, numHands, pokerhands, flushSuit);
			}
		}
	}
}

void checkHighPairTripQuad(Pool& pool, Hand* hands, unsigned char numHands, PokerHand* pokerhands)
{
	unsigned char* numHist = (unsigned char*)_malloca(sizeof(unsigned char) * NUM_CARD_NUMBERS);
	for (int i = 0; i < NUM_CARD_NUMBERS; i++) {
		numHist[i] = 0;
	}

	for (int i = 0; i < NUM_POOL_CARDS; i++) {
		numHist[getCardsNumber(pool.cards[i])]++;
	}

	for (int k = 0; k < numHands; k++) {
		unsigned char firstNum = getCardsNumber(hands[k].cards[0]);
		unsigned char secNum = getCardsNumber(hands[k].cards[1]);
		numHist[firstNum]++;
		numHist[secNum]++;
		char tempKicker1 = -1;
		char tempKicker2 = -1;
		char unusedCombo = -1;

		for (int i = 0; i < NUM_CARD_NUMBERS; i++) {
			if (numHist[i] == 2)
			{
				if (!isQuads(pokerhands, k))
				{
					if (pokerhands[k].pair[1] > -1)
					{
						if (isHandCard(firstNum, secNum, pokerhands[k].pair[1])) {
							tempKicker1 = pokerhands[k].pair[1];
						}
						else {
							unusedCombo = pokerhands[k].pair[1];
						}
					}
					pokerhands[k].pair[1] = pokerhands[k].pair[0];
					pokerhands[k].pair[0] = i;
				}
				else if (isHandCard(firstNum, secNum, i)) {
					tempKicker1 = i;
				}
				else {
					unusedCombo = i;
				}
			}
			else if (numHist[i] == 3) 
			{
				if (isQuads(pokerhands, k) && isHandCard(firstNum, secNum, i))
				{
					tempKicker1 = i;
				}
				else {
					if (pokerhands[k].triple != -1) {
						pokerhands[k].pair[0] = pokerhands[k].triple;
					}
					pokerhands[k].triple = i;
				}
			}
			else if (numHist[i] == 4) 
			{
				if (pokerhands[k].triple != -1 && isHandCard(firstNum, secNum, pokerhands[k].triple)) {
					tempKicker1 = pokerhands[k].triple;
				}
				else if (pokerhands[k].pair[0] != -1){
					if (isHandCard(firstNum, secNum, pokerhands[k].pair[0])) {
						tempKicker1 = pokerhands[k].pair[0];
					}
					else {
						unusedCombo = pokerhands[k].pair[0];
					}
				}
				pokerhands[k].pair[0] = i;
				pokerhands[k].pair[1] = i;
			}
		}

		if (isQuads(pokerhands, k) || (!(pokerhands[k].flush || pokerhands[k].straight))) {
			int i;
			int numUsed = 0;
			
			if (isQuads(pokerhands, k)) {
				numUsed = 4;
			}
			else if (pokerhands[k].triple > -1) {
				numUsed = 3;
			}
			else if (pokerhands[k].pair[1] > -1) {
				numUsed = 4;
			}
			else if (pokerhands[k].pair[0] > -1) {
				numUsed = 2;
			}

			for (i = NUM_CARD_NUMBERS - 1; numUsed < 5 && i > -1; i--)
			{
				if (numHist[i] == 1) {
					if (isHandCard(firstNum, secNum, i)) {
						if (i > tempKicker1) {
							tempKicker2 = tempKicker1;
							tempKicker1 = i;
						}
						else if (i > tempKicker2) {
							tempKicker2 = i;
						}
					}
					numUsed++;
				}
				else if (i == unusedCombo) {
					numUsed++;
				}
			}

			if (++i > tempKicker1) {
				tempKicker1 = -1;
				tempKicker2 = -1;
			}

			if (isQuads(pokerhands, k)) {
				tempKicker2 = -1;
			}
			else if (pokerhands[k].triple != -1 && pokerhands[k].pair[0] != -1) {
				tempKicker1 = pokerhands[k].triple;
				tempKicker2 = pokerhands[k].pair[0];
			}
			else if (pokerhands[k].pair[1] > -1) {
				tempKicker2 = -1;
			}

			pokerhands[k].kicker1 = tempKicker1;
			pokerhands[k].kicker2 = tempKicker2;
		}

		numHist[secNum]--;
		numHist[firstNum]--;
	}
}

struct WinningHand {
	unsigned char player;
	unsigned char winning;
};

void fillBestHands(char* bestHands, unsigned char winningHands, HandType winHandType, PokerHand* pokerhands, unsigned int numHands)
{
	WinningHand* handsWithIndex = (WinningHand*)alloca(sizeof(handsWithIndex) * numHands);
	char highestHand[2] = { -1, -1 };
	for (unsigned char k = 0; k < numHands; k++)
	{
		handsWithIndex[k].player = k;

		if (isWinning(winningHands, k)) {
			switch (winHandType) {
			case HandType::PAIR:
				if (highestHand[0] < pokerhands[k].pair[0]) {
					highestHand[0] = pokerhands[k].pair[0];
				}
				break;
			case HandType::TWOPAIR:
				if (highestHand[0] < pokerhands[k].pair[0]) {
					highestHand[0] = pokerhands[k].pair[0];
				}
				if (highestHand[0] == pokerhands[k].pair[0] && highestHand[1] < pokerhands[k].pair[1]) {
					highestHand[1] = pokerhands[k].pair[1];
				}
				break;
			case HandType::TRIPLE:
				if (highestHand[0] < pokerhands[k].triple) {
					highestHand[0] = pokerhands[k].triple;
				}
				break;
			case HandType::QUADS:
				if (highestHand[0] < pokerhands[k].pair[0]) {
					highestHand[0] = pokerhands[k].pair[0];
				}
				break;
			}
		}
	}

	for (unsigned char k = 0; k < numHands; k++) {
		if (isWinning(winningHands, k)) {
			switch (winHandType) {
			case HandType::PAIR:
				handsWithIndex[k].winning = (unsigned char)(pokerhands[k].pair[0] == highestHand[0]);
				break;
			case HandType::TWOPAIR:
				handsWithIndex[k].winning = (unsigned char)(pokerhands[k].pair[0] == highestHand[0] &&
					pokerhands[k].pair[1] == highestHand[1]);
				break;
			case HandType::TRIPLE:
				handsWithIndex[k].winning = (unsigned char)(pokerhands[k].triple == highestHand[0]);
				break;
			case HandType::QUADS:
				handsWithIndex[k].winning = (unsigned char)(pokerhands[k].pair[0] == highestHand[0]);
				break;
			default:
				handsWithIndex[k].winning = 1;
			}
		} 
		else {
			handsWithIndex[k].winning = 0;
		}
	}

	std::sort(handsWithIndex, handsWithIndex + numHands,
		[pokerhands](WinningHand& val1, WinningHand& val2) -> bool
		{
			if (val1.winning == 1 && val2.winning == 1) {
				if (pokerhands[val1.player].kicker1 > pokerhands[val2.player].kicker1) 
				{
					val2.winning = 0;
					return true;
				}
				else if (pokerhands[val1.player].kicker1 < pokerhands[val2.player].kicker1) {
					val1.winning = 0;
					return false;
				}
				else {
					if (pokerhands[val1.player].kicker2 == pokerhands[val2.player].kicker2) 
					{
						return val1.player < val2.player;
					}
					else if (pokerhands[val1.player].kicker2 > pokerhands[val2.player].kicker2) {
						val2.winning = 0;
						return true;
					} else if (pokerhands[val1.player].kicker2 < pokerhands[val2.player].kicker2) {
						val1.winning = 0;
						return false;
					}	
				}
			} else if (val1.winning == 1) {
				return true;
			} else if (val2.winning == 1) {
				return false;
			} 
			return val1.player < val2.player;
		});
	
	for (unsigned int i = 0; i < numHands; i++)
	{
		bestHands[i] = handsWithIndex[i].winning ? handsWithIndex[i].player : -1;
	}
}

void getBestHands(Pool& pool, Hand* hands, unsigned char numHands, char* bestHands)
{
	//TODO::Account for split pots. To do so, make bestHands a 2D array 
	//(to be implemented as a 1D array with the ability to sort individual subgames)
	//Additionally, take an extra parameter which shows which hands are to be taken 
	//account of in each subgame. This will also be a 2D array which indices showing which 
	//players are in which subgame (i.e. subpot)

	//char* bestHands = new char[numHands];
	std::fill_n(bestHands, numHands, -1);

	PokerHand* pokerhands = (PokerHand*)alloca(sizeof(PokerHand) * numHands);
	for (int i = 0; i < numHands; i++) {
		initPokerHand(pokerhands[i]);
	}

	sortPoolAndHands(pool, hands, numHands);

	unsigned char flushHands = 0, straightHands = 0, quadHands = 0, tripleHands = 0,
				  twoPairHands = 0, pairHands = 0, straightFlushHands = 0, houseHands = 0;

	checkStraight(pool, hands, numHands, pokerhands, -1);
	char flushSuit = checkFlush(pool, hands, numHands, pokerhands);
	for (unsigned int i = 0; i < numHands; i++) 
	{
		flushHands |= PLAYER_BIT_PACK[i] * (char)pokerhands[i].flush;
		straightHands |= PLAYER_BIT_PACK[i] * (char)pokerhands[i].straight;
	}

	straightFlushHands = flushHands & straightHands;
	if (straightFlushHands) {
		checkStraight(pool, hands, numHands, pokerhands, flushSuit);
		straightFlushHands = 0;
		for (unsigned int i = 0; i < numHands; i++) 
		{
			straightFlushHands |= PLAYER_BIT_PACK[i] * ((char)pokerhands[i].straightFlush);
		}
		CHECK_BEST_HAND(straightFlushHands, HandType::STRFLUSH)
	}

	checkHighPairTripQuad(pool, hands, numHands, pokerhands);

	for (unsigned int i = 0; i < numHands; i++)
	{
		quadHands |= PLAYER_BIT_PACK[i] * (pokerhands[i].pair[0] == pokerhands[i].pair[1]);
		tripleHands |= PLAYER_BIT_PACK[i] * (pokerhands[i].triple != -1);
		twoPairHands |= PLAYER_BIT_PACK[i] * (pokerhands[i].pair[1] > -1);
		pairHands |= PLAYER_BIT_PACK[i] * (pokerhands[i].pair[0] != -1);
	}

	CHECK_BEST_HAND(quadHands, HandType::QUADS)

	houseHands = (tripleHands & pairHands) | (tripleHands & twoPairHands);
	CHECK_BEST_HAND(houseHands, HandType::HOUSE)

	CHECK_BEST_HAND(flushHands, HandType::FLUSH)
	CHECK_BEST_HAND(straightHands, HandType::STRAIGHT)
	CHECK_BEST_HAND(tripleHands, HandType::TRIPLE)
	CHECK_BEST_HAND(twoPairHands, HandType::TWOPAIR)
	CHECK_BEST_HAND(pairHands, HandType::PAIR)
	CHECK_BEST_HAND(0x3F, HandType::HIGH)
}
