#include "BestHand.h"

#define CHECK_BEST_HAND(hands, handType) \
	if (hands & playersInPot) { \
		fillBestHands(bestHands, i, hands, handType, pokerhands); \
		continue; \
	}

//std::cout << "Best hand:" << (int)handType << std::endl; \

inline bool SuitRequirement(Card card, int flushSuit) { 
	return ((flushSuit == -1) || (flushSuit == card.suit));
}

inline bool FitsInLowest(Card usedCard, Card handCard, char lowestCard) {
	return (usedCard != handCard && handCard.number == (lowestCard - 1));
}

inline bool FitsInHighest(Card usedCard, Card handCard, char highestCard) {
	return (usedCard != handCard && handCard.number == (highestCard + 1));
}

inline bool isHandCard(char firstNum, char secNum, char cardNum) {
	return (firstNum == cardNum || secNum == cardNum);
}

inline bool isQuads(PokerHand* pokerhands, int k) {
	return (pokerhands[k].pair[0] == pokerhands[k].pair[1]);
}

inline unsigned char isWinning(unsigned char winningHands, unsigned char k) {
	return (unsigned char)((bool)(winningHands & PLAYER_BIT_PACK[k]));
}

std::pair<char, char> getKickers(Card card1, Card card2, Pool& pool, unsigned char includeMap) 
{
	std::pair<char, char> kickers;

	int card1Less = 0, card2Less = 0;

	bool card1NotNONE = (card1 != Card::NULL_CARD), card2NotNONE = (card2 != Card::NULL_CARD);

	for (int i = 0; i < NUM_POOL_CARDS; i++)
	{	
		if (includeMap & POOL_BIT_PACK[i]) {
			if (card1NotNONE && pool.cards[i].number > card1.number) {
				card1Less++;
			}
			if (card2NotNONE && pool.cards[i].number > card2.number) {
				card2Less++;
			}
		}
	}

	if (card1NotNONE && card2NotNONE)
	{
		if (card1.number < card2.number) {
			kickers.first = (card2Less < 5) ? card2.number : -1;
			kickers.second = (++card1Less < 5) ? card1.number : -1;
		}
		else if (card1.number > card2.number) {
			kickers.first = (card1Less < 5) ? card2.number : -1;
			kickers.second = (++card2Less < 5) ? card2.number : -1;
		}
	} else if (card1NotNONE && !card2NotNONE) {
		kickers.first = (card1Less < 5) ? card1.number : -1;
		kickers.second = -1;
	} else if (!card1NotNONE && card2NotNONE) {
		kickers.first = (card2Less < 5) ? card2.number : -1;
		kickers.second = -1;
	} else {
		kickers.first = -1; kickers.second = -1;
	}

	return kickers;
}

char possibleFlush(Pool& pool, char* suitHist, unsigned char& includeMap)
{
	unsigned char i;
	for (i = 0; i < NUM_POOL_CARDS; i++)
	{
		suitHist[pool.cards[i].suit]++;
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
		if (pool.cards[i].suit == flushSuit) 
		{
			includeMap |= POOL_BIT_PACK[i];
		}
	}

	return flushSuit;
}

char checkFlush(Pool& pool, Hand* hands, TwoDimArray& bestHands, PokerHand* pokerhands)
{
	char suitHist[NUM_SUITS];
	for (int i = 0; i < NUM_SUITS; i++) {
		suitHist[i] = 0;
	}

	unsigned char includeMap = 0; 
	char flushSuit = possibleFlush(pool, suitHist, includeMap);

	if (flushSuit != -1) 
	{

		int currPlayer;
		while ((currPlayer = bestHands.getNextUnique()) != -1) 
		{
			char firstSuit = hands[currPlayer].cards[0].suit;
			char secSuit = hands[currPlayer].cards[1].suit;
			
			if ( (suitHist[flushSuit] + (firstSuit == flushSuit) + (secSuit == flushSuit)) > 4 ) 
			{
				pokerhands[currPlayer].flush = true;
			
				std::pair<char, char> kickers;

				if (firstSuit == flushSuit) {
					if (secSuit == flushSuit) {
						kickers = getKickers(hands[currPlayer].cards[0], hands[currPlayer].cards[1], pool, includeMap);
					} else {
						kickers = getKickers(hands[currPlayer].cards[0], Card::NULL_CARD, pool, includeMap);
					}
				} else if (secSuit == flushSuit) {
					kickers = getKickers(Card::NULL_CARD, hands[currPlayer].cards[1], pool, includeMap);
				} else {
					kickers = { -1, -1 };
				}

				pokerhands[currPlayer].kicker1 = kickers.first;
				pokerhands[currPlayer].kicker2 = kickers.second;
			}
			else {
				pokerhands[currPlayer].flush = false;
			}
		}			
	}
	return flushSuit;
}

