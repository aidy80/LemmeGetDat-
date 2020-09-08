#include "InfoSet.h"
#include "Table.h"

InfoSet::InfoSet()
{
#ifdef _DEBUG
	assert((int)HandID::NUM_HAND_COMBOS == 169);
#endif 

	for (int i = 0; i < NUM_GARB_HANDS; i++) 
	{
		garbageHands.insert(GARBAGE_HANDS[i]);
	}
}
	
InfoSet::~InfoSet() {}

bool fiveSuccBit(InfoSet::BitSet& newPool, int endIndex) 
{
	bool fiveSucc = true;
	for (int i = 0; i < endIndex && fiveSucc; i++) 
	{
		if (newPool[i]) 
		{
			int origI = i;
			for (int j = origI; j < origI + 5; j++, i++) 
			{
				if (!newPool[j]) 
				{
					fiveSucc = false;
					break;
				}
			}
		}
		
		if (!fiveSucc) 
		{
			break;
		}
	}
	return fiveSucc;
}

void setCardBits(int startIndex, int endIndex, int currLoop, std::vector<InfoSet::BitSet>& allPools, InfoSet::BitSet& newPool) 
{
#ifdef _DEBUG
	assert(currLoop > -1);
	assert(startIndex > -1);
#endif
	for (int i = startIndex; i < endIndex; i++) {
		newPool.set(i, 1);
		if (currLoop == 0) {
			if (startIndex == i && fiveSuccBit(newPool, endIndex)) {
				newPool.set(i, 0);
				continue;
			}
			allPools.push_back(newPool);
		}
		else {
			setCardBits(i + 1, endIndex, currLoop - 1, allPools, newPool);
		}
		newPool.set(i, 0);
	}
}

void InfoSet::getAllPoolID(std::vector<BitSet>& allPools)
{
	const int numStarsBars = NUM_POOL_CARDS + NUM_CARD_NUMBERS - 1;
	const int suitOptions = exp(2, InfoSet::SUIT_BITS);

	int temp = nCr(numStarsBars, NUM_POOL_CARDS);
	allPools.reserve(nCr(numStarsBars, NUM_POOL_CARDS) * suitOptions);

	BitSet newPool;
#ifdef _DEBUG
	assert(InfoSet::SUIT_BITS == 1);
#endif
	for (int j = 0; j < 2; j++) {
		newPool.set(numStarsBars, j);
		setCardBits(0, numStarsBars, 4, allPools, newPool); //Add rivers
		setCardBits(0, numStarsBars - 1, 3, allPools, newPool); //Add turns
		setCardBits(0, numStarsBars - 2, 2, allPools, newPool); //Add flops
	}
}

void InfoSet::getAllHandID(std::vector<HandID>& allHands) 
{
#ifdef _DEBUG
	assert(allHands.size() == 0);
#endif
	for (int hand = 0; hand < (int)HandID::NUM_HAND_COMBOS; hand++) 
	{
		if (garbageHands.find((HandID)hand) == garbageHands.end()) {
			allHands.push_back((HandID)hand);
		}
	}

	allHands.push_back(HandID::GARBAGE_HAND);
}

void InfoSet::updateHandDist(const BitSet& infoKey, int currTurn, std::vector<float>& oppoHandLikeli)
{

}

void InfoSet::getStrat(const BitSet& infoKey, HandID hand, std::vector<float>& strat)
{

}

void InfoSet::writeRegrets()
{

}

void InfoSet::addAction(BitSet& infoKey, ActionClass act, int depth)
{
	int BIT_FLAGS[ACT_BITS] = { 0x1, 0x2, 0x4 };
	int startBit = CARD_KEY_SIZE + depth * ACT_BITS;

	for (int i = 0; i < ACT_BITS; i++) 
	{
		infoKey.set(startBit + i, (int)act & BIT_FLAGS[i]);
	}
}

void InfoSet::removeAction(BitSet& infoKey, int depth)
{
	int startBit = CARD_KEY_SIZE + depth * ACT_BITS;
	for (int i = 0; i < ACT_BITS; i++) 
	{
		infoKey.set(startBit + i, 0);
	}
}

void InfoSet::initRegret(const BitSet& infoKey, int numLegalActions)
{
	regrets.emplace(infoKey, Simp2DArray<Regret>(NUM_REAL_HANDS, numLegalActions));
}

void InfoSet::getPoolID(Pool& pool, Phase currPhase, BitSet& infoKey) 
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

