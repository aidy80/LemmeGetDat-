#include "UnitTests.h"

/* Cards.cpp */

void testCardEnum()
{
	std::cout << "ENUM TEST" << std::endl;

	std::cout << "My hand is " << (int)(char)Card::TWD << " and " << (int)(char)Card::KIS << std::endl;
	std::cout << "Expected: 0 and 50" << std::endl;

	std::cout << "My hand is " << (int)(char)Card::THC << " and " << (int)(char)Card::TWH << std::endl;
	std::cout << "Expected: 27 and 13\n\n" << std::endl;
}

void testPrintCard()
{
	std::cout << "PRINT CARD TEST" << std::endl;
	std::cout << "Expected: Ace of Clubs. Actual: " << getCardsString(Card::ACC) << std::endl;
	std::cout << "Expected: Three of Hearts. Actual: " << getCardsString(Card::THH) << std::endl;
	std::cout << "Expected: Six of Diamonds. Actual: " << getCardsString(Card::SID) << std::endl;
	std::cout << "Expected: Jack of Spades. Actual: " << getCardsString(Card::JAS) << "\n\n" << std::endl;
}

void testCard()
{
	testCardEnum();
	testPrintCard();
}

/* PokerSim.cpp */
void testRandomCard()
{
	std::cout << "RANDOM CARD TEST" << std::endl; 
	for (int i = 0; i < 5; i++)
	{
		std::cout << "This is a random card: " << getCardsString(randomCard()) << std::endl;
	}
	std::cout << std::endl;
}

void testRandomCardDist()
{
	const unsigned int numTrails = (unsigned int)pow(10, 9);
	const unsigned int numCards = 52;

	std::vector<unsigned int> cardDist(52, 0);

	for (unsigned int i = 0; i < numTrails; i++)
	{
		cardDist[(unsigned char)randomCard()]++;
	}

	std::vector<float> cardPercentDist(52, 0);

	for (unsigned int i = 0; i < numCards; i++) {
		cardPercentDist[i] = (float)cardDist[i] / (float)numTrails;
		std::cout << cardPercentDist[i] << std::endl;
	}

	float mean = (float)numTrails/(float)numCards;
	float variance = 0;
	unsigned int totCheck = 0;
	for (unsigned int i = 0; i < numCards; i++)
	{
		totCheck += cardDist[i];
		variance += pow(cardDist[i] - mean, 2);
	}

	float stdev = sqrt(variance / (float)(numCards - 1));

	std::cout << "totCheck: " << totCheck << ". numTrials: " << numTrails << "\n";
	std::cout << "Mean: " << mean << ". Standard Deviation: " << stdev << "\n";
	std::cout << "Percentage Mean: " << mean / (float)numTrails
		<< ". Percentage Stdev: " << stdev / (float)numTrails;
}

void testCreateRandomHand(Hand& hand)
{
	createRandomHand(hand);
	std::cout << "This is a random hand: " << getCardsString(hand.cards[0]) 
			  << " + " << getCardsString(hand.cards[1]) << std::endl;
}

void testCreateRandomPoolCards(Pool& pool)
{
	createRandomPoolCards(pool);
	std::cout << "This is the random "; 
	printPool(pool);
}

void testPassedBestHand(Pool& pool, Hand* hands, unsigned char numHands)
{
	std::cout << "Player " << (int)getBestHands(pool, hands, numHands)[0] << " had the best hand" << std::endl;
}