void testPoolStraight(Pool& pool, unsigned char startCardIndex, unsigned char endCardIndex, Hand* hands, TwoDimArray& bestHands, PokerHand* pokerhands, char flushSuit)
{	
	int currPlayer;
	while ((currPlayer = bestHands.getNextUnique()) != -1) {
		bool gotoNext = false;
		if (flushSuit != -1) {
			if (endCardIndex - startCardIndex + 1 + (hands[currPlayer].cards[0].suit == flushSuit)
				+ (hands[currPlayer].cards[1].suit == flushSuit) < 5) 
			{
				gotoNext = true;
			}
		}

		Card usedHandCards[2] = { Card::NULL_CARD, Card::NULL_CARD };
		unsigned char numUsedHandCards = 0;
		int lowestCard = pool.cards[startCardIndex].number;
		int highestCard = pool.cards[endCardIndex].number;
		int numStraightCards = highestCard - lowestCard + 1;

		for (int i = startCardIndex; !gotoNext && i < endCardIndex; i++)
		{
			int poolSpacing = pool.cards[i + 1].number - pool.cards[i].number - 1;
			for (int j = 0; j < poolSpacing; j++) {
				int neededCard = pool.cards[i].number + j + 1;
				if ((hands[currPlayer].cards[0].number == neededCard) && SuitRequirement(hands[currPlayer].cards[0], flushSuit)) {
					usedHandCards[numUsedHandCards] = hands[currPlayer].cards[0];
				}
				else if ((hands[currPlayer].cards[1].number == neededCard) && SuitRequirement(hands[currPlayer].cards[1], flushSuit)) {
					usedHandCards[numUsedHandCards] = hands[currPlayer].cards[1];
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
				if (FitsInLowest(usedHandCards[0], hands[currPlayer].cards[0], lowestCard))
				{
					lowestCard = hands[currPlayer].cards[0].number;
					numStraightCards++;
					usedHandCards[0] = hands[currPlayer].cards[0];
				} 
				else if (FitsInLowest(usedHandCards[0], hands[currPlayer].cards[1], lowestCard)) {
					lowestCard = hands[currPlayer].cards[1].number;
					numStraightCards++;
					usedHandCards[0] = hands[currPlayer].cards[1];
				} 
				else if (FitsInHighest(usedHandCards[0], hands[currPlayer].cards[0], highestCard)) {
					highestCard = hands[currPlayer].cards[0].number;
					numStraightCards++;
					usedHandCards[0] = hands[currPlayer].cards[0];
				} 
				else if (FitsInHighest(usedHandCards[0], hands[currPlayer].cards[1], highestCard)) {
					highestCard = hands[currPlayer].cards[1].number;
					numStraightCards++;
					usedHandCards[0] = hands[currPlayer].cards[1];
				}
				else {
					break;
				}
				numUsedHandCards++;
			}

			if (numStraightCards > 4) {
				if (flushSuit == -1) {
					pokerhands[currPlayer].straight = true;
				}
				else {
					pokerhands[currPlayer].straightFlush = true;
				}
				if (pokerhands[currPlayer].kicker1 < highestCard) {
					pokerhands[currPlayer].kicker1 = highestCard;
				}
			}
		}
	}
}

void checkStraight(Pool& originalPool, Hand* hands, TwoDimArray& bestHands, PokerHand* pokerhands, char flushSuit)
{
	unsigned char numPoolCards = 0;
	Pool appliedPool; //Change to card array?
	if (flushSuit != -1) {
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			if (originalPool.cards[i].suit == flushSuit) {
				appliedPool.cards[numPoolCards] = originalPool.cards[i];
				numPoolCards++;
			}
		}
	}
	else {
		appliedPool.cards[0] = originalPool.cards[0];
		numPoolCards++;
		for (int i = 1; i < NUM_POOL_CARDS; i++) {
			if (originalPool.cards[i].number != appliedPool.cards[numPoolCards - 1].number) {
				appliedPool.cards[numPoolCards] = originalPool.cards[i];
				numPoolCards++;
			}
		}
	}

	unsigned char* poolSpacing = (unsigned char*)alloca(sizeof(unsigned char) * (numPoolCards - 1));
	for (int i = 0; i < numPoolCards - 1; i++) 
	{
		poolSpacing[i] = appliedPool.cards[i + 1].number - appliedPool.cards[i].number - 1;
	}

	 
	for (int i = 0; i < numPoolCards - 2; i++) {
		int totalPoolSpacing = 0;
		for (int s = 2; s < (numPoolCards - i); s++) {
			for (int j = i; j < i + s; j++) {
				totalPoolSpacing += poolSpacing[j];
			}
			if (totalPoolSpacing < 3) {
				testPoolStraight(appliedPool, i, i + s, hands, bestHands, pokerhands, flushSuit);
			}
		}
	}
}

