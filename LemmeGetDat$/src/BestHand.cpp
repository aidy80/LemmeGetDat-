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

inline bool FitsInLowest(Card usedCard, Card handCard, int lowestCard) {
	return (usedCard != handCard && handCard.number == (lowestCard - 1));
}

inline bool FitsInHighest(Card usedCard, Card handCard, int highestCard) {
	return (usedCard != handCard && handCard.number == (highestCard + 1));
}

inline bool isHandCard(int firstNum, int secNum, int cardNum) {
	return (firstNum == cardNum || secNum == cardNum);
}

inline bool isQuads(PokerHand* pokerhands, int k) {
	return (pokerhands[k].pair[0] == pokerhands[k].pair[1]);
}

inline int isWinning(int winningHands, int k) {
	return (int)((bool)(winningHands & PLAYER_BIT_PACK[k]));
}

/*Looks at cards 1 and 2 to determine if either can be considered as 
kickers given the pool signified by include map. 

Params: cards 1 and 2 - the cards to be considered
		pool - the pool currently on board
		includeMap - a binary map signifying the pool cards which should be considered.
					 If includeMap = 0010011, only the first two cards on the flop and 
					 the river should be considered
					 
Return: A pair of numbers signifying the kickers. If a given card can be considered a kicker, 
		it is returned in pair as its number value. Otherwise, a -1 is placed. Pair is sorted such 
		that the larger kicker is in the first spot.
*/
std::pair<int, int> getKickers(const Card card1, const Card card2, const Pool& pool, const int includeMap) 
{
	std::pair<int, int> kickers;

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

/*Determine whether there is a possible flush given the pool and which cards would be 
considered in the flush if there were one. 

Params: pool - the current pool 
		suitHist - a histogram used to count the number of each suit in the pool
		includeMap - a binary map that is written in this function. It will show 
					 which cards have the "flush suit" if there is indeed a possible flush
					 (at least 3 cards on pool with the same suit).
					 If the flush suit is hearts and the outputtedinclude map is
					 0010011, we know that the first two cards on the flop and the river 
					 are all the flush suit

Return: a int representing the flush suit. The same numerics is used as the Card struct
(see Card.h). If there is no possible flush on the pool, -1 is returned
*/
int possibleFlush(const Pool& pool, int* suitHist, int& includeMap)
{
	includeMap = 0;
	for (int i = 0; i < NUM_POOL_CARDS; i++)
	{
		suitHist[pool.cards[i].suit]++;
	}
	
	int flushSuit = -1;
	for (int i = 0; i < NUM_SUITS; i++)
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

/*Determine which hands have a flush. 
Params: pool - the cards on the pool
		hands - the array of all players hands
		bestHands - a 2D array which contains info about the players currently in the game (not folded)
		pokerhands - an array of pokerhand structs, one for each of the hands in the hands array. Each struct 
		describes the type of pokerhand that player has. If a flush is present for a given hand(s), the "flush"
		field will be set to true and kickers will be assigned if the player has any valid kicker
		
		
Return: The "flush suit" of the board. I.e. if there are at least 3 cards on the pool with the same suit. If 
there is no possible flush on the pool, -1 is returned.*/
int checkFlush(const Pool& pool, const Hand* hands, Ranged2DArray& bestHands, PokerHand* pokerhands)
{
	int suitHist[NUM_SUITS];
	for (int i = 0; i < NUM_SUITS; i++) {
		suitHist[i] = 0;
	}

	int includeMap; 
	int flushSuit = possibleFlush(pool, suitHist, includeMap);

	if (flushSuit != -1) 
	{
		int currPlayer;
		while ((currPlayer = bestHands.getNextUnique()) != -1) 
		{
			int firstSuit = hands[currPlayer].cards[0].suit;
			int secSuit = hands[currPlayer].cards[1].suit;
			
			if ( (suitHist[flushSuit] + (firstSuit == flushSuit) + (secSuit == flushSuit)) > 4 ) 
			{
				pokerhands[currPlayer].flush = true;
			
				std::pair<int, int> kickers;

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

/*Fill in the current straight with cards from currHand if they fit into the top and bottom of 
the current straight (defined by lowestCard and highestCard) and numStraightCards for the 
number of cards between them. 
See testPoolStraight for more parameter info */
void testHighLowStrCards(const Hand currHand, int& lowestCard, int& highestCard, 
								int numUsedHandCards, Card* usedHandCards) {
	while (numUsedHandCards < 2)
	{
		if (FitsInLowest(usedHandCards[0], currHand.cards[0], lowestCard))
		{
			lowestCard = currHand.cards[0].number;
			usedHandCards[0] = currHand.cards[0];
		}
		else if (FitsInLowest(usedHandCards[0], currHand.cards[1], lowestCard)) {
			lowestCard = currHand.cards[1].number;
			usedHandCards[0] = currHand.cards[1];
		}
		else if (FitsInHighest(usedHandCards[0], currHand.cards[0], highestCard)) {
			highestCard = currHand.cards[0].number;
			usedHandCards[0] = currHand.cards[0];
		}
		else if (FitsInHighest(usedHandCards[0], currHand.cards[1], highestCard)) {
			highestCard = currHand.cards[1].number;
			usedHandCards[0] = currHand.cards[1];
		}
		else {
			break;
		}
		numUsedHandCards++;
	}
}



/*Check the pool subsection to see if the given "currHand" fills in the gaps preventing the pool 
from being a straight on its own. If some card in the currHand does fill a needed gap, usedHandCards 
is set to include it, and the returned "numUsedHandCards" denotes the number of hand cards that were used to 
fill gaps. If the proper gap was not filled, gotoNext is set to true. Check the function "testPoolStraight" for the rest of the parameters*/
int checkPoolGaps(const Pool& pool, const Hand& currHand, int startCardIndex, int endCardIndex, bool& gotoNext, int flushSuit, Card* usedHandCards)
{
	int numUsedHandCards = 0;

	for (int i = startCardIndex; !gotoNext && i < endCardIndex; i++)
	{
		int poolSpacing = pool.cards[i + 1].number - pool.cards[i].number - 1;
		for (int j = 0; j < poolSpacing; j++) {
			int neededCard = pool.cards[i].number + j + 1;
			if ((currHand.cards[0].number == neededCard) && SuitRequirement(currHand.cards[0], flushSuit)) {
				usedHandCards[numUsedHandCards] = currHand.cards[0];
			}
			else if ((currHand.cards[1].number == neededCard) && SuitRequirement(currHand.cards[1], flushSuit)) {
				usedHandCards[numUsedHandCards] = currHand.cards[1];
			}
			else {
				gotoNext = true;
				continue;
			}
			numUsedHandCards++;
		}
	}

	return numUsedHandCards;
}

/*Test whether any hands has a straight using a specified subsection of pool cards. If so, mark the result in the pokerhands array

Params: pool - the pool cards. They must come sorted in order from lowest number to highest number
		startCardIndex, endCardIndex - The first and last pool indices to consider. So if start=1 and end is 5, 
									   all cards on the pool will be considered except for the first card on the flop
		hands - the array of all hands on the table
		bestHands - a two dimensional array with information about which players have not yet folded (whose hands must be checked for straights)
		pokerhands - an array of pokerhands structs whose "straight" fields will be set to true if the corresponding player has a straight.
				     additionally, if there is a straight, kicker1 will be set to the highest card in the straight.
		flushSuit - If a straightflush is being tested for, a flushSuit which is not -1 will indicate that we should only consider cards 
					with the same suit as flushsuit (a numerical representation identical to the Card struct in Card.h)
*/
void testPoolStraight(const Pool& pool, const int startCardIndex, const int endCardIndex, const Hand* hands, 
					  Ranged2DArray& bestHands, PokerHand* pokerhands, const int flushSuit = -1)
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

		int lowestCard = pool.cards[startCardIndex].number;
		int highestCard = pool.cards[endCardIndex].number;
		Card usedHandCards[2] = { Card::NULL_CARD, Card::NULL_CARD };
		int numUsedHandCards = checkPoolGaps(pool, hands[currPlayer], startCardIndex, endCardIndex, 
																	gotoNext, flushSuit, usedHandCards);

		if (!gotoNext) {
			testHighLowStrCards(hands[currPlayer], lowestCard, highestCard, numUsedHandCards, usedHandCards);

			if ((highestCard - lowestCard + 1) > 4) {
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

/*Take an original pool of cards and create an applied pool which, if there is a flushSuit!=-1, 
does not include pool non flushSuit cards. If flushSuit==-1, duplicate numbers are not included in applied pool.

The number of cards in the applied pool is returned

See check straight for parameter info*/
int fillAppliedPool(const Pool& originalPool, Pool& appliedPool, int flushSuit) 
{
	int numPoolCards = 0;
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

	return numPoolCards;
}

/*
Check which players have straights

Params: pool - the pool cards. They must come sorted in order from smallest number to largest number
		hands - the array of all hands on the table
		bestHands - a two dimensional array with information about which players have not yet folded (whose hands must be checked for straights)
		pokerhands - an array of pokerhands structs whose "straight" fields will be set to true if the corresponding player has a straight.
				     additionally, if there is a straight, kicker1 will be set to the highest card in the straight.
		flushSuit - If a straightflush is being tested for, a flushSuit which is not -1 will indicate that we should only consider cards 
					with the same suit as flushsuit (a numerical representation identical to the Card struct in Card.h)
*/
void checkStraight(const Pool& originalPool, const Hand* hands, Ranged2DArray& bestHands, PokerHand* pokerhands, const int flushSuit = -1)
{
	Pool appliedPool; //Change to card array?
	int numPoolCards = fillAppliedPool(originalPool, appliedPool, flushSuit);

	int* poolSpacing = (int*)_malloca(sizeof(int) * (numPoolCards - 1));
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
	_freea(poolSpacing);
}

/*Fill the number histogram to reflect what numbers are given by the pool and how many*/
inline void fillNumHistPool(int* numHist, const Pool& pool) 
{
	for (int i = 0; i < NUM_CARD_NUMBERS; i++) {
		numHist[i] = 0;
	}

	for (int i = 0; i < NUM_POOL_CARDS; i++) {
		numHist[pool.cards[i].number]++;
	}
}

/*Go through all the entire histogram and fill in pokerhands to reflect when a quad, triple, or pair is seen. Additionally, adjust and tempKicker1, tempKicker2
to contain the likely kickers depending on whether a quad, triple, and/or pair is seen. Additionally, unusedCombo is used to store the case where there is 
an unsed pair or triple (as is the case with a quad+triple) or any unused pair (as is the case with three pairs)

See checkHighPairTripQuad for more parameter info*/
void processNumHist(int* numHist, const Hand& currHand, const int currPlayer, PokerHand* pokerhands, int& tempKicker1, int& tempKicker2, int& unusedCombo)
{
	const int firstNum = currHand.cards[0].number;
	const int secNum = currHand.cards[1].number;

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
			else if (pokerhands[currPlayer].pair[0] != -1) {
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
}

/*
Given a filled histogram and pokerhands made to reflect the existence of pairs, triples, and quads, fill in the players kickers if they are valid. 
That is, only give a player a kicker if it is used as a single card and comes from their hands and the size of the pokerhand 
remains at a max of 5 cards. So, the player has three jacks, two on board and one in their hand, and they have a nine in hand, that nine will 
only be considered as a kicker if there are not two higher cards on board (i.e. if there is a king and an ace on board, the nine will not be a kicker)
*/
void fillHistKickers(int* numHist, const Hand& currHand, const int currPlayer, PokerHand* pokerhands, int tempKicker1, int tempKicker2, int unusedCombo) {
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
				if (isHandCard(currHand.cards[0].number, currHand.cards[1].number, i)) {
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
}



/*
Check which have the best quad, house, three of a kind, two pairs, pair, or, 
if nothing else, high card. This is supposed to be called after checking for 
straights and flushes and will only change the player's kicker value if a given 
player has quads or a full house and/or if the player has neither a flush or straight.
Further, if the player has a three of a kind but no full house or quads, the kickers
will represent the player's three of a kind hand. If the player only has a two pair, 
the kicker will represent the two pair, etc for pair and high card.

Params: pool - the boards pool cards
		hands - the hands of all players on the table
		bestHands - a two dimensional array with information about which players have not yet folded
		pokerhands - an array of Pokerhand structs which hold information about the best pokerhands
					 that (unfolded) players have. This will be updated with the quads, triple, pair, 
					 and high card info
*/
void checkHighPairTripQuad(const Pool& pool, const Hand* hands, Ranged2DArray& bestHands, PokerHand* pokerhands)
{
	int* numHist = (int*)_malloca(sizeof(int) * NUM_CARD_NUMBERS);
	fillNumHistPool(numHist, pool);

	int currPlayer;
	while ((currPlayer = bestHands.getNextUnique()) != -1) {
		int tempKicker1 = -1;
		int tempKicker2 = -1;
		int unusedCombo = -1;
		numHist[hands[currPlayer].cards[0].number]++;
		numHist[hands[currPlayer].cards[1].number]++;

		processNumHist(numHist, hands[currPlayer], currPlayer, pokerhands, tempKicker1, tempKicker2, unusedCombo);
		fillHistKickers(numHist, hands[currPlayer], currPlayer, pokerhands, tempKicker1, tempKicker2, unusedCombo);

		numHist[hands[currPlayer].cards[0].number]--;
		numHist[hands[currPlayer].cards[1].number]--;
	}
	_freea(numHist);
}

template <typename T> 
void swapElems(T& elem1, T& elem2) {
	T temp = elem1;
	elem1 = elem2;
	elem2 = temp;
}

/*Standard templated insert sort*/
template <typename T>
void insertSort(T* elems, int numElems, std::function<bool(T&, T&)> comparator)
{	
#ifdef _DEBUG
	assert(numElems >= 0);
#endif
	for (int i = 1; i < numElems; i++) {
		for (int j = i - 1; j > -1; j--) {
			if(comparator(elems[j + 1], elems[j])) {
				swapElems<T>(elems[j + 1], elems[j]);
			}
			else {
				break;
			}
		}
	}
}

/*player - Contains a given hand's player, 
winType - whether it has the best type of hand (i.e. it is true if the hand had three Jacks if three jacks is the best poker hand out of the players int the pot),
winWithKicker - whether it has the best type of hand (same as winType) AND it has the best kicker(s) if applicable*/
struct WinningHand {
	int player;
	bool winType;
	bool winWithKicker;
};

/*Go through all of the hands in the current pot and, given the best hand type (winHandType), find out which is the highest form of that hand. 
So, if the HandType is QUADS, and one player has 4 twos while another has 4 jacks, the highest hands will be set to {9, 0} where 9 denotes a jack. 
in the case of a two pair, the highest two pair will be picked. So if one player has Aces and Kings while the other has Aces and Threes, highest
hands will be {12, 11}

See fillBestHand for more param info
*/
void getHighestHand(int* highestHand, const int potNum, Ranged2DArray& bestHands, PokerHand* pokerhands, 
						   const int winningHands, const HandType winHandType) {
	for (int i = 0; i < bestHands.getRowSize(potNum); i++)
	{
		int currPlayer = bestHands.get(potNum, i);

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
}

/*
Given the best type of pokerhand and the highest hand in that subtype, determine which players(s) have that winning hand type 
and record whether or not they do into the WinningHand array. 

See fillBestHand for more specific param information
*/
void fillWinType(WinningHand* handsWithIndex, int* highestHand, const int potNum, 
						Ranged2DArray& bestHands, PokerHand* pokerhands, const int winningHands, const HandType winHandType) {
	for (int i = 0; i < bestHands.getRowSize(potNum); i++) {
		int currPlayer = bestHands.get(potNum, i);
		handsWithIndex[i].player = currPlayer;

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
}

/*Return a lambda function that compares Winning hands such that only hands with the highest kicker 
and highest winning hand will be earlier in the order, and have winWithKicker==true*/
auto getWinHandCompare(const PokerHand* pokerhands) {
	return [pokerhands](WinningHand& val1, WinningHand& val2) -> bool
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
				}
				else if (pokerhands[val1.player].kicker2 < pokerhands[val2.player].kicker2) {
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
		}
		else if (val1.winType == 1) {
			return true;
		}
		else if (val2.winType == 1) {
			return false;
		}
		return val1.player < val2.player;
	};
}

/*Fill the bestHands array to reflect the winning hands in the given potNum. So, if players 1, 2, and 3 are in the pot and 
players 1 and 2 tied but both beat player 3, we would see the corresponding row in the bestHands array be set to 
 1, 2, -1, -1, -1, -1
 
 Params: bestHands - the two dimensional array denoting the various pots in the game, 
					 with each row containing the players in each pot. See getBestHand for more detail
		 potNum - the number of the pot being filled
		 winningHands - a bitmap showing which players in the pot have the "winHandType" of hand. That is
						if winHandType = TRIPLE, all 1s in the winningHands bit map represent players with triples.
						So, it winningHands = 00001001, players 0 and 3 have the winning hand types
		 winHandType - the winning type of hand
		 pokerhands - an array containing information about the pokerhands for the various players in the pot. 
					  Specifically, info about a given player has quads, triples, two pairs, flushes, ..., etc 
					  along with their best kicker(s).
*/
void fillBestHands(Ranged2DArray& bestHands, const int potNum, const int winningHands, const HandType winHandType, PokerHand* pokerhands) 
{
	WinningHand* handsWithIndex = (WinningHand*)_malloca(sizeof(WinningHand) * bestHands.getRowSize(potNum));
	int highestHand[2] = { -1, -1 };
	
	getHighestHand(highestHand, potNum, bestHands, pokerhands, winningHands, winHandType);
	fillWinType(handsWithIndex, highestHand, potNum, bestHands, pokerhands, winningHands, winHandType);

	/*Sort the winning hands such that only the hands with the 
	highest winning hand and highest kicker have "winWithKicker == true"*/
	insertSort<WinningHand>(handsWithIndex, bestHands.getRowSize(potNum), getWinHandCompare(pokerhands));
	
	for (int i = 0; i < bestHands.getRowSize(potNum); i++)
	{
		bestHands.set(potNum, i, handsWithIndex[i].winWithKicker ? handsWithIndex[i].player : -1);
	}

	_freea(handsWithIndex);
}

/*Sort the pool such that lower numbered cards are earlier in the pool (i.e. twos come before jacks) */
inline void sortPool(Pool& pool) 
{
	insertSort<Card>(pool.cards, NUM_POOL_CARDS,
		[](Card card1, Card card2) {
			return card1.number < card2.number;
		   });
}

/*Function which takes in the hands of all players, the (full 5-card) pool on board, and returns the player(s) 
with the best pokerhands. It requires bestHands as both an input (showing the players in each pot) and 
uses it as an output (showing which player(s) win the pot(s)).

Params: pool - The pool cards on board
		allHands - all the players hands at the tables
		bestHands - a 2D array outlining the various pots in the game. As an input, it will have a 
					row for each pot on table. In it, will be the players immediately adjecent to 
					each other. So, if players 1 2 3 are in one pot and players 1 2 3 4 are in another, 
					out bestHands input should be
					| 1 2 3 -1 -1 -1 |
					| 1 2 3  4 -1 -1 |
					Then, if players 1 and 2 tie the first pot and player 4 wins the second pot, 
					the outputted bestHands arrays will be
					| 1  2 -1 -1 -1 -1|
					| 4 -1 -1 -1 -1 -1|
*/
void getBestHands(Pool& pool, const Hand* allHands, Ranged2DArray& bestHands)
{
	PokerHand* pokerhands = (PokerHand*)_malloca(sizeof(PokerHand) * bestHands.getNumCols());
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
		int playersInPot = 0;
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
	_freea(pokerhands);
}