void testBestHand(Pool& pool, Hand *hands, std::string description, std::vector<std::vector<char>> expected) 
{
	char* bH = getBestHands(pool, hands, (unsigned char)expected[0].size());
	std::cout << description << ". Expected: |";
	for (unsigned int i = 0; i < expected.size(); i++) {
		std::cout << " ";
		for (unsigned int j = 0; j < expected[i].size(); j++) {
			std::cout << (int)expected[i][j] << " ";
		}
		std::cout << "|";
	}
	std::cout << ", Actual: |";

	bool failed = false;;
	for (unsigned int i = 0; i < expected.size(); i++) {
		std::cout << " ";
		for (unsigned int j = 0; j < expected[i].size(); j++) {
			std::cout << (int)bH[j] << " ";
			if (bH[j] != expected[i][j]) {
				failed = true;
			}
		}
		std::cout << "| ";
	}

	delete []bH;

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
	Pool strFlushOnBoard = { Card::NIH, Card::KIH, Card::QUH, Card::JAH, Card::TEH };

	Hand hand[2] = { {Card::TWH, Card::THH}, {Card::FOH, Card::EIH} };
	testBestHand(strFlushOnBoard, hand,
		"straight flush on board. Neither have a higher kicker", { { 0, 1 } });

	hand[1] = {Card::ACH, Card::EIH};
	testBestHand(strFlushOnBoard, hand,
		"straight flush on board. Player 1 has the high kicker", {{ 1, -1 } });

	Pool fourGutStrFlush = { Card::NIH, Card::EIH, Card::SIH, Card::FIH, Card::SEC };

	hand[0] = {Card::SEH, Card::THH};
	hand[1] = {Card::TEH, Card::SED};
	testBestHand( fourGutStrFlush, hand,
		"4 card gutshot straight flush on board. Player 0 has the gutshot", { { 0, -1 } });

	Pool thrGutStrFlush = { Card::NIS, Card::EIH, Card::SIH, Card::FIH, Card::SEC };

	hand[0] = {Card::SEH, Card::FOH};
	hand[1] = {Card::TEH, Card::SED};
	testBestHand( thrGutStrFlush, hand,
		"3 card gutshot straight flush on board. Player 0 has the gutshot and low card", { { 0, -1 } });

	Pool thrGutStrFlush2 = { Card::NIS, Card::EIH, Card::SIH, Card::FIH, Card::FIC };

	hand[0] = {Card::SEH, Card::NIH};
	hand[1] = {Card::FIS, Card::FID};
	testBestHand( thrGutStrFlush2, hand,
		"3 card gutshot straight flush on board 2.0. Player 0 has the gutshot and high card", { { 0, -1 } });

	Pool openStrFlush = { Card::NIS, Card::SEH, Card::SIH, Card::FIH, Card::FIC };

	hand[0] = {Card::TES, Card::SED};
	hand[1] = {Card::EIH, Card::FOH};
	testBestHand( openStrFlush, hand,
		"3 card open ended straight flush on board. Player 1 has the low and high kicker", { { 1, -1 } });

	hand[0] = { Card::FIS, Card::FID };
	testBestHand( openStrFlush, hand,
		"3 card open ended straight flush on board. Player 0 has quads, player 1 has strflush", { { 1, -1 } });

	hand[0] = {Card::EIH, Card::NIH};
	hand[1] = {Card::THH, Card::FOH};
	testBestHand( openStrFlush, hand,
		"3 card open ended straight flush on board. Player 1 has the low suited kickers, player 0 has high unsuited kickers", { { 0, -1 } });

	hand[0] = {Card::THH, Card::FOH};
	hand[1] = {Card::FIS, Card::FIC};
	testBestHand( openStrFlush, hand,
		"3 card open ended straight flush on board. Player 1 has the low suited kickers, player 0 has high unsuited kickers", { { 0, -1 } });

	std::cout << "\n\n" << std::endl;
}

