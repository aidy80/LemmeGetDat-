#include "TestInfoSet.h"

void checkHand(CardEnum card1, CardEnum card2, HandID expected)
{
	Hand testHand(card1, card2);
	assert(InfoSet::getHandID(testHand) == expected);
}

void testHandID()
{
	std::cout << "Runnigng handID tests" << std::endl;
	checkHand(CardEnum::TWH, CardEnum::TWD, HandID::TWTW);

	checkHand(CardEnum::TWH, CardEnum::THD, HandID::TWTHo);

	checkHand(CardEnum::TWH, CardEnum::FOD, HandID::TWFOo);

	checkHand(CardEnum::TWS, CardEnum::ACC, HandID::TWACo);

	checkHand(CardEnum::TWS, CardEnum::ACC, HandID::TWACo);

	checkHand(CardEnum::THS, CardEnum::THC, HandID::THTH);

	checkHand(CardEnum::THS, CardEnum::FIC, HandID::THFIo);

	checkHand(CardEnum::THS, CardEnum::FIC, HandID::THFIo);

	checkHand(CardEnum::THS, CardEnum::KIC, HandID::THKIo);

	checkHand(CardEnum::FOC, CardEnum::FOD, HandID::FOFO);

	checkHand(CardEnum::SEC, CardEnum::KIS, HandID::SEKIo);

	checkHand(CardEnum::QUC, CardEnum::ACS, HandID::QUACo);

	checkHand(CardEnum::ACS, CardEnum::ACD, HandID::ACAC);

	checkHand(CardEnum::TWC, CardEnum::THC, HandID::TWTHs);

	checkHand(CardEnum::FOC, CardEnum::THC, HandID::THFOs);
	
	checkHand(CardEnum::ACS, CardEnum::SES, HandID::SEACs);

	checkHand(CardEnum::KIS, CardEnum::ACS, HandID::KIACs);
	
	std::cout << "PASSED all HandID tests!" << std::endl;
}

void checkPool(CardEnum poolCards[5], Phase currPhase, int32_t expected) 
{
	Pool testPool(poolCards);
	std::bitset<InfoSet::INFO_KEY_SIZE> poolID;
	InfoSet::getPoolID(testPool, currPhase, poolID);
	std::bitset<InfoSet::INFO_KEY_SIZE> expectedSet(expected);

	if (poolID != expectedSet) 
	{
		std::cout << "The pool " << poolID.to_ulong() << " did not match the expected " << expectedSet.to_ulong() << std::endl;
		assert(false);
	}
}

void testPoolID()
{
	std::cout << "Testing Pool ID" << std::endl;
	CardEnum suitedPool[5] = { CardEnum::TWC, CardEnum::ACC, CardEnum::TED, CardEnum::SEC, CardEnum::JAC };

	checkPool(suitedPool, Phase::FLOP, 0x14201);
	checkPool(suitedPool, Phase::TURN, 0x28441);
	checkPool(suitedPool, Phase::RIVER, 0x51441);

	CardEnum rainPool[5] = { CardEnum::TWC, CardEnum::TWS, CardEnum::TWD, CardEnum::THC, CardEnum::FOC };

	checkPool(rainPool, Phase::FLOP, 0x7);
	checkPool(rainPool, Phase::TURN, 0x17);
	checkPool(rainPool, Phase::RIVER, 0x40057);

	std::cout << "Pool ID Passed!!" << std::endl;
}

void testAllPoolID()
{
	InfoSet info;

	std::vector<InfoSet::BitSet> allPools;
	info.getAllPoolID(allPools);

	for (int i = 0; i < 10; i++) {
		InfoSet::printPoolID(allPools.at(i));
	}

	std::unordered_set<InfoSet::BitSet, std::hash<InfoSet::BitSet>> allRiverPools, allTurnPools, allFlopPools;
	int suitedBit = NUM_CARD_NUMBERS + NUM_POOL_CARDS - 1;
	for (InfoSet::BitSet bset : allPools) 
	{
		if (bset[suitedBit] == 1 && bset.count() == 6 || 
			bset[suitedBit] == 0 && bset.count() == 5) 
		{
			allRiverPools.insert(bset);
		}
		if (bset[suitedBit] == 1 && bset.count() == 5 || 
			bset[suitedBit] == 0 && bset.count() == 4) 
		{
			allTurnPools.insert(bset);
		}
		if (bset[suitedBit] == 1 && bset.count() == 4 || 
			bset[suitedBit] == 0 && bset.count() == 3) 
		{
			allFlopPools.insert(bset);
		}
	}

	std::cout << "Actual: " << allRiverPools.size() << " Expected: " << nCr(NUM_CARD_NUMBERS + NUM_POOL_CARDS - 1, NUM_POOL_CARDS) * 2 - NUM_CARD_NUMBERS * 2 << std::endl;
	assert((int)allRiverPools.size() == nCr(NUM_CARD_NUMBERS + NUM_POOL_CARDS - 1, NUM_POOL_CARDS) * 2 - NUM_CARD_NUMBERS * 2);
	std::cout << "Actual: " << allTurnPools.size() << " Expected: " << nCr(NUM_CARD_NUMBERS + NUM_POOL_CARDS - 2, NUM_POOL_CARDS - 1) * 2 << std::endl;
	assert((int)allTurnPools.size() == nCr(NUM_CARD_NUMBERS + NUM_POOL_CARDS - 2, NUM_POOL_CARDS - 1) * 2);
	std::cout << "Actual: " << allFlopPools.size() << " Expected: " << nCr(NUM_CARD_NUMBERS + NUM_FLOP_CARDS - 1, NUM_FLOP_CARDS) * 2 << std::endl;
	assert((int)allFlopPools.size() == nCr(NUM_CARD_NUMBERS + NUM_FLOP_CARDS - 1, NUM_FLOP_CARDS) * 2);

	for (auto iter = allRiverPools.begin(); iter != allRiverPools.end(); iter++) {
		int numInRow = 0;
		for (int j = 0; j < suitedBit; j++) {
			if ((*iter)[j])
			{
				numInRow++;
			} else 
			{
				numInRow = 0;
			}
			assert(numInRow != 5);
		}
	}
}

void testAllHandID() 
{
	InfoSet info;

	std::vector<HandID> allHands;
	info.getAllHandID(allHands);

	for (HandID hand : allHands) 
	{
		std::cout << "HandID " << (int)hand << ", written as ";
		InfoSet::printHandID(hand);
	}
}

void testPrintKeyInfo()
{
	std::cout << "Print Key Test\n";
	CardEnum suitedPool[5] = { CardEnum::TWC, CardEnum::ACC, CardEnum::TED, CardEnum::SEC, CardEnum::JAC };
	Pool testPool(suitedPool);

	InfoSet::BitSet key;
	InfoSet::getPoolID(testPool, Phase::RIVER, key);

	InfoSet::addAction(key, ActionClass::FOLD, 0);
	InfoSet::addAction(key, ActionClass::CALL, 1);
	InfoSet::addAction(key, ActionClass::OPEN1, 2);
	InfoSet::addAction(key, ActionClass::BET3, 3);
	InfoSet::addAction(key, ActionClass::BET3, 4);

	InfoSet::printKeyInfo(key);
}


void testInfoSet() 
{
	testHandID();
	testPoolID();
	testAllPoolID();
	testAllHandID();
	testPrintKeyInfo();
}