HandID InfoSet::getHandID(Hand& currHand) 
{
	int handID;
	int card1Num = currHand.cards[0].number <= currHand.cards[1].number ? currHand.cards[0].number : currHand.cards[1].number;
	int card2Num = card1Num == currHand.cards[0].number ? currHand.cards[1].number : currHand.cards[0].number;
	
	handID = card1Num * NUM_CARD_NUMBERS - sigma0(card1Num);
	handID += card2Num;

	if (currHand.cards[0].suit == currHand.cards[1].suit) 
	{
		handID += sigma0(NUM_CARD_NUMBERS) - (card1Num + 1);
	}

	return (HandID)handID;
}

void printAction(ActionClass action)
{
	switch(action)
	{
	case ActionClass::FOLD:
		std::cout << "folds";
		break;
	case ActionClass::CALL:
		std::cout << "calls";
		break;
	case ActionClass::ALL_IN:
		std::cout << "goes all in";
		break;
		/*
	case ActionClass::RAISE_HALF:
		std::cout << "raises half pot";
		break;
	case ActionClass::RAISE_POT:
		std::cout << "raises pot";
		break;
		*/
	case ActionClass::OPEN1:
		std::cout << "open raises";
		break;
	case ActionClass::OPEN2:
		std::cout << "open2 raises";
		break;
	case ActionClass::OPEN3:
		std::cout << "open raises";
		break;
	case ActionClass::BET3:
		std::cout << "3 bets";
		break;
	case ActionClass::BET4:
		std::cout << "4 bets";
		break;
	default:
		std::cout << "Other action: " << (int)action;
		break;
	}
	std::cout << std::endl;
}

const char* numToCard(int cardNum) 
{
	switch (cardNum)
	{
	case 0:
		return "Two";
	case 1:
		return "Three";
	case 2:
		return "Four";
	case 3:
		return "Five";
	case 4:
		return "Six";
	case 5:
		return "Seven";
	case 6:
		return "Eight";
	case 7:
		return "Nine";
	case 8:
		return "Ten";
	case 9:
		return "Jack";
	case 10:
		return "Queen";
	case 11:
		return "King";
	case 12:
		return "Ace";
	default:
		return "UnknownCard";
	}
}

void InfoSet::printHandID(HandID hand) 
{
	if (hand == HandID::GARBAGE_HAND) 
	{
		std::cout << "Garbage Hand " << std::endl;
		return;
	}

	int card1Num = -1;
	int card2Num = -1;
	bool suited = false;

	if ((int)hand > (int)HandID::ACAC) 
	{ //suited
		suited = true;
		hand = (HandID)((int)hand - (int)HandID::TWTHs);
	} 

	int currSum = 0;
	int suitShift = suited ? 1 : 0;

	for (int i = NUM_CARD_NUMBERS - suitShift; i > 0; i--) 
	{
		if ((int)hand < currSum + i) 
		{
			card1Num = (NUM_CARD_NUMBERS - suitShift) - i;
			card2Num = (int)hand - currSum + card1Num + suitShift;
			break;
		}
		currSum += i;
	}

	if (card1Num != card2Num) {
		std::cout << numToCard(card2Num) << " ";
		std::cout << numToCard(card1Num) << " ";

		if (suited)
		{
			std::cout << "Suited";
		}
		else
		{
			std::cout << "Offsuit";
		}
		std::cout << std::endl;
	} else 
	{
		std::cout << "Pair of " << numToCard(card1Num) << "s" << std::endl;
	}
}

void InfoSet::printPoolID(const BitSet& pool) 
{
	for (int j = 0; j < InfoSet::CARD_KEY_SIZE; j++) {
		std::cout << pool[j];
	}
	std::cout << std::endl;
}

const char *actionToString(const InfoSet::BitSet& currAct) 
{
	switch(currAct.to_ulong()) 
	{
	case 0:
		return "Null Action (shouldn't happen)";
	case 1:
		return "Fold";
	case 2:
		return "Call";
	case 3:
		return "All In";
	case 4:
		return "Open1";
	case 5:
		return "Open2";
	case 6:
		return "Open3";
	case 7:
		return "3Bet";
	case 8:
		return "4Bet";
	default:
		return "UnknownAction";
	}
}

void InfoSet::printKeyInfo(const BitSet& key) 
{
	std::cout << "The current pool is ";
	printPoolID(key);

	std::cout << "The action sequence is ";
	int currShift = CARD_KEY_SIZE;
	BitSet currFlag(0x7 << currShift);
	BitSet currAct = (currFlag & key) >> currShift;

	while (currAct.to_ulong() != (unsigned long)ActionClass::NULL_ACTION)
	{
		std::cout << actionToString(currAct) << " ";

		currFlag <<= ACT_BITS;
		currShift += ACT_BITS;
		currAct = (currFlag & key) >> currShift;
	} 

	std::cout << std::endl;
}
