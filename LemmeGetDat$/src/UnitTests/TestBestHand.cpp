#include "TestBestHand.h"

void testBestHand(Pool& pool, Hand *hands, std::string description, std::vector<std::vector<char>> expected, std::optional<std::vector<std::vector<int>>> pots = std::nullopt) 
{
	TwoDimArray bestHands(expected.size(), expected[0].size());
	if (pots.has_value()) {
		for (unsigned int i = 0; i < pots->size(); i++) {
			for (unsigned int j = 0; j < pots->at(i).size(); j++) {
				bestHands.set(i, j, pots->at(i).at(j));
			}
		}
	}
	else {
		for (unsigned int i = 0; i < expected.size(); i++) {
			for (unsigned int j = 0; j < expected[i].size(); j++) {
				bestHands.set(i, j, j);
			}
		}
	}

	getBestHands(pool, hands, bestHands);
	std::cout << description << "\nExpected: \n";
	for (unsigned int i = 0; i < expected.size(); i++) {
		std::cout << "| ";
		for (unsigned int j = 0; j < expected[i].size(); j++) {
			std::cout << (int)expected[i][j] << " ";
		}
		std::cout << "|\n";
	}
	std::cout << "\nActual: \n";

	bool failed = false;;
	for (unsigned int i = 0; i < expected.size(); i++) {
		std::cout << "| ";
		for (unsigned int j = 0; j < expected[i].size(); j++) {
			std::cout << (int)bestHands.get(i, j) << " ";
			if (bestHands.get(i, j) != expected[i][j]) {
				failed = true;
			}
		}
		std::cout << "|\n";
	}

	if (failed) {
		std::cout << "FAILED" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "PASSED" << std::endl;
	}
	std::cout << std::endl;
}

void testStraightFlushBestHand()
{
	std::cout << "STRAIGHT FLUSH TEST" << std::endl;
	CardEnum strFlushOnBoard[5] = { CardEnum::NIH, CardEnum::KIH, CardEnum::QUH, CardEnum::JAH, CardEnum::TEH };
	Pool pool(strFlushOnBoard);

	CardEnum initHands[2][2] = { {CardEnum::TWH, CardEnum::THH}, {CardEnum::FOH, CardEnum::EIH} };
	Hand hand[2] = { initHands[0], initHands[1] };
	//testBestHand(pool, hand,
	//	"straight flush on board. Neither have a higher kicker", { { 0, 1 } });

	hand[0].setHand(CardEnum::ACH, CardEnum::EIH);
	testBestHand(pool, hand,
		"straight flush on board. Player 0 has the high kicker", { { 0, -1 } });

	CardEnum fourGutStrFlush[5] = { CardEnum::NIH, CardEnum::EIH, CardEnum::SIH, CardEnum::FIH, CardEnum::SEC };
	pool.setCards(fourGutStrFlush);

	hand[0].setHand(CardEnum::SEH, CardEnum::THH);
	hand[1].setHand(CardEnum::TEH, CardEnum::SED);
	testBestHand(pool, hand,
		"4 card gutshot straight flush on board. Player 0 has the gutshot", { { 0, -1 } });

	CardEnum thrGutStrFlush[5] = { CardEnum::NIS, CardEnum::EIH, CardEnum::SIH, CardEnum::FIH, CardEnum::SEC };
	pool.setCards(thrGutStrFlush);

	hand[0].setHand(CardEnum::SEH, CardEnum::FOH);
	hand[1].setHand(CardEnum::TEH, CardEnum::SED);
	testBestHand(pool, hand,
		"3 card gutshot straight flush on board. Player 0 has the gutshot and low card", { { 0, -1 } });

	CardEnum thrGutStrFlush2[5] = { CardEnum::NIS, CardEnum::EIH, CardEnum::SIH, CardEnum::FIH, CardEnum::FIC };
	pool.setCards(thrGutStrFlush2);

	hand[0].setHand(CardEnum::SEH, CardEnum::NIH);
	hand[1].setHand(CardEnum::FIS, CardEnum::FID);
	testBestHand( pool, hand,
		"3 card gutshot straight flush on board 2.0. Player 0 has the gutshot and high card", { { 0, -1 } });

	CardEnum openStrFlush[5] = { CardEnum::NIS, CardEnum::SEH, CardEnum::SIH, CardEnum::FIH, CardEnum::FIC };
	pool.setCards(openStrFlush);

	hand[0].setHand(CardEnum::TES, CardEnum::SED);
	hand[1].setHand(CardEnum::EIH, CardEnum::FOH);
	testBestHand( pool, hand,
		"3 card open ended straight flush on board. Player 1 has the low and high kicker", { { 1, -1 } });

	hand[0].setHand(CardEnum::FIS, CardEnum::FID);
	testBestHand( pool, hand,
		"3 card open ended straight flush on board. Player 0 has quads, player 1 has strflush", { { 1, -1 } });

	hand[0].setHand(CardEnum::EIH, CardEnum::NIH);
	hand[1].setHand(CardEnum::THH, CardEnum::FOH);
	testBestHand( pool, hand,
		"3 card open ended straight flush on board. Player 1 has the low suited kickers, player 0 has high unsuited kickers", { { 0, -1 } });

	hand[0].setHand(CardEnum::THH, CardEnum::FOH);
	hand[1].setHand(CardEnum::FIS, CardEnum::FIC);
	testBestHand( pool, hand,
		"3 card open ended straight flush on board. Player 1 has the low suited kickers, player 0 has high unsuited kickers", { { 0, -1 } });

	std::cout << "\n\n" << std::endl;
}