void testQuads() 
{
	std::cout << "QUADS TEST" << std::endl;
	Pool tripleOnBoard = { Card::SEH, Card::SES, Card::SEC, Card::TED, Card::EID };
	Hand hand[2];

	hand[0] = { Card::SED, Card::EIC };
	hand[1] = { Card::TWH, Card::TEC };
	testBestHand( tripleOnBoard, hand,
		"pair on board. player 0 has quads, player 1 has a house", { { 0, -1 } });

	Pool quadsOnBoard = { Card::SEH, Card::SES, Card::SEC, Card::SED, Card::EID };

	hand[0] = { Card::TWC, Card::THC };
	hand[1] = { Card::TWD, Card::THD };
	testBestHand( quadsOnBoard, hand,
		"quads on board. Neither player has a higher kicker", { { 0, 1 } });

	hand[0] = { Card::ACS, Card::THC };
	hand[1] = { Card::KIS, Card::THD };
	testBestHand( quadsOnBoard, hand,
			"quads on board. player 0 has a higher kicker", { { 0, -1 } });

	hand[0] = { Card::KIC, Card::KIC };
	hand[1] = { Card::KIS, Card::THD };
	testBestHand( quadsOnBoard, hand,
		"quads on board. both have same top kicker, player 0 has a higher kicker2", { { 0, 1 } });

	hand[0] = { Card::EIS, Card::TWH };
	hand[1] = { Card::EIC, Card::EIH };
	testBestHand( quadsOnBoard, hand,
		"quads on board. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	Pool quadsOnBoard2 = { Card::SEH, Card::SES, Card::SEC, Card::SED, Card::SID };

	hand[0] = { Card::SIS, Card::TWH };
	hand[1] = { Card::SIC, Card::SIH };
	testBestHand( quadsOnBoard2, hand,
		"quads on board 2.0. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	hand[0] = { Card::FIH, Card::TWH };
	hand[1] = { Card::FIC, Card::FID };
	testBestHand( quadsOnBoard2, hand,
		"quads on board 2.0. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	Pool twoPairOnBoard = { Card::SEH, Card::KIH, Card::SES, Card::EIC, Card::EID };

	hand[0] = { Card::SED, Card::SEC };
	hand[1] = { Card::EIH, Card::EIS };
	testBestHand( twoPairOnBoard, hand,
		"Two pair on board. player 1 has a higher quads", { { 1, -1 } });
}

void testHouse()
{
	std::cout << "HOUSE TEST" << std::endl;
	Pool tripleOnBoard = { Card::SEH, Card::SES, Card::SEC, Card::TED, Card::EID };

	Hand hand[2] = { {Card::EIC, Card::SIS},{Card::TWH, Card::THC} };
	testBestHand( tripleOnBoard, hand,
		"triple on Board. player 0 has the house player 1 does not have it", { { 0, -1 } });

	hand[1] = { Card::TEH, Card::TWH };
	testBestHand( tripleOnBoard, hand,
		"triple on Board. player 1 has the higher pair", { { 1, -1 } });

	hand[0] = { Card::TES, Card::TEC };
	testBestHand( tripleOnBoard, hand,
		"triple on Board. player 0 has a higher three of a kind", { { 0, -1 } });

	hand[0] = { Card::QUD, Card::QUC };
	hand[1] = { Card::EIC, Card::TEC };
	testBestHand(tripleOnBoard, hand,
		"triple on board. player 0 has pocket pair resulting in higher house", { { 0, -1 } });

	Pool pairOnBoard = { Card::SEH, Card::SES, Card::SIS, Card::TED, Card::EID };

	hand[0] = { Card::TWH, Card::THC };
	hand[1] = { Card::SEC, Card::TEC };
	testBestHand( pairOnBoard, hand,
		"pair on board. player 1 has the house, player 0 does not", { { 1, -1 } });

	Pool twoPairOnBoard = { Card::SEH, Card::KIH, Card::SES, Card::EIC, Card::EID };

	hand[0] = { Card::SEC, Card::TWH };
	hand[1] = { Card::EIH, Card::TWC };
	testBestHand( twoPairOnBoard, hand,
		"two pair on board. player 1 has higher three of a kind than player 0", { { 1, -1 } });

	hand[0] = { Card::EIS, Card::ACS };
	testBestHand( twoPairOnBoard, hand,
		"two pair on board. both players have same house, player 0 has higher kicker", { { 0, 1 } });

	Pool houseVFlush = { Card::SEH, Card::SES, Card::TEH, Card::TED, Card::EIH };
	hand[0] = { Card::SEC, Card::TWH };
	hand[1] = { Card::ACH, Card::KIH };
	testBestHand( houseVFlush, hand,
		"two pair on board. player 0 has the house, player 1 has a flush", { { 0, -1 } });

	Pool houseOnBoard = { Card::SEH, Card::SES, Card::SEC, Card::TED, Card::TEH };

	hand[0] = { Card::ACS, Card::KIC };
	hand[1] = { Card::QUC, Card::JAC };
	testBestHand( houseOnBoard, hand,
		"house on board. Both players have higher kickers", { { 0, 1 } });

	hand[0] = { Card::JAD, Card::JAH };
	testBestHand( houseOnBoard, hand,
		"house on board. player 0 has a higher pocket pair", { { 0, -1 } });

	hand[1] = { Card::TWH, Card::TEC };
	testBestHand( houseOnBoard, hand,
		"house on board. player 1 has a higher triple", { { 1, -1 } });
}



void testFlushBestHand()
{
	std::cout << "FLUSH BEST HAND TEST\n";

	Pool clubFlushPool = { Card::ACC, Card::JAC, Card::SEC, Card::KIC, Card::EIC };

	Hand hand[2] = { { Card::QUC, Card::EID }, { Card::NIS, Card::ACS } };
	testBestHand(clubFlushPool, hand,
		"Club Flush on pool, queen kicker for player 0. No kicker for player 1", {{ 0, -1 }});

	hand[0] = { Card::NIS, Card::ACS };
	hand[1] = { Card::QUC, Card::EID };
	testBestHand(clubFlushPool, hand,
		"Club Flush on pool, queen kicker for player 1. No kicker for player 0", {{ 1, -1 }});

	hand[0] = { Card::NIC, Card::ACS };
	hand[1] = { Card::QUC, Card::EID };
	testBestHand(clubFlushPool, hand,
		"Club Flush on pool, queen kicker for player 1, nine kicker for player 0", {{ 1, -1 } });

	hand[0] = { Card::TWC, Card::KIC };
	hand[1] = { Card::QUC, Card::NIC };
	testBestHand(clubFlushPool, hand,
		"Club Flush on pool, queen and nine kickers for player 1, eight and king kickers for player 0", { { 0, -1 } });

	hand[0] = { Card::THC, Card::TWC };
	hand[1] = { Card::FIC, Card::FOC };
	testBestHand(clubFlushPool, hand,
		"Club Flush on pool, both players have two clubs but smaller than those on board", { { 0, 1 } });

	hand[0] = { Card::TWS, Card::THS };
	hand[1] = { Card::THH, Card::TWH };
	testBestHand(clubFlushPool, hand,
		"Club Flush on pool, nobody has clubs", {{ 0, 1 }});

	Pool threeHeartPool = { Card::ACH, Card::JAH, Card::THH, Card::KIC, Card::TEC };

	hand[0] = { Card::QUC, Card::EID };
	hand[1] = { Card::NIH, Card::JAC };
	testBestHand(threeHeartPool, hand,
		"Three hearts on pool, Nobody has two hearts", {{ 0, -1 } });

	hand[0] = { Card::QUC, Card::EID };
	hand[1] = { Card::NIH, Card::QUH };
	testBestHand(threeHeartPool, hand,
		"Three hearts on pool, player 1 has two hearts, player 0 had none", {{ 1, -1 } });

	hand[0] = { Card::QUC, Card::EIH };
	hand[1] = { Card::NIH, Card::QUH };
	testBestHand( threeHeartPool, hand,
		"Three hearts on pool, player 1 has two hearts, player 0 had one", { {1, -1} });

	Pool thrGutStrFlush = { Card::NIS, Card::EIH, Card::SIH, Card::FIH, Card::SEC };

	hand[0] = { Card::SEC, Card::TED };
	hand[1] = { Card::THH, Card::TWH };
	testBestHand( threeHeartPool, hand,
		"Three hearts on pool, player 1 has flush, player 0 has straight", { {1, -1} });

	std::cout << "\n\n" << std::endl;
}

void testStraightBestHand()
{
	std::cout << "TESTING STRAIGHT BEST HAND\n";

	Pool straightOnBoard = { Card::FOC, Card::FIC, Card::SID, Card::SED, Card::EIS };

	Hand hand[2] = { {Card::TWS, Card::TWH}, {Card::THC, Card::THH} };
	testBestHand(straightOnBoard, hand,
		"straight on board. no player has a valid kicker, but player 1 does have an under connecting card", { { 0, 1 } });

	hand[0] = { Card::TWH, Card::NIC };
	hand[1] = { Card::THC, Card::TEC };
	testBestHand(straightOnBoard, hand,
		"straight on board. no player 0 has a high kicker, player 1 does not", {{ 0, -1 }});

	hand[1] = { Card::NID, Card::TEH };
	testBestHand(straightOnBoard, hand,
		"straight on board. no player 0 has a high kicker, player 1 has two high kickers", { { 1,-1 } });

	Pool gutShotStraight = { Card::EIS, Card::FOC, Card::FIC, Card::SID, Card::ACS };

	hand[0] = { Card::SEC, Card::JAH };
	hand[1] = { Card::TWC, Card::EID };
	testBestHand(gutShotStraight, hand,
		"gut shot straight. no player 0 has a gutshot, player 1 does not", { { 0, -1 } });

	hand[1] = { Card::SEH, Card::NIH };
	testBestHand(gutShotStraight, hand,
		"gut shot straight. no player 0 has a gutshot, player 1 has gutshot and high kicker", { { 1, -1 } });

	Pool openEndedStraight = { Card::FOC, Card::SID, Card::TED, Card::FIC, Card::ACS };

	hand[0] = { Card::TEC, Card::TWH };
	hand[1] = { Card::THC, Card::KIC };
	testBestHand(openEndedStraight, hand,
		"3-card open ended straight. neither player has it", { { 0, -1 } });

	hand[0] = { Card::THD, Card::JAD };
	hand[1] = { Card::THC, Card::SEH };
	testBestHand(openEndedStraight, hand,
		"3-card open ended straight. player 1 has it with one under, one over. player 0 does not", { { 1, -1 } });

	hand[0] = { Card::SES, Card::EIS };
	testBestHand(openEndedStraight, hand,
		"3-card open ended straight. player 1 has it with one under, one over. player 0 has it with two over", { { 0, -1 } });

	hand[1] = { Card::SED, Card::EID };
	testBestHand( openEndedStraight, hand,
		"3-card open ended straight. player 0 and 1 have same cards (both straights)", { { 0, 1 } });

	hand[0] = { Card::TWH, Card::THD };
	hand[1] = { Card::ACH, Card::ACC };
	testBestHand( openEndedStraight, hand,
		"3-card open ended straight. player 0 has the striaight, player 1 has a triple", { { 0, -1 } });

	std::cout << "\n\n" << std::endl;
}

void testTriple() 
{
	std::cout << "TRIPLE TEST" << std::endl;
	Pool onlySingles = { Card::SEH, Card::KIH, Card::QUC, Card::TED, Card::EID };
	Hand hand[2] = { {Card::SEC, Card::SED}, {Card::TWC, Card::THC} };
	testBestHand( onlySingles, hand,
		"onlySingles. player 0 has pocket pair creating a triple", { { 0, -1 } });

	hand[1] = { Card::TEC, Card::TEH };
	testBestHand( onlySingles, hand,
		"onlySingles. player 0 has pocket pair creating a triple, but player 1 has a higher one", { { 1, -1 } });

	Pool pairOnBoard = { Card::SEH, Card::SES, Card::QUC, Card::TED, Card::EID };

	hand[0] = { Card::SEC, Card::TWH };
	hand[1] = { Card::SIS, Card::TWC };
	testBestHand( pairOnBoard, hand,
		"pairOnBoard. player 0 has card creating triple, but player 1 does not", { { 0, -1 } });

	hand[1] = { Card::SED, Card::THC };
	testBestHand( pairOnBoard, hand,
		"pairOnBoard. both have cards creating triples. player 1 has a higher kicker but both lower than those on board", { { 0, 1 } });

	hand[0] = { Card::SEC, Card::TWH };
	hand[1] = { Card::SED, Card::JAS };
	testBestHand( pairOnBoard, hand,
		"pairOnBoard. both have cards creating triples, but player 1 has a higher valid kicker", { { 1, -1 } });

	hand[0] = { Card::SEC, Card::JAC };
	testBestHand( pairOnBoard, hand,
		"pairOnBoard. players have same hand", { { 0, 1 } });

	hand[1] = { Card::QUD, Card::TWC };
	testBestHand( pairOnBoard, hand,
		"pairOnBoard. player 0 has a triple, player 1 has a two pair", { { 0, -1 } });

	Pool noPairOnBoard = { Card::SEH, Card::FOD, Card::QUC, Card::TED, Card::EID };

	hand[0] = { Card::TEH, Card::TEC };
	hand[1] = { Card::SEC, Card::SED };
	testBestHand( noPairOnBoard, hand,
		"noPairOnBoard. player 0 has a higher pocket pair resulting in higher triple", { { 0, -1 } });

	Pool tripleOnBoard = { Card::SEH, Card::SES, Card::SEC, Card::TED, Card::EID };

	hand[0] = { Card::QUD, Card::JAS };
	hand[1] = { Card::QUC, Card::JAC };
	testBestHand( tripleOnBoard, hand,
		"tripleOnBoard. players have same hand", { { 0, 1 } });

	hand[0] = { Card::ACC, Card::JAS };
	testBestHand( tripleOnBoard, hand,
		"tripleOnBoard. player 0 has a higher kicker", { { 0, -1 } });

	hand[0] = { Card::QUD, Card::JAS };
	hand[1] = { Card::QUC, Card::NID };
	testBestHand( tripleOnBoard, hand,
		"tripleOnBoard. players 0 and 1 have same higher kicker but player 0 has higher kicker2", { { 0, -1 } });

	}

void testTwoPairs()
{
	std::cout << "TWO PAIR TEST" << std::endl;
	Pool noPairOnBoard = { Card::SEH, Card::KIH, Card::QUC, Card::ACS, Card::EID };

	Hand hand[2] = {{ Card::KIS, Card::TWH }, { Card::QUH, Card::EIC }};
	testBestHand(noPairOnBoard, hand,
    		"no pair on board. Player 0 has one pair, player 1 two pairs", { { 1, -1 } });

	hand[0] = { Card::QUS,Card::EIS };
	testBestHand(noPairOnBoard, hand,
    		"no pair on board. Players have the same hands", { { 0, 1 } });

	hand[0] = { Card::EIS, Card::ACS };
	testBestHand(noPairOnBoard, hand,
    		"no pair on board. Player 0 has a higher two pair than player 1", { { 0, -1 } });

	Pool onePairOnBoard = { Card::SEH, Card::SEC, Card::QUC, Card::ACS, Card::FOD };

	hand[0] = { Card::SIS, Card::TWC };
	hand[1] = { Card::QUD, Card::FOD };
	testBestHand(onePairOnBoard, hand,
    		"one pair on board. Player 1 has a higher second pair", { { 1, -1 } });

	hand[0] = { Card::QUH, Card::KIH };
	testBestHand(onePairOnBoard, hand,
    		"one pair on board. Players have the same second pair but player 0 has better kicker", { { 0, -1 } });

	hand[0] = { Card::QUH, Card::FOC };
	testBestHand(onePairOnBoard, hand,
    		"one pair on board. Players have the same hand", { { 0, 1 } });

	Pool twoPairOnBoard = { Card::SEH, Card::KIH, Card::SEC, Card::EIC, Card::EID };

	hand[0] = { Card::TWH, Card::THH };
	hand[1] = { Card::KIS, Card::TWC };
	testBestHand(twoPairOnBoard, hand,
    		"two pairs on board. Player 1 has a king creating a pair", { { 1, -1 } });

	hand[0] = { Card::KIC, Card::SIC };
	hand[1] = { Card::KIS, Card::TWC };
	testBestHand(twoPairOnBoard, hand,
		"two pairs on board, both players have a king creating a pair."
		"Player 0 has a higher kicker but still lower than 3rd pair", { {0, 1} });

	hand[0] = { Card::ACS, Card::ACC };
	testBestHand(twoPairOnBoard, hand,
    		"two pairs on board. Player 0 has a pocket aces", { { 0, -1 } });

	hand[0] = { Card::ACD, Card::TWC };
	hand[1] = { Card::QUC, Card::THH };
	testBestHand(twoPairOnBoard, hand,
    		"two pairs on board. Player 0 has a better first kicker", { { 0, -1 } });

	hand[0] = { Card::ACD, Card::JAD };
	hand[1] = { Card::ACS, Card::TED };
	testBestHand(twoPairOnBoard, hand,
    		"two pairs on board. players have same first kicker but player 1 has a better second kicker", { { 0, 1 } });
}



void testPairs()
{
	std::cout << "PAIR TEST" << std::endl;
	Pool aHighishBoard = { Card::SEH, Card::KIH, Card::QUC, Card::FOS, Card::EID };

	Hand hand[2] = {{ Card::KIS, Card::TWH }, { Card::TWH, Card::THH }};
	testBestHand(aHighishBoard, hand,
    		"highish board. Player 0 has king pair, player 1 has nothing", { { 0, -1 } });

	hand[1] = { Card::TWH, Card::QUS };
	testBestHand(aHighishBoard, hand,
    		"highish board. Player 0 has king pair, player 1 has queen pair", { { 0, -1 } });

	hand[1] = { Card::KIC, Card::JAD };
	testBestHand(aHighishBoard, hand,
    		"highish board. Both have kings, player 1 has jack kicker", { { 1, -1 } });

	hand[0] = { Card::KIS, Card::JAC };
	testBestHand(aHighishBoard, hand,
    		"highish board. Both have kings, and same kicker", { { 0, 1 } });


	hand[0] = { Card::ACC, Card::ACS };
	hand[1] = { Card::KIC, Card::JAD };
	testBestHand(aHighishBoard, hand,
    		"highis board. player 1 has kings. player 0 has pocket aces", { { 0, -1 } });

	Pool pairOnBoard = { Card::KIS, Card::KIH, Card::QUC, Card::THS, Card::EID };


	hand[0] = { Card::SEC, Card::SIC };
	hand[1] = { Card::JAS, Card:: SES};
	testBestHand(pairOnBoard, hand,
    		"pair on board. player one has jack kicker", { { 1, -1 } });

	hand[0] = { Card::JAC, Card::TEC };
	hand[1] = { Card::JAS, Card::SES};
	testBestHand(pairOnBoard, hand,
    		"pair on board. both have jack kicker but player 0 also has a 10", { { 0, -1 } });
}

void testHighCards() 
{
	std::cout << "HIGH CARD TEST" << std::endl;
	Pool aHighishBoard = { Card::SEH, Card::KIH, Card::QUC, Card::ACS, Card::EID };

	Hand hand[2] = { {Card::TWH, Card::THH}, {Card::FOH, Card::SIH} };
	testBestHand(aHighishBoard, hand,
    		"highish board. Neither have a higher kicker", { { 0, 1 } });

	hand[0] = { Card::TED, Card::TWH };
	testBestHand(aHighishBoard, hand,
		"highish board. Player 0 has one higher kicker", { { 0, -1 } });

	hand[1] = { Card::JAD, Card::SIS };
	testBestHand(aHighishBoard, hand,
		"highish board. Player 0 has one higher kicker but player 1 has an even higher kicker", { { 1, -1 } });

	Pool lowerBoard = { Card::SEH, Card::TWH, Card::THC, Card::JAS, Card::EIS };

	hand[0] = { Card::KIS, Card::QUS };
	hand[1] = { Card::ACS, Card::KIS };
	testBestHand(lowerBoard, hand,
		"lowish board. both have over kickers but player 1 has a higher one", { { 1, -1 } });

	hand[0] = { Card::QUS, Card::TED };
	hand[1] = { Card::QUD, Card::NID };
	testBestHand(lowerBoard, hand,
		"lowish board. both have same highest kicker but player 0 has a higher second kicker", { { 0, -1 } });

	hand[0] = { Card::QUS, Card::TED };
	hand[1] = { Card::QUD, Card::TES };
	testBestHand(lowerBoard, hand,
		"lowish board. both have same kickers", { { 0, 1 } });

	std::cout << "\n\n" << std::endl;
}

void testCalcPreFlopEquity(Hand* hands, unsigned char numHands)
{
	std::vector<float> equity = calcPreFlopEquity(hands, numHands);
	std::cout << "All in preflop results in: " << std::endl;
		
	for (unsigned int i = 0; i < equity.size(); i++)
	{
		std::cout << getCardsString(hands[i].cards[0]) << " + " << getCardsString(hands[i].cards[1]) 
			<< ". Equity: " << equity[i] * 100 << "%" << std::endl;
	}
	std::cout << std::endl;
}

void testBestHandAndEquity(const unsigned int numHands, const unsigned int numTrials)
{
	std::cout << numHands << " Player RANDOM HANDS and POOLS, BEST HAND, and EQUITY TESTS" << std::endl;

	for (unsigned int i = 0; i < numTrials; i++)
	{
		Hand* hands = new Hand[numHands];

		for (unsigned int j = 0; j < numHands; j++)
		{
			testCreateRandomHand(hands[j]);
		}

		Pool newPool;
		testCreateRandomPoolCards(newPool);

		testPassedBestHand(newPool, hands, numHands);
		testCalcPreFlopEquity(hands, numHands);
		delete[] hands;
	}
}

void testPokerSim()
{
	//testRandomCard();
	//testBestHandAndEquity(2, 3);
	//testBestHandAndEquity(6, 3);
	//testRandomCardDist();

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
