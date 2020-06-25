#include "TestPokerCards.h"

void checkHand(CardEnum card1, CardEnum card2, HandID expected)
{
	Hand testHand(card1, card2);
	assert(getHandID(testHand) == expected);
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
	std::bitset<INFO_KEY_SIZE> poolID;
	getPoolID(testPool, currPhase, poolID);
	std::bitset<INFO_KEY_SIZE> expectedSet(expected);

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