void testQuads() 
{
	std::cout << "QUADS TEST" << std::endl;
	CardEnum tripleOnBoard[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::SEC, CardEnum::TED, CardEnum::EID };
	Pool pool(tripleOnBoard);
	Hand hand[2];

	hand[0].setHand(CardEnum::SED, CardEnum::EIC);
	hand[1].setHand(CardEnum::TWH, CardEnum::TEC);
	testBestHand( pool, hand,
		"pair on board. player 0 has quads, player 1 has a house", { { 0, -1 } });

	CardEnum quadsOnBoard[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::SEC, CardEnum::SED, CardEnum::EID };
	pool.setCards(quadsOnBoard);

	hand[0].setHand(CardEnum::TWC, CardEnum::THC);
	hand[1].setHand(CardEnum::TWD, CardEnum::THD);
	testBestHand( pool, hand,
		"quads on board. Neither player has a higher kicker", { { 0, 1 } });

	hand[0].setHand(CardEnum::ACS, CardEnum::THC);
	hand[1].setHand(CardEnum::KIS, CardEnum::THD);
	testBestHand( pool, hand,
			"quads on board. player 0 has a higher kicker", { { 0, -1 } });

	hand[0].setHand(CardEnum::KIC, CardEnum::KIC);
	hand[1].setHand(CardEnum::KIS, CardEnum::THD);
	testBestHand( pool, hand,
		"quads on board. both have same top kicker, player 0 has a higher kicker2", { { 0, 1 } });

	hand[0].setHand(CardEnum::EIS, CardEnum::TWH);
	hand[1].setHand(CardEnum::EIC, CardEnum::EIH);
	testBestHand( pool, hand,
		"quads on board. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	CardEnum quadsOnBoard2[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::SEC, CardEnum::SED, CardEnum::SID };
	pool.setCards(quadsOnBoard2);

	hand[0].setHand(CardEnum::SIS, CardEnum::TWH);
	hand[1].setHand(CardEnum::SIC, CardEnum::SIH);
	testBestHand( pool, hand,
		"quads on board 2.0. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	hand[0].setHand(CardEnum::FIH, CardEnum::TWH);
	hand[1].setHand(CardEnum::FIC, CardEnum::FID);
	testBestHand( pool, hand,
		"quads on board 2.0. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	CardEnum twoPairOnBoard[5] = { CardEnum::SEH, CardEnum::KIH, CardEnum::SES, CardEnum::EIC, CardEnum::EID };
	pool.setCards(twoPairOnBoard);

	hand[0].setHand(CardEnum::SED, CardEnum::SEC);
	hand[1].setHand(CardEnum::EIH, CardEnum::EIS);
	testBestHand( pool, hand,
		"Two pair on board. player 1 has a higher quads", { { 1, -1 } });
}

void testHouse()
{
	std::cout << "HOUSE TEST" << std::endl;
	CardEnum tripleOnBoard[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::SEC, CardEnum::TED, CardEnum::EID };
	Pool pool(tripleOnBoard);
	Hand hand[2];

	hand[0].setHand(CardEnum::EIC, CardEnum::SIS); 
	hand[1].setHand(CardEnum::TWH, CardEnum::THC);

	testBestHand( pool, hand,
		"triple on Board. player 0 has the house player 1 does not have it", { { 0, -1 } });

	hand[1].setHand(CardEnum::TEH, CardEnum::TWH);
	testBestHand( pool, hand,
		"triple on Board. player 1 has the higher pair", { { 1, -1 } });

	hand[0].setHand(CardEnum::TES, CardEnum::TEC);
	testBestHand( pool, hand,
		"triple on Board. player 0 has a higher three of a kind", { { 0, -1 } });

	hand[0].setHand(CardEnum::QUD, CardEnum::QUC);
	hand[1].setHand(CardEnum::EIC, CardEnum::TEC);
	testBestHand(pool, hand,
		"triple on board. player 0 has pocket pair resulting in higher house", { { 0, -1 } });

	CardEnum pairOnBoard[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::SIS, CardEnum::TED, CardEnum::EID };
	pool.setCards(pairOnBoard);

	hand[0].setHand(CardEnum::TWH, CardEnum::THC);
	hand[1].setHand(CardEnum::SEC, CardEnum::TEC);
	testBestHand( pool, hand,
		"pair on board. player 1 has the house, player 0 does not", { { 1, -1 } });

	CardEnum twoPairOnBoard[5] = { CardEnum::SEH, CardEnum::KIH, CardEnum::SES, CardEnum::EIC, CardEnum::EID };
	pool.setCards(twoPairOnBoard);

	hand[0].setHand(CardEnum::SEC, CardEnum::TWH);
	hand[1].setHand(CardEnum::EIH, CardEnum::TWC);
	testBestHand( pool, hand,
		"two pair on board. player 1 has higher three of a kind than player 0", { { 1, -1 } });

	hand[0].setHand(CardEnum::EIS, CardEnum::ACS);
	testBestHand( pool, hand,
		"two pair on board. both players have same house, player 0 has higher kicker", { { 0, 1 } });

	CardEnum houseVFlush[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::TEH, CardEnum::TED, CardEnum::EIH };
	pool.setCards(houseVFlush);

	hand[0].setHand(CardEnum::SEC, CardEnum::TWH);
	hand[1].setHand(CardEnum::ACH, CardEnum::KIH);
	testBestHand( pool, hand,
		"two pair on board. player 0 has the house, player 1 has a flush", { { 0, -1 } });

	CardEnum houseOnBoard[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::SEC, CardEnum::TED, CardEnum::TEH };
	pool.setCards(houseOnBoard);

	hand[0].setHand(CardEnum::ACS, CardEnum::KIC);
	hand[1].setHand(CardEnum::QUC, CardEnum::JAC);
	testBestHand( pool, hand,
		"house on board. Both players have higher kickers", { { 0, 1 } });

	hand[0].setHand(CardEnum::JAD, CardEnum::JAH);
	testBestHand( pool, hand,
		"house on board. player 0 has a higher pocket pair", { { 0, -1 } });

	hand[1].setHand(CardEnum::TWH, CardEnum::TEC);
	testBestHand( pool, hand,
		"house on board. player 1 has a higher triple", { { 1, -1 } });
}



void testFlushBestHand()
{
	std::cout << "FLUSH BEST HAND TEST\n";

	CardEnum clubFlushPool[5] = { CardEnum::ACC, CardEnum::JAC, CardEnum::SEC, CardEnum::KIC, CardEnum::EIC };
	Pool pool(clubFlushPool);

	CardEnum initList[2][2] = { { CardEnum::QUC, CardEnum::EID }, { CardEnum::NIS, CardEnum::ACS } };
	Hand hand[2] = { initList[0], initList[1] };
	testBestHand(pool, hand,
		"Club Flush on pool, queen kicker for player 0. No kicker for player 1", {{ 0, -1 }});

	hand[0].setHand(CardEnum::NIS, CardEnum::ACS);
	hand[1].setHand(CardEnum::QUC, CardEnum::EID);
	testBestHand(pool, hand,
		"Club Flush on pool, queen kicker for player 1. No kicker for player 0", {{ 1, -1 }});

	hand[0].setHand(CardEnum::NIC, CardEnum::ACS);
	hand[1].setHand(CardEnum::QUC, CardEnum::EID);
	testBestHand(pool, hand,
		"Club Flush on pool, queen kicker for player 1, nine kicker for player 0", {{ 1, -1 } });

	hand[0].setHand(CardEnum::TWC, CardEnum::KIC);
	hand[1].setHand(CardEnum::QUC, CardEnum::NIC);
	testBestHand(pool, hand,
		"Club Flush on pool, queen and nine kickers for player 1, eight and king kickers for player 0", { { 0, -1 } });

	hand[0].setHand(CardEnum::THC, CardEnum::TWC);
	hand[1].setHand(CardEnum::FIC, CardEnum::FOC);
	testBestHand(pool, hand,
		"Club Flush on pool, both players have two clubs but smaller than those on board", { { 0, 1 } });

	hand[0].setHand(CardEnum::TWS, CardEnum::THS);
	hand[1].setHand(CardEnum::THH, CardEnum::TWH);
	testBestHand(pool, hand,
		"Club Flush on pool, nobody has clubs", {{ 0, 1 }});

	CardEnum threeHeartPool[5] = { CardEnum::ACH, CardEnum::JAH, CardEnum::THH, CardEnum::KIC, CardEnum::TEC };
	pool.setCards(threeHeartPool);

	hand[0].setHand(CardEnum::QUC, CardEnum::EID);
	hand[1].setHand(CardEnum::NIH, CardEnum::JAC);
	testBestHand(pool, hand,
		"Three hearts on pool, Nobody has two hearts", {{ 0, -1 } });

	hand[0].setHand(CardEnum::QUC, CardEnum::EID);
	hand[1].setHand(CardEnum::NIH, CardEnum::QUH);
	testBestHand(pool, hand,
		"Three hearts on pool, player 1 has two hearts, player 0 had none", {{ 1, -1 } });

	hand[0].setHand(CardEnum::QUC, CardEnum::EIH);
	hand[1].setHand(CardEnum::NIH, CardEnum::QUH);
	testBestHand( pool, hand,
		"Three hearts on pool, player 1 has two hearts, player 0 had one", { {1, -1} });

	CardEnum thrGutStrFlush[5] = { CardEnum::NIS, CardEnum::EIH, CardEnum::SIH, CardEnum::FIH, CardEnum::SEC };
	pool.setCards(thrGutStrFlush);

	hand[0].setHand(CardEnum::SEC, CardEnum::TED);
	hand[1].setHand(CardEnum::THH, CardEnum::TWH);
	testBestHand(pool, hand,
		"Three hearts on pool, player 1 has flush, player 0 has straight", { {1, -1} });

	std::cout << "\n\n" << std::endl;
}

void testStraightBestHand()
{
	std::cout << "TESTING STRAIGHT BEST HAND\n";

	CardEnum straightOnBoard[5] = { CardEnum::FOC, CardEnum::FIC, CardEnum::SID, CardEnum::SED, CardEnum::EIS };
	Pool pool(straightOnBoard);

	CardEnum initCardEnums[2][2] = { {CardEnum::TWS, CardEnum::TWH}, {CardEnum::THC, CardEnum::THH} };
	Hand hand[2] = { initCardEnums[0], initCardEnums[1] };

	testBestHand(pool, hand,
		"straight on board. no player has a valid kicker, but player 1 does have an under connecting card", { { 0, 1 } });

	hand[0].setHand(CardEnum::TWH, CardEnum::NIC);
	hand[1].setHand(CardEnum::THC, CardEnum::TEC);
	testBestHand(pool, hand,
		"straight on board. no player 0 has a high kicker, player 1 does not", { { 0, -1 } });

	hand[1].setHand(CardEnum::NID, CardEnum::TEH);
	testBestHand(pool, hand,
		"straight on board. no player 0 has a high kicker, player 1 has two high kickers", { { 1,-1 } });

	CardEnum gutShotStraight[5] = { CardEnum::EIS, CardEnum::FOC, CardEnum::FIC, CardEnum::SID, CardEnum::ACS };
	pool.setCards(gutShotStraight);

	hand[0].setHand(CardEnum::SEC, CardEnum::JAH);
	hand[1].setHand(CardEnum::TWC, CardEnum::EID);
	testBestHand(pool, hand,
		"gut shot straight. no player 0 has a gutshot, player 1 does not", { { 0, -1 } });

	hand[1].setHand(CardEnum::SEH, CardEnum::NIH);
	testBestHand(pool, hand,
		"gut shot straight. no player 0 has a gutshot, player 1 has gutshot and high kicker", { { 1, -1 } });

	CardEnum openEndedStraight[5] = { CardEnum::FOC, CardEnum::SID, CardEnum::TED, CardEnum::FIC, CardEnum::ACS };
	pool.setCards(openEndedStraight);

	hand[0].setHand(CardEnum::TEC, CardEnum::TWH);
	hand[1].setHand(CardEnum::THC, CardEnum::KIC);
	testBestHand(pool, hand,
		"3-card open ended straight. neither player has it", { { 0, -1 } });

	hand[0].setHand(CardEnum::THD, CardEnum::JAD);
	hand[1].setHand(CardEnum::THC, CardEnum::SEH);
	testBestHand(pool, hand,
		"3-card open ended straight. player 1 has it with one under, one over. player 0 does not", { { 1, -1 } });

	hand[0].setHand(CardEnum::SES, CardEnum::EIS);
	testBestHand(pool, hand,
		"3-card open ended straight. player 1 has it with one under, one over. player 0 has it with two over", { { 0, -1 } });

	hand[1].setHand(CardEnum::SED, CardEnum::EID);
	testBestHand(pool, hand,
		"3-card open ended straight. player 0 and 1 have same cards (both straights)", { { 0, 1 } });

	hand[0].setHand(CardEnum::TWH, CardEnum::THD);
	hand[1].setHand(CardEnum::ACH, CardEnum::ACC);
	testBestHand( pool, hand,
		"3-card open ended straight. player 0 has the striaight, player 1 has a triple", { { 0, -1 } });

	std::cout << "\n\n" << std::endl;
}

void testTriple() 
{
	std::cout << "TRIPLE TEST" << std::endl;
	CardEnum onlySingles[5] = { CardEnum::SEH, CardEnum::KIH, CardEnum::QUC, CardEnum::TED, CardEnum::EID };
	Pool pool(onlySingles);

	CardEnum initList[2][2] = { {CardEnum::SEC, CardEnum::SED}, {CardEnum::TWC, CardEnum::THC} };
	Hand hand[2] = { initList[0], initList[1] };

	testBestHand( pool, hand,
		"onlySingles. player 0 has pocket pair creating a triple", { { 0, -1 } });

	hand[1].setHand(CardEnum::TEC, CardEnum::TEH);
	testBestHand( pool, hand,
		"onlySingles. player 0 has pocket pair creating a triple, but player 1 has a higher one", { { 1, -1 } });

	CardEnum pairOnBoard[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::QUC, CardEnum::TED, CardEnum::EID };
	pool.setCards(pairOnBoard);

	hand[0].setHand(CardEnum::SEC, CardEnum::TWH);
	hand[1].setHand(CardEnum::SIS, CardEnum::TWC);
	testBestHand(pool, hand,
		"pairOnBoard. player 0 has card creating triple, but player 1 does not", { { 0, -1 } });

	hand[1].setHand(CardEnum::SED, CardEnum::THC);
	testBestHand( pool, hand,
		"pairOnBoard. both have cards creating triples. player 1 has a higher kicker but both lower than those on board", { { 0, 1 } });

	hand[0].setHand(CardEnum::SEC, CardEnum::TWH);
	hand[1].setHand(CardEnum::SED, CardEnum::JAS);
	testBestHand( pool, hand,
		"pairOnBoard. both have cards creating triples, but player 1 has a higher valid kicker", { { 1, -1 } });

	hand[0].setHand(CardEnum::SEC, CardEnum::JAC);
	testBestHand( pool, hand,
		"pairOnBoard. players have same hand", { { 0, 1 } });

	hand[1].setHand(CardEnum::QUD, CardEnum::TWC);
	testBestHand( pool, hand,
		"pairOnBoard. player 0 has a triple, player 1 has a two pair", { { 0, -1 } });

	CardEnum noPairOnBoard[5] = { CardEnum::SEH, CardEnum::FOD, CardEnum::QUC, CardEnum::TED, CardEnum::EID };
	pool.setCards(noPairOnBoard);

	hand[0].setHand(CardEnum::TEH, CardEnum::TEC);
	hand[1].setHand(CardEnum::SEC, CardEnum::SED);
	testBestHand( pool, hand,
		"noPairOnBoard. player 0 has a higher pocket pair resulting in higher triple", { { 0, -1 } });

	CardEnum tripleOnBoard[5] = { CardEnum::SEH, CardEnum::SES, CardEnum::SEC, CardEnum::TED, CardEnum::EID };
	pool.setCards(tripleOnBoard);

	hand[0].setHand(CardEnum::QUD, CardEnum::JAS);
	hand[1].setHand(CardEnum::QUC, CardEnum::JAC);
	testBestHand( pool, hand,
		"tripleOnBoard. players have same hand", { { 0, 1 } });

	hand[0].setHand(CardEnum::ACC, CardEnum::JAS);
	testBestHand( pool, hand,
		"tripleOnBoard. player 0 has a higher kicker", { { 0, -1 } });

	hand[0].setHand(CardEnum::QUD, CardEnum::JAS);
	hand[1].setHand(CardEnum::QUC, CardEnum::NID);
	testBestHand( pool, hand,
		"tripleOnBoard. players 0 and 1 have same higher kicker but player 0 has higher kicker2", { { 0, -1 } });
}

void testTwoPairs()
{
	std::cout << "TWO PAIR TEST" << std::endl;
	CardEnum noPairOnBoard[5] = { CardEnum::SEH, CardEnum::KIH, CardEnum::QUC, CardEnum::ACS, CardEnum::EID };
	Pool pool(noPairOnBoard);

	CardEnum initCardEnums[2][2] = { { CardEnum::KIS, CardEnum::TWH }, { CardEnum::QUH, CardEnum::EIC } };
	Hand hand[2] = {initCardEnums[0], initCardEnums[1]};
	testBestHand(pool, hand,
    		"no pair on board. Player 0 has one pair, player 1 two pairs", { { 1, -1 } });

	hand[0].setHand(CardEnum::QUS, CardEnum::EIS);
	testBestHand(pool, hand,
    		"no pair on board. Players have the same hands", { { 0, 1 } });

	hand[0].setHand(CardEnum::EIS, CardEnum::ACS);
	testBestHand(pool, hand,
    		"no pair on board. Player 0 has a higher two pair than player 1", { { 0, -1 } });

	CardEnum onePairOnBoard[5] = { CardEnum::SEH, CardEnum::SEC, CardEnum::QUC, CardEnum::ACS, CardEnum::FOD };
	pool.setCards(onePairOnBoard);

	hand[0].setHand(CardEnum::SIS, CardEnum::TWC);
	hand[1].setHand(CardEnum::QUD, CardEnum::FOD);
	testBestHand(pool, hand,
    		"one pair on board. Player 1 has a higher second pair", { { 1, -1 } });

	hand[0].setHand(CardEnum::QUH, CardEnum::KIH);
	testBestHand(pool, hand,
    		"one pair on board. Players have the same second pair but player 0 has better kicker", { { 0, 1 } });

	hand[0].setHand(CardEnum::QUH, CardEnum::FOC);
	testBestHand(pool, hand,
    		"one pair on board. Players have the same hand", { { 0, 1 } });

	CardEnum twoPairOnBoard[5] = { CardEnum::SEH, CardEnum::KIH, CardEnum::SEC, CardEnum::EIC, CardEnum::EID };
	pool.setCards(twoPairOnBoard);

	hand[0].setHand(CardEnum::TWH, CardEnum::THH);
	hand[1].setHand(CardEnum::KIS, CardEnum::TWC);
	testBestHand(pool, hand,
    		"two pairs on board. Player 1 has a king creating a pair", { { 1, -1 } });

	hand[0].setHand(CardEnum::KIC, CardEnum::SIC);
	hand[1].setHand(CardEnum::KIS, CardEnum::TWC);
	testBestHand(pool, hand,
		"two pairs on board, both players have a king creating a pair. "
		"Player 0 has a higher kicker but still lower than 3rd pair", { {0, 1} });

	hand[0].setHand(CardEnum::ACS, CardEnum::ACC);
	testBestHand(pool, hand,
    		"two pairs on board. Player 0 has a pocket aces", { { 0, -1 } });

	hand[0].setHand(CardEnum::ACD, CardEnum::TWC);
	hand[1].setHand(CardEnum::QUC, CardEnum::THH);
	testBestHand(pool, hand,
    		"two pairs on board. Player 0 has a better first kicker", { { 0, -1 } });

	hand[0].setHand(CardEnum::ACD, CardEnum::JAD);
	hand[1].setHand(CardEnum::ACS, CardEnum::TED);
	testBestHand(pool, hand,
    		"two pairs on board. players have same first kicker but player 1 has a better second kicker", { { 0, 1 } });
}



void testPairs()
{
	std::cout << "PAIR TEST" << std::endl;
	CardEnum aHighishBoard[5] = { CardEnum::SEH, CardEnum::KIH, CardEnum::QUC, CardEnum::FOS, CardEnum::EID };
	Pool pool(aHighishBoard);

	CardEnum initCardEnums[2][2] = {{ CardEnum::KIS, CardEnum::TWH }, { CardEnum::TWH, CardEnum::THH }};
	Hand hand[2] = { initCardEnums[0], initCardEnums[1] };

	testBestHand(pool, hand,
    		"highish board. Player 0 has king pair, player 1 has nothing", { { 0, -1 } });

	hand[1].setHand(CardEnum::TWH, CardEnum::QUS);
	testBestHand(pool, hand,
    		"highish board. Player 0 has king pair, player 1 has queen pair", { { 0, -1 } });

	hand[1].setHand(CardEnum::KIC, CardEnum::JAD);
	testBestHand(pool, hand,
    		"highish board. Both have kings, player 1 has jack kicker", { { 1, -1 } });

	hand[0].setHand(CardEnum::KIS, CardEnum::JAC);
	testBestHand(pool, hand,
    		"highish board. Both have kings, and same kicker", { { 0, 1 } });


	hand[0].setHand(CardEnum::ACC, CardEnum::ACS);
	hand[1].setHand(CardEnum::KIC, CardEnum::JAD);
	testBestHand(pool, hand,
    		"highis board. player 1 has kings. player 0 has pocket aces", { { 0, -1 } });

	CardEnum pairOnBoard[5] = { CardEnum::KIS, CardEnum::KIH, CardEnum::QUC, CardEnum::THS, CardEnum::EID };
	pool.setCards(pairOnBoard);

	hand[0].setHand(CardEnum::SEC, CardEnum::SIC);
	hand[1].setHand(CardEnum::JAS, CardEnum::SES);
	testBestHand(pool, hand,
    		"pair on board. player one has jack kicker", { { 1, -1 } });

	hand[0].setHand(CardEnum::JAC, CardEnum::TEC);
	hand[1].setHand(CardEnum::JAS, CardEnum::SES);
	testBestHand(pool, hand,
    		"pair on board. both have jack kicker but player 0 also has a 10", { { 0, -1 } });

	CardEnum lowPair[5] = { CardEnum::TWC, CardEnum::EID, CardEnum::JAH, CardEnum::KIS, CardEnum::ACC };
	pool.setCards(lowPair);
	
	hand[0].setHand(CardEnum::TWD, CardEnum::FID);
	hand[1].setHand(CardEnum::TWH, CardEnum::FOH);
	testBestHand(pool, hand,
    		"low pair on board. Testing for a tie!", { { 0, 1 } });
}

void testHighCards() 
{
	std::cout << "HIGH CARD TEST" << std::endl;
	CardEnum aHighishBoard[5] = { CardEnum::SEH, CardEnum::KIH, CardEnum::QUC, CardEnum::ACS, CardEnum::EID };
	Pool pool(aHighishBoard);

	CardEnum initCardEnums[2][2] = { {CardEnum::TWH, CardEnum::THH}, {CardEnum::FOH, CardEnum::SIH} };
	Hand hand[2] = { initCardEnums[0], initCardEnums[1] };
	testBestHand(pool, hand,
    		"highish board. Neither have a higher kicker", { { 0, 1 } });

	hand[0].setHand(CardEnum::TED, CardEnum::TWH);
	testBestHand(pool, hand,
		"highish board. Player 0 has one higher kicker", { { 0, -1 } });

	hand[1].setHand(CardEnum::JAD, CardEnum::SIS);
	testBestHand(pool, hand,
		"highish board. Player 0 has one higher kicker but player 1 has an even higher kicker", { { 1, -1 } });

	CardEnum lowerBoard[5] = { CardEnum::SEH, CardEnum::TWH, CardEnum::THC, CardEnum::JAS, CardEnum::EIS };
	pool.setCards(lowerBoard);

	hand[0].setHand(CardEnum::KIS, CardEnum::QUS);
	hand[1].setHand(CardEnum::ACS, CardEnum::KIS);
	testBestHand(pool, hand,
		"lowish board. both have over kickers but player 1 has a higher one", { { 1, -1 } });

	hand[0].setHand(CardEnum::QUS, CardEnum::TED);
	hand[1].setHand(CardEnum::QUD, CardEnum::NID);
	testBestHand(pool, hand,
		"lowish board. both have same highest kicker but player 0 has a higher second kicker", { { 0, -1 } });

	hand[0].setHand(CardEnum::QUS, CardEnum::TED);
	hand[1].setHand(CardEnum::QUD, CardEnum::TES);
	testBestHand(pool, hand,
		"lowish board. both have same kickers", { { 0, 1 } });

	std::cout << "\n\n" << std::endl;
}

void testCalcPreFlopEquity(Hand* hands, unsigned char numHands, Deck& deck){
	std::vector<float> equity = calcPreFlopEquity(hands, numHands, deck);
	std::cout << "All in preflop results in: " << std::endl;
		
	for (unsigned int i = 0; i < (equity.size() / 2); i++)
	{
		std::cout << getCardsString(hands[i].cards[0]) << " + "
			<< getCardsString(hands[i].cards[1]) << ". Equity: "
			<< equity[i] * 100 << "%" << ". Tie likelihood: "
			<< equity[i+(equity.size() / 2)] * 100 << "%" << std::endl;
	}
}

void testTieCase() 
{
	Deck deck(5);
	const unsigned int numTrials = 20;
	const unsigned int numHands = 2;
	Hand hands[numHands];
	Pool pool(deck);

	hands[0].setHand(CardEnum::TWH, CardEnum::THH);
	hands[1].setHand(CardEnum::TWD, CardEnum::FOD);

	TwoDimArray bestHands(1, 2);

	for (int i = 0; i < numTrials; i++) {
		getBestHands(pool, hands, bestHands);
		printPool(pool);
		if (bestHands.get(0, 1) == -1) {
			std::cout << "Player " << (int)bestHands.get(0, 0) << " took the W\n" << std::endl;
		}
		else {
			std::cout << "TIE\n" << std::endl;
		}

		pool.newPool(deck);
	}
}

void testBestHandAndEquity(const unsigned int numHands, const unsigned int numTrials)
{
	std::cout << numHands << " Player RANDOM HANDS and POOLS, BEST HAND, and EQUITY TESTS" << std::endl;

	Hand* hands = new Hand[numHands];
	Deck deck;
	for (unsigned int i = 0; i < numTrials; i++)
	{
		for (unsigned int k = 0; k < numHands; k++) {
			hands[k].setHand(deck.getNextHandCard(), deck.getNextHandCard());
		}

		testCalcPreFlopEquity(hands, numHands, deck);
		deck.resetEntireDeck();
	}
	delete[] hands;
}

void testMultiWayPot() 
{
	CardEnum testPool[5] = { CardEnum::TWC, CardEnum::THC,CardEnum::THD, CardEnum::FOH, CardEnum::KIS };
	Pool pool(testPool);
	Hand hands[4];

	hands[0].setHand(CardEnum::KIH, CardEnum::ACS); //pair of kings
	hands[1].setHand(CardEnum::THH, CardEnum::TWD); //full house
	hands[2].setHand(CardEnum::FIC, CardEnum::SID); //straight
	hands[3].setHand(CardEnum::TWS, CardEnum::FOC); //two pair
	testBestHand(pool, hands, 
				  "Test pool. Player 1 has the best hand (full house)", { {1, -1, -1, -1} });

	hands[3].setHand(CardEnum::THS, CardEnum::TWS);
	testBestHand(pool, hands, 
				  "Test pool. Players 1 and 3 have the same best hand (full house)", { {1, 3, -1, -1} });

	CardEnum strFlushOnBoard[5] = { CardEnum::NIH, CardEnum::KIH, CardEnum::QUH, CardEnum::JAH, CardEnum::TEH };
	pool.setCards(strFlushOnBoard);

	hands[0].setHand(CardEnum::SEH, CardEnum::SEC);
	testBestHand(pool, hands, "strFlushOnBoard. allPlayers tie", { {0, 1, 2, 3} });

	CardEnum acePair[5] = { CardEnum::ACS, CardEnum::ACC, CardEnum::TWD, CardEnum::THC, CardEnum::FIS};
	pool.setCards(acePair);

	hands[0].setHand(CardEnum::QUH, CardEnum::SEC);
	hands[1].setHand(CardEnum::QUD, CardEnum::EIH);
	hands[2].setHand(CardEnum::QUC, CardEnum::NID);
	hands[3].setHand(CardEnum::QUS, CardEnum::TED);
	testBestHand(pool, hands, "Ace pair. player 3 has the best second kicker", { {3, -1, -1, -1} });

	hands[2].setHand(CardEnum::QUC, CardEnum::TES);
	testBestHand(pool, hands, "Ace pair. players 2 and 3 have the best second kicker", { {2, 3, -1, -1} });

	hands[0].setHand(CardEnum::KIH, CardEnum::SEC);
	testBestHand(pool, hands, "Ace pair. player 0 has the best kicker", { {0, -1, -1, -1} });

	CardEnum twoPair[5] = { CardEnum::ACS, CardEnum::ACC, CardEnum::TED, CardEnum::JAS, CardEnum::JAC};
	pool.setCards(twoPair);

	hands[0].setHand(CardEnum::TWC, CardEnum::TEC);
	hands[1].setHand(CardEnum::TWD, CardEnum::EID);
	hands[2].setHand(CardEnum::TWS, CardEnum::SES);
	hands[3].setHand(CardEnum::SIC, CardEnum::SIH);
	testBestHand(pool, hands, "Two pair. AllPlayers tie", { {0, 1, 2, 3} });

	CardEnum randomPool[5] = {CardEnum::FOH, CardEnum::SIC, CardEnum::SES, CardEnum::TED, CardEnum::QUC};
	pool.setCards(randomPool);

	hands[0].setHand(CardEnum::FID, CardEnum::FIS);
	hands[1].setHand(CardEnum::SIS, CardEnum::TEC);
	hands[2].setHand(CardEnum::TWC, CardEnum::FOS);
	hands[3].setHand(CardEnum::FOD, CardEnum::QUS);
	testBestHand(pool, hands, "problematic two pair", { {3, -1, -1, -1} });
}

void testSidePots()
{
	CardEnum testPool[5] = { CardEnum::TWC, CardEnum::THC, CardEnum::SED, CardEnum::FOH, CardEnum::KIS };
	Pool pool(testPool);
	Hand hands[6];
	hands[0].setHand(CardEnum::TWH, CardEnum::ACS); //pair of twos
	hands[1].setHand(CardEnum::THH, CardEnum::THD); //theee threes
	hands[2].setHand(CardEnum::FOC, CardEnum::ACC); //pair of fours
	hands[3].setHand(CardEnum::FOS, CardEnum::ACD); //pair of fours (identical)
	hands[4].setHand(CardEnum::TWD, CardEnum::FOD); //two pair: twos and fours
	hands[5].setHand(CardEnum::QUC, CardEnum::JAC); //high card

	testBestHand(pool, hands, "one pot, everyone but player 5", { {1, -1, -1, -1, -1, -1} }, { { {0, 1, 2, 3, 4} } });
	testBestHand(pool, hands, "two pots, three of a kind in both", { {1,-1,-1,-1,-1,-1},
																	 {1,-1,-1,-1,-1,-1} },
																   { {{0, 1, 2, 3},
																	 {0, 1, 4, 5}} });

	testBestHand(pool, hands, "bunch of different sub pots. Kind of rando", { {1,-1,-1,-1,-1,-1},
																	 {1,-1,-1,-1,-1,-1},
																	 {4,-1,-1,-1,-1,-1},
																	 {2, 3,-1,-1,-1,-1} },
																	 { { {0, 1, 2, 3},
																	 { 0, 1, 4, 5 },
																	 { 4, 0, 3} ,
																	 { 2, 3, 5}} });
}

void allBestHandTests() 
{
	testStraightFlushBestHand();
	testQuads();
	testHouse();
	testFlushBestHand();
	testStraightBestHand();
	testTriple();
	testTwoPairs();
	testPairs();
	testHighCards();
}


void runBHUnitTests()
{
	//allBestHandTests();
	//testTieCase();

	//testMultiWayPot();
	//testSidePots();

	testBestHandAndEquity(2, 3);
	//testBestHandAndEquity(4, 3);
}