void checkHighPairTripQuad(Pool& pool, Hand* hands, TwoDimArray& bestHands, PokerHand* pokerhands)
{
	unsigned char* numHist = (unsigned char*)alloca(sizeof(unsigned char) * NUM_CARD_NUMBERS);
	for (int i = 0; i < NUM_CARD_NUMBERS; i++) {
		numHist[i] = 0;
	}

	for (int i = 0; i < NUM_POOL_CARDS; i++) {
		numHist[pool.cards[i].number]++;
	}

	int currPlayer;
	while ((currPlayer = bestHands.getNextUnique()) != -1) {
		unsigned char firstNum = hands[currPlayer].cards[0].number;
		unsigned char secNum = hands[currPlayer].cards[1].number;
		numHist[firstNum]++;
		numHist[secNum]++;
		char tempKicker1 = -1;
		char tempKicker2 = -1;
		char unusedCombo = -1;

		for (int i = 0; i < NUM_CARD_NUMBERS; i++) {
			if (numHist[i] == 2)
			{
				if (!isQuads(pokerhands, currPlayer))
				{
					if (pokerhands[currPlayer].pair[1] > -1)
					{
						if (firstNum == pokerhands[0].pair[0] && 
							secNum == pokerhands[currPlayer].pair[1]) {
							tempKicker1 = pokerhands[currPlayer].pair[1];
						}
						else {
							unusedCombo = pokerhands[currPlayer].pair[1];
						}
					}
					pokerhands[currPlayer].pair[1] = pokerhands[currPlayer].pair[0];
					pokerhands[currPlayer].pair[0] = i;
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
				if (isQuads(pokerhands, currPlayer) && isHandCard(firstNum, secNum, i))
				{
					tempKicker1 = i;
				}
				else {
					if (pokerhands[currPlayer].triple != -1) {
						pokerhands[currPlayer].pair[0] = pokerhands[currPlayer].triple;
					}
					pokerhands[currPlayer].triple = i;
				}
			}
			else if (numHist[i] == 4) 
			{
				if (pokerhands[currPlayer].triple != -1 && isHandCard(firstNum, secNum, pokerhands[currPlayer].triple)) {
					tempKicker1 = pokerhands[currPlayer].triple;
				}
				else if (pokerhands[currPlayer].pair[0] != -1){
					if (isHandCard(firstNum, secNum, pokerhands[currPlayer].pair[0])) {
						tempKicker1 = pokerhands[currPlayer].pair[0];
					}
					else {
						unusedCombo = pokerhands[currPlayer].pair[0];
					}
				}
				pokerhands[currPlayer].pair[0] = i;
				pokerhands[currPlayer].pair[1] = i;
			}
		}

		if (isQuads(pokerhands, currPlayer) || (!(pokerhands[currPlayer].flush || pokerhands[currPlayer].straight))) {
			int i;
			int numUsed = 0;
			
			if (isQuads(pokerhands, currPlayer)) {
				numUsed = 4;
			}
			else if (pokerhands[currPlayer].triple > -1) {
				numUsed = 3;
			}
			else if (pokerhands[currPlayer].pair[1] > -1) {
				numUsed = 4;
			}
			else if (pokerhands[currPlayer].pair[0] > -1) {
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

			if (isQuads(pokerhands, currPlayer)) {
				tempKicker2 = -1;
			}
			else if (pokerhands[currPlayer].triple != -1 && pokerhands[currPlayer].pair[0] != -1) {
				tempKicker1 = pokerhands[currPlayer].triple;
				tempKicker2 = pokerhands[currPlayer].pair[0];
			}
			else if (pokerhands[currPlayer].pair[1] > -1) {
				tempKicker2 = -1;
			}

			pokerhands[currPlayer].kicker1 = tempKicker1;
			pokerhands[currPlayer].kicker2 = tempKicker2;
		}

		numHist[secNum]--;
		numHist[firstNum]--;
	}
}

template <typename T> 
void swapElems(T& elem1, T& elem2) {
	T temp = elem1;
	elem1 = elem2;
	elem2 = temp;
}

template <typename T>
void insertSort(T* elems, int numElems, std::function<bool(T&, T&)> comparator)
{	
#ifdef _DEBUG
	assert(numElems >= 0);
#endif
	for (int i = 1; i < numElems; i++) {
		for (int j = i - 1; j > -1; j--) {
			bool comp = comparator(elems[j + 1], elems[j]);
			//std::cout << "Elem 1: " << elems[j] << "Elem 2: " << elems[j + 1] <<
			//			 ". Comparator: " << comp;
			if (comp) {
				swapElems<T>(elems[j + 1], elems[j]);
			}
			else {
				break;
			}
		}
	}
}

struct WinningHand {
	unsigned char player;
	bool winType;
	bool winWithKicker;
};

void fillBestHands(TwoDimArray& bestHands, int potNum, int winningHands, HandType winHandType, PokerHand* pokerhands) 
{
	WinningHand* handsWithIndex = (WinningHand*)alloca(sizeof(WinningHand) * bestHands.getRowSize(potNum));
	char highestHand[2] = { -1, -1 };
	for (int i = 0; i < bestHands.getRowSize(potNum); i++)
	{
		int currPlayer = bestHands.get(potNum, i);
		handsWithIndex[i].player = currPlayer;

		if (isWinning(winningHands, currPlayer)) {
			switch (winHandType) {
			case HandType::PAIR:
				if (highestHand[0] < pokerhands[currPlayer].pair[0]) {
					highestHand[0] = pokerhands[currPlayer].pair[0];
				}
				break;
			case HandType::TWOPAIR:
				if (highestHand[0] < pokerhands[currPlayer].pair[0]) {
					highestHand[0] = pokerhands[currPlayer].pair[0];
					highestHand[1] = pokerhands[currPlayer].pair[1];
				}
				if (highestHand[0] == pokerhands[currPlayer].pair[0] && highestHand[1] < pokerhands[currPlayer].pair[1]) {
					highestHand[1] = pokerhands[currPlayer].pair[1];
				}
				break;
			case HandType::TRIPLE:
				if (highestHand[0] < pokerhands[currPlayer].triple) {
					highestHand[0] = pokerhands[currPlayer].triple;
				}
				break;
			case HandType::QUADS:
				if (highestHand[0] < pokerhands[currPlayer].pair[0]) {
					highestHand[0] = pokerhands[currPlayer].pair[0];
				}
				break;
			}
		}
	}

	for (int i = 0; i < bestHands.getRowSize(potNum); i++) {
		int currPlayer = bestHands.get(potNum, i);
		if (isWinning(winningHands, currPlayer)) {
			switch (winHandType) {
			case HandType::PAIR:
				handsWithIndex[i].winType = (pokerhands[currPlayer].pair[0] == highestHand[0]);
				break;
			case HandType::TWOPAIR:
				handsWithIndex[i].winType = (pokerhands[currPlayer].pair[0] == highestHand[0] && 
											pokerhands[currPlayer].pair[1] == highestHand[1]);
				break;
			case HandType::TRIPLE:
				handsWithIndex[i].winType = (pokerhands[currPlayer].triple == highestHand[0]);
				break;
			case HandType::QUADS:
				handsWithIndex[i].winType = (pokerhands[currPlayer].pair[0] == highestHand[0]);
				break;
			default:
				handsWithIndex[i].winType = true;
			}
		} 
		else {
			handsWithIndex[i].winType = false;
		}

		handsWithIndex[i].winWithKicker = handsWithIndex[i].winType;
	}

	insertSort<WinningHand>(handsWithIndex, bestHands.getRowSize(potNum),
		[pokerhands](WinningHand& val1, WinningHand& val2) -> bool
		{
			if (val1.winType == 1 && val2.winType == 1) {
				if (pokerhands[val1.player].kicker1 > pokerhands[val2.player].kicker1) 
				{
					val2.winWithKicker = false;
					return true;
				}
				else if (pokerhands[val1.player].kicker1 < pokerhands[val2.player].kicker1) {
					val1.winWithKicker = false;
					return false;
				}
				else {
					if (pokerhands[val1.player].kicker2 > pokerhands[val2.player].kicker2) {
						val2.winWithKicker = false;
						return true;
					} else if (pokerhands[val1.player].kicker2 < pokerhands[val2.player].kicker2) {
						val1.winWithKicker = false;
						return false;
					}
					else {
						if (!val1.winWithKicker) {
							val2.winWithKicker = false;
						}
						else if (!val2.winWithKicker) {
							val1.winWithKicker = false;
						}
						return val1.player < val2.player;
					}
				}
			} else if (val1.winType == 1) {
				return true;
			} else if (val2.winType == 1) {
				return false;
			} 
			return val1.player < val2.player;
		});
	
	for (int i = 0; i < bestHands.getRowSize(potNum); i++)
	{
		bestHands.set(potNum, i, handsWithIndex[i].winWithKicker ? handsWithIndex[i].player : -1);
	}
}

inline void sortPool(Pool& pool) 
{
	//std::sort(pool.cards, pool.cards + NUM_POOL_CARDS,
	insertSort<Card>(pool.cards, NUM_POOL_CARDS,
		[](Card card1, Card card2) {
			return card1.number < card2.number;
		   });
}

//TODO:: allow the user to pass in only a range of hands to consider. Add an array of player indices to be considered
void getBestHands(Pool& pool, Hand* allHands, TwoDimArray& bestHands)
{
	PokerHand* pokerhands = (PokerHand*)alloca(sizeof(PokerHand) * bestHands.getNumCols());
	int currPlayer;
	while ((currPlayer = bestHands.getNextUnique()) != -1) 
	{
		initPokerHand(pokerhands[currPlayer]);
	}

	sortPool(pool);

	int flushHands = 0, straightHands = 0, quadHands = 0, tripleHands = 0,
		twoPairHands = 0, pairHands = 0, straightFlushHands = 0, houseHands = 0;

	checkStraight(pool, allHands, bestHands, pokerhands, -1);
	int flushSuit = checkFlush(pool, allHands, bestHands, pokerhands);

	while ((currPlayer = bestHands.getNextUnique()) != -1)
	{
		flushHands |= PLAYER_BIT_PACK[currPlayer] * (int)pokerhands[currPlayer].flush;
		straightHands |= PLAYER_BIT_PACK[currPlayer] * (int)pokerhands[currPlayer].straight;
	}

	if (flushHands & straightHands) {
		checkStraight(pool, allHands, bestHands, pokerhands, flushSuit);
	}

	checkHighPairTripQuad(pool, allHands, bestHands, pokerhands);

	while ((currPlayer = bestHands.getNextUnique()) != -1) {
		straightFlushHands |= PLAYER_BIT_PACK[currPlayer] * ((int)pokerhands[currPlayer].straightFlush);
		quadHands |= PLAYER_BIT_PACK[currPlayer] * (pokerhands[currPlayer].pair[0] == pokerhands[currPlayer].pair[1]);
		tripleHands |= PLAYER_BIT_PACK[currPlayer] * (pokerhands[currPlayer].triple != -1);
		twoPairHands |= PLAYER_BIT_PACK[currPlayer] * (pokerhands[currPlayer].pair[1] > -1);
		pairHands |= PLAYER_BIT_PACK[currPlayer] * (pokerhands[currPlayer].pair[0] != -1);
	}
	houseHands = (tripleHands & pairHands) | (tripleHands & twoPairHands);

	for (int i = 0; i < bestHands.getNumRows(); i++) {
		unsigned char playersInPot = 0;
		for (int p = 0; p < bestHands.getRowSize(i); p++) {
			playersInPot |= PLAYER_BIT_PACK[bestHands.get(i, p)];
		}
		CHECK_BEST_HAND(straightFlushHands, HandType::STRFLUSH)
		CHECK_BEST_HAND(quadHands, HandType::QUADS)
		CHECK_BEST_HAND(houseHands, HandType::HOUSE)
		CHECK_BEST_HAND(flushHands, HandType::FLUSH)
		CHECK_BEST_HAND(straightHands, HandType::STRAIGHT)
		CHECK_BEST_HAND(tripleHands, HandType::TRIPLE)
		CHECK_BEST_HAND(twoPairHands, HandType::TWOPAIR)
		CHECK_BEST_HAND(pairHands, HandType::PAIR)
		CHECK_BEST_HAND(0x3F, HandType::HIGH)
	}
}
