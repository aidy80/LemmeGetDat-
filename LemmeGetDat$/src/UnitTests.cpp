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
void testNextCard()
{
	std::cout << "RANDOM CARD TEST" << std::endl; 
	std::unordered_map<Card, bool> seenCards;
	Deck deck(5);
	for (int i = 0; i < NUM_CARDS_IN_DECK; i++)
	{
		deck.printDeck();
		Card newCard = deck.getNextPoolCard();
		std::cout << "This is another random card: " << (int)newCard << " a.k.a. " << getCardsString(newCard) << std::endl;
		assert(!seenCards[newCard]);
		seenCards[newCard] = true;
	}
	std::cout << std::endl;
}

void testRandomCardDist()
{
	const unsigned int numTrails = (unsigned int)pow(10, 7);
	Deck deck(5);

	std::vector<unsigned int> cardDist(NUM_CARDS_IN_DECK, 0);

	for (unsigned int i = 0; i < numTrails; i++)
	{
		Card newCard = deck.getNextPoolCard();
		cardDist[(int)newCard]++;
		deck.resetEntireDeck();
		if (i % 1000000 == 0) {
			std::cout << i << std::endl;
		}
	}

	std::vector<float> cardPercentDist(52, 0);

	for (unsigned int i = 0; i < NUM_CARDS_IN_DECK; i++) {
		cardPercentDist[i] = (float)cardDist[i] / (float)numTrails;
		std::cout << cardPercentDist[i] << " ";
	}
	std::cout << std::endl;

	float mean = (float)numTrails/(float)NUM_CARDS_IN_DECK;
	float variance = 0;
	unsigned int totCheck = 0;
	for (unsigned int i = 0; i < NUM_CARDS_IN_DECK; i++)
	{
		totCheck += cardDist[i];
		variance += pow(cardDist[i] - mean, 2);
	}

	float stdev = sqrt(variance / (float)(NUM_CARDS_IN_DECK - 1));

	std::cout << "totCheck: " << totCheck << ". numTrials: " << numTrails << "\n";
	std::cout << "Mean: " << mean << ". Standard Deviation: " << stdev << "\n";
	std::cout << "Percentage Mean: " << mean / (float)numTrails
		<< ". Percentage Stdev: " << stdev / (float)numTrails << std::endl;
}

void testPassedBestHand(Pool& pool, Hand* hands, unsigned char numHands)
{
	char* bestHands = (char*)alloca(sizeof(char) * numHands);
	getBestHands(pool, hands, numHands, bestHands);
	std::cout << "Player " << (int)bestHands[0] << " had the best hand" << std::endl;
}

void testBestHand(Pool& pool, Hand *hands, std::string description, std::vector<std::vector<char>> expected) 
{
	char* bH = (char*)alloca(sizeof(char) * expected[0].size());
	getBestHands(pool, hands, (unsigned char)expected[0].size(), bH);
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
	Card strFlushOnBoard[5] = { Card::NIH, Card::KIH, Card::QUH, Card::JAH, Card::TEH };
	Pool pool(strFlushOnBoard);

	Card initHands[2][2] = { {Card::TWH, Card::THH}, {Card::FOH, Card::EIH} };
	Hand hand[2] = { initHands[0], initHands[1] };
	testBestHand(pool, hand,
		"straight flush on board. Neither have a higher kicker", { { 0, 1 } });

	hand[1].setHand(Card::ACH, Card::EIH);
	testBestHand(pool, hand,
		"straight flush on board. Player 1 has the high kicker", { { 1, -1 } });

	Card fourGutStrFlush[5] = { Card::NIH, Card::EIH, Card::SIH, Card::FIH, Card::SEC };
	pool.setCards(fourGutStrFlush);

	hand[0].setHand(Card::SEH, Card::THH);
	hand[1].setHand(Card::TEH, Card::SED);
	testBestHand(pool, hand,
		"4 card gutshot straight flush on board. Player 0 has the gutshot", { { 0, -1 } });

	Card thrGutStrFlush[5] = { Card::NIS, Card::EIH, Card::SIH, Card::FIH, Card::SEC };
	pool.setCards(thrGutStrFlush);

	hand[0].setHand(Card::SEH, Card::FOH);
	hand[1].setHand(Card::TEH, Card::SED);
	testBestHand(pool, hand,
		"3 card gutshot straight flush on board. Player 0 has the gutshot and low card", { { 0, -1 } });

	Card thrGutStrFlush2[5] = { Card::NIS, Card::EIH, Card::SIH, Card::FIH, Card::FIC };
	pool.setCards(thrGutStrFlush2);

	hand[0].setHand(Card::SEH, Card::NIH);
	hand[1].setHand(Card::FIS, Card::FID);
	testBestHand( pool, hand,
		"3 card gutshot straight flush on board 2.0. Player 0 has the gutshot and high card", { { 0, -1 } });

	Card openStrFlush[5] = { Card::NIS, Card::SEH, Card::SIH, Card::FIH, Card::FIC };
	pool.setCards(openStrFlush);

	hand[0].setHand(Card::TES, Card::SED);
	hand[1].setHand(Card::EIH, Card::FOH);
	testBestHand( pool, hand,
		"3 card open ended straight flush on board. Player 1 has the low and high kicker", { { 1, -1 } });

	hand[0].setHand(Card::FIS, Card::FID);
	testBestHand( pool, hand,
		"3 card open ended straight flush on board. Player 0 has quads, player 1 has strflush", { { 1, -1 } });

	hand[0].setHand(Card::EIH, Card::NIH);
	hand[1].setHand(Card::THH, Card::FOH);
	testBestHand( pool, hand,
		"3 card open ended straight flush on board. Player 1 has the low suited kickers, player 0 has high unsuited kickers", { { 0, -1 } });

	initHands[0][0] = Card::THH;
	initHands[0][1] = Card::FOH;
	initHands[1][0] = Card::FIS;
	initHands[1][1] = Card::FIC;
	hand[0].setHand(Card::THH, Card::FOH);
	hand[1].setHand(Card::FIS, Card::FIC);
	testBestHand( pool, hand,
		"3 card open ended straight flush on board. Player 1 has the low suited kickers, player 0 has high unsuited kickers", { { 0, -1 } });

	std::cout << "\n\n" << std::endl;
}

void testQuads() 
{
	std::cout << "QUADS TEST" << std::endl;
	Card tripleOnBoard[5] = { Card::SEH, Card::SES, Card::SEC, Card::TED, Card::EID };
	Pool pool(tripleOnBoard);
	Hand hand[2];

	hand[0].setHand(Card::SED, Card::EIC);
	hand[1].setHand(Card::TWH, Card::TEC);
	testBestHand( pool, hand,
		"pair on board. player 0 has quads, player 1 has a house", { { 0, -1 } });

	Card quadsOnBoard[5] = { Card::SEH, Card::SES, Card::SEC, Card::SED, Card::EID };
	pool.setCards(quadsOnBoard);

	hand[0].setHand(Card::TWC, Card::THC);
	hand[1].setHand(Card::TWD, Card::THD);
	testBestHand( pool, hand,
		"quads on board. Neither player has a higher kicker", { { 0, 1 } });

	hand[0].setHand(Card::ACS, Card::THC);
	hand[1].setHand(Card::KIS, Card::THD);
	testBestHand( pool, hand,
			"quads on board. player 0 has a higher kicker", { { 0, -1 } });

	hand[0].setHand(Card::KIC, Card::KIC);
	hand[1].setHand(Card::KIS, Card::THD);
	testBestHand( pool, hand,
		"quads on board. both have same top kicker, player 0 has a higher kicker2", { { 0, 1 } });

	hand[0].setHand(Card::EIS, Card::TWH);
	hand[1].setHand(Card::EIC, Card::EIH);
	testBestHand( pool, hand,
		"quads on board. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	Card quadsOnBoard2[5] = { Card::SEH, Card::SES, Card::SEC, Card::SED, Card::SID };
	pool.setCards(quadsOnBoard2);

	hand[0].setHand(Card::SIS, Card::TWH);
	hand[1].setHand(Card::SIC, Card::SIH);
	testBestHand( pool, hand,
		"quads on board 2.0. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	hand[0].setHand(Card::FIH, Card::TWH);
	hand[1].setHand(Card::FIC, Card::FID);
	testBestHand( pool, hand,
		"quads on board 2.0. both have same top kicker, player 1 has a higher kicker2", { { 0, 1 } });

	Card twoPairOnBoard[5] = { Card::SEH, Card::KIH, Card::SES, Card::EIC, Card::EID };
	pool.setCards(twoPairOnBoard);

	hand[0].setHand(Card::SED, Card::SEC);
	hand[1].setHand(Card::EIH, Card::EIS);
	testBestHand( pool, hand,
		"Two pair on board. player 1 has a higher quads", { { 1, -1 } });
}

void testHouse()
{
	std::cout << "HOUSE TEST" << std::endl;
	Card tripleOnBoard[5] = { Card::SEH, Card::SES, Card::SEC, Card::TED, Card::EID };
	Pool pool(tripleOnBoard);
	Hand hand[2];

	hand[0].setHand(Card::EIC, Card::SIS); 
	hand[1].setHand(Card::TWH, Card::THC);

	testBestHand( pool, hand,
		"triple on Board. player 0 has the house player 1 does not have it", { { 0, -1 } });

	hand[1].setHand(Card::TEH, Card::TWH);
	testBestHand( pool, hand,
		"triple on Board. player 1 has the higher pair", { { 1, -1 } });

	hand[0].setHand(Card::TES, Card::TEC);
	testBestHand( pool, hand,
		"triple on Board. player 0 has a higher three of a kind", { { 0, -1 } });

	hand[0].setHand(Card::QUD, Card::QUC);
	hand[1].setHand(Card::EIC, Card::TEC);
	testBestHand(pool, hand,
		"triple on board. player 0 has pocket pair resulting in higher house", { { 0, -1 } });

	Card pairOnBoard[5] = { Card::SEH, Card::SES, Card::SIS, Card::TED, Card::EID };
	pool.setCards(pairOnBoard);

	hand[0].setHand(Card::TWH, Card::THC);
	hand[1].setHand(Card::SEC, Card::TEC);
	testBestHand( pool, hand,
		"pair on board. player 1 has the house, player 0 does not", { { 1, -1 } });

	Card twoPairOnBoard[5] = { Card::SEH, Card::KIH, Card::SES, Card::EIC, Card::EID };
	pool.setCards(twoPairOnBoard);

	hand[0].setHand(Card::SEC, Card::TWH);
	hand[1].setHand(Card::EIH, Card::TWC);
	testBestHand( pool, hand,
		"two pair on board. player 1 has higher three of a kind than player 0", { { 1, -1 } });

	hand[0].setHand(Card::EIS, Card::ACS);
	testBestHand( pool, hand,
		"two pair on board. both players have same house, player 0 has higher kicker", { { 0, 1 } });

	Card houseVFlush[5] = { Card::SEH, Card::SES, Card::TEH, Card::TED, Card::EIH };
	pool.setCards(houseVFlush);

	hand[0].setHand(Card::SEC, Card::TWH);
	hand[1].setHand(Card::ACH, Card::KIH);
	testBestHand( pool, hand,
		"two pair on board. player 0 has the house, player 1 has a flush", { { 0, -1 } });

	Card houseOnBoard[5] = { Card::SEH, Card::SES, Card::SEC, Card::TED, Card::TEH };
	pool.setCards(houseOnBoard);

	hand[0].setHand(Card::ACS, Card::KIC);
	hand[1].setHand(Card::QUC, Card::JAC);
	testBestHand( pool, hand,
		"house on board. Both players have higher kickers", { { 0, 1 } });

	hand[0].setHand(Card::JAD, Card::JAH);
	testBestHand( pool, hand,
		"house on board. player 0 has a higher pocket pair", { { 0, -1 } });

	hand[1].setHand(Card::TWH, Card::TEC);
	testBestHand( pool, hand,
		"house on board. player 1 has a higher triple", { { 1, -1 } });
}



void testFlushBestHand()
{
	std::cout << "FLUSH BEST HAND TEST\n";

	Card clubFlushPool[5] = { Card::ACC, Card::JAC, Card::SEC, Card::KIC, Card::EIC };
	Pool pool(clubFlushPool);

	Card initList[2][2] = { { Card::QUC, Card::EID }, { Card::NIS, Card::ACS } };
	Hand hand[2] = { initList[0], initList[1] };
	testBestHand(pool, hand,
		"Club Flush on pool, queen kicker for player 0. No kicker for player 1", {{ 0, -1 }});

	hand[0].setHand(Card::NIS, Card::ACS);
	hand[1].setHand(Card::QUC, Card::EID);
	testBestHand(pool, hand,
		"Club Flush on pool, queen kicker for player 1. No kicker for player 0", {{ 1, -1 }});

	hand[0].setHand(Card::NIC, Card::ACS);
	hand[1].setHand(Card::QUC, Card::EID);
	testBestHand(pool, hand,
		"Club Flush on pool, queen kicker for player 1, nine kicker for player 0", {{ 1, -1 } });

	hand[0].setHand(Card::TWC, Card::KIC);
	hand[1].setHand(Card::QUC, Card::NIC);
	testBestHand(pool, hand,
		"Club Flush on pool, queen and nine kickers for player 1, eight and king kickers for player 0", { { 0, -1 } });

	hand[0].setHand(Card::THC, Card::TWC);
	hand[1].setHand(Card::FIC, Card::FOC);
	testBestHand(pool, hand,
		"Club Flush on pool, both players have two clubs but smaller than those on board", { { 0, 1 } });

	hand[0].setHand(Card::TWS, Card::THS);
	hand[1].setHand(Card::THH, Card::TWH);
	testBestHand(pool, hand,
		"Club Flush on pool, nobody has clubs", {{ 0, 1 }});

	Card threeHeartPool[5] = { Card::ACH, Card::JAH, Card::THH, Card::KIC, Card::TEC };
	pool.setCards(threeHeartPool);

	hand[0].setHand(Card::QUC, Card::EID);
	hand[1].setHand(Card::NIH, Card::JAC);
	testBestHand(pool, hand,
		"Three hearts on pool, Nobody has two hearts", {{ 0, -1 } });

	hand[0].setHand(Card::QUC, Card::EID);
	hand[1].setHand(Card::NIH, Card::QUH);
	testBestHand(pool, hand,
		"Three hearts on pool, player 1 has two hearts, player 0 had none", {{ 1, -1 } });

	hand[0].setHand(Card::QUC, Card::EIH);
	hand[1].setHand(Card::NIH, Card::QUH);
	testBestHand( pool, hand,
		"Three hearts on pool, player 1 has two hearts, player 0 had one", { {1, -1} });

	Card thrGutStrFlush[5] = { Card::NIS, Card::EIH, Card::SIH, Card::FIH, Card::SEC };
	pool.setCards(thrGutStrFlush);

	hand[0].setHand(Card::SEC, Card::TED);
	hand[1].setHand(Card::THH, Card::TWH);
	testBestHand(pool, hand,
		"Three hearts on pool, player 1 has flush, player 0 has straight", { {1, -1} });

	std::cout << "\n\n" << std::endl;
}

void testStraightBestHand()
{
	std::cout << "TESTING STRAIGHT BEST HAND\n";

	Card straightOnBoard[5] = { Card::FOC, Card::FIC, Card::SID, Card::SED, Card::EIS };
	Pool pool(straightOnBoard);

	Card initCards[2][2] = { {Card::TWS, Card::TWH}, {Card::THC, Card::THH} };
	Hand hand[2] = { initCards[0], initCards[1] };

	testBestHand(pool, hand,
		"straight on board. no player has a valid kicker, but player 1 does have an under connecting card", { { 0, 1 } });

	hand[0].setHand(Card::TWH, Card::NIC);
	hand[1].setHand(Card::THC, Card::TEC);
	testBestHand(pool, hand,
		"straight on board. no player 0 has a high kicker, player 1 does not", { { 0, -1 } });

	hand[1].setHand(Card::NID, Card::TEH);
	testBestHand(pool, hand,
		"straight on board. no player 0 has a high kicker, player 1 has two high kickers", { { 1,-1 } });

	Card gutShotStraight[5] = { Card::EIS, Card::FOC, Card::FIC, Card::SID, Card::ACS };
	pool.setCards(gutShotStraight);

	hand[0].setHand(Card::SEC, Card::JAH);
	hand[1].setHand(Card::TWC, Card::EID);
	testBestHand(pool, hand,
		"gut shot straight. no player 0 has a gutshot, player 1 does not", { { 0, -1 } });

	hand[1].setHand(Card::SEH, Card::NIH);
	testBestHand(pool, hand,
		"gut shot straight. no player 0 has a gutshot, player 1 has gutshot and high kicker", { { 1, -1 } });

	Card openEndedStraight[5] = { Card::FOC, Card::SID, Card::TED, Card::FIC, Card::ACS };
	pool.setCards(openEndedStraight);

	hand[0].setHand(Card::TEC, Card::TWH);
	hand[1].setHand(Card::THC, Card::KIC);
	testBestHand(pool, hand,
		"3-card open ended straight. neither player has it", { { 0, -1 } });

	hand[0].setHand(Card::THD, Card::JAD);
	hand[1].setHand(Card::THC, Card::SEH);
	testBestHand(pool, hand,
		"3-card open ended straight. player 1 has it with one under, one over. player 0 does not", { { 1, -1 } });

	hand[0].setHand(Card::SES, Card::EIS);
	testBestHand(pool, hand,
		"3-card open ended straight. player 1 has it with one under, one over. player 0 has it with two over", { { 0, -1 } });

	hand[1].setHand(Card::SED, Card::EID);
	testBestHand(pool, hand,
		"3-card open ended straight. player 0 and 1 have same cards (both straights)", { { 0, 1 } });

	hand[0].setHand(Card::TWH, Card::THD);
	hand[1].setHand(Card::ACH, Card::ACC);
	testBestHand( pool, hand,
		"3-card open ended straight. player 0 has the striaight, player 1 has a triple", { { 0, -1 } });

	std::cout << "\n\n" << std::endl;
}

void testTriple() 
{
	std::cout << "TRIPLE TEST" << std::endl;
	Card onlySingles[5] = { Card::SEH, Card::KIH, Card::QUC, Card::TED, Card::EID };
	Pool pool(onlySingles);

	Card initList[2][2] = { {Card::SEC, Card::SED}, {Card::TWC, Card::THC} };
	Hand hand[2] = { initList[0], initList[1] };

	testBestHand( pool, hand,
		"onlySingles. player 0 has pocket pair creating a triple", { { 0, -1 } });

	hand[1].setHand(Card::TEC, Card::TEH);
	testBestHand( pool, hand,
		"onlySingles. player 0 has pocket pair creating a triple, but player 1 has a higher one", { { 1, -1 } });

	Card pairOnBoard[5] = { Card::SEH, Card::SES, Card::QUC, Card::TED, Card::EID };
	pool.setCards(pairOnBoard);

	hand[0].setHand(Card::SEC, Card::TWH);
	hand[1].setHand(Card::SIS, Card::TWC);
	testBestHand(pool, hand,
		"pairOnBoard. player 0 has card creating triple, but player 1 does not", { { 0, -1 } });

	hand[1].setHand(Card::SED, Card::THC);
	testBestHand( pool, hand,
		"pairOnBoard. both have cards creating triples. player 1 has a higher kicker but both lower than those on board", { { 0, 1 } });

	hand[0].setHand(Card::SEC, Card::TWH);
	hand[1].setHand(Card::SED, Card::JAS);
	testBestHand( pool, hand,
		"pairOnBoard. both have cards creating triples, but player 1 has a higher valid kicker", { { 1, -1 } });

	hand[0].setHand(Card::SEC, Card::JAC);
	testBestHand( pool, hand,
		"pairOnBoard. players have same hand", { { 0, 1 } });

	hand[1].setHand(Card::QUD, Card::TWC);
	testBestHand( pool, hand,
		"pairOnBoard. player 0 has a triple, player 1 has a two pair", { { 0, -1 } });

	Card noPairOnBoard[5] = { Card::SEH, Card::FOD, Card::QUC, Card::TED, Card::EID };
	pool.setCards(noPairOnBoard);

	hand[0].setHand(Card::TEH, Card::TEC);
	hand[1].setHand(Card::SEC, Card::SED);
	testBestHand( pool, hand,
		"noPairOnBoard. player 0 has a higher pocket pair resulting in higher triple", { { 0, -1 } });

	Card tripleOnBoard[5] = { Card::SEH, Card::SES, Card::SEC, Card::TED, Card::EID };
	pool.setCards(tripleOnBoard);

	hand[0].setHand(Card::QUD, Card::JAS);
	hand[1].setHand(Card::QUC, Card::JAC);
	testBestHand( pool, hand,
		"tripleOnBoard. players have same hand", { { 0, 1 } });

	hand[0].setHand(Card::ACC, Card::JAS);
	testBestHand( pool, hand,
		"tripleOnBoard. player 0 has a higher kicker", { { 0, -1 } });

	hand[0].setHand(Card::QUD, Card::JAS);
	hand[1].setHand(Card::QUC, Card::NID);
	testBestHand( pool, hand,
		"tripleOnBoard. players 0 and 1 have same higher kicker but player 0 has higher kicker2", { { 0, -1 } });
}

void testTwoPairs()
{
	std::cout << "TWO PAIR TEST" << std::endl;
	Card noPairOnBoard[5] = { Card::SEH, Card::KIH, Card::QUC, Card::ACS, Card::EID };
	Pool pool(noPairOnBoard);

	Card initCards[2][2] = { { Card::KIS, Card::TWH }, { Card::QUH, Card::EIC } };
	Hand hand[2] = {initCards[0], initCards[1]};
	testBestHand(pool, hand,
    		"no pair on board. Player 0 has one pair, player 1 two pairs", { { 1, -1 } });

	hand[0].setHand(Card::QUS, Card::EIS);
	testBestHand(pool, hand,
    		"no pair on board. Players have the same hands", { { 0, 1 } });

	hand[0].setHand(Card::EIS, Card::ACS);
	testBestHand(pool, hand,
    		"no pair on board. Player 0 has a higher two pair than player 1", { { 0, -1 } });

	Card onePairOnBoard[5] = { Card::SEH, Card::SEC, Card::QUC, Card::ACS, Card::FOD };
	pool.setCards(onePairOnBoard);

	hand[0].setHand(Card::SIS, Card::TWC);
	hand[1].setHand(Card::QUD, Card::FOD);
	testBestHand(pool, hand,
    		"one pair on board. Player 1 has a higher second pair", { { 1, -1 } });

	hand[0].setHand(Card::QUH, Card::KIH);
	testBestHand(pool, hand,
    		"one pair on board. Players have the same second pair but player 0 has better kicker", { { 0, 1 } });

	hand[0].setHand(Card::QUH, Card::FOC);
	testBestHand(pool, hand,
    		"one pair on board. Players have the same hand", { { 0, 1 } });

	Card twoPairOnBoard[5] = { Card::SEH, Card::KIH, Card::SEC, Card::EIC, Card::EID };
	pool.setCards(twoPairOnBoard);

	hand[0].setHand(Card::TWH, Card::THH);
	hand[1].setHand(Card::KIS, Card::TWC);
	testBestHand(pool, hand,
    		"two pairs on board. Player 1 has a king creating a pair", { { 1, -1 } });

	hand[0].setHand(Card::KIC, Card::SIC);
	hand[1].setHand(Card::KIS, Card::TWC);
	testBestHand(pool, hand,
		"two pairs on board, both players have a king creating a pair. "
		"Player 0 has a higher kicker but still lower than 3rd pair", { {0, 1} });

	hand[0].setHand(Card::ACS, Card::ACC);
	testBestHand(pool, hand,
    		"two pairs on board. Player 0 has a pocket aces", { { 0, -1 } });

	hand[0].setHand(Card::ACD, Card::TWC);
	hand[1].setHand(Card::QUC, Card::THH);
	testBestHand(pool, hand,
    		"two pairs on board. Player 0 has a better first kicker", { { 0, -1 } });

	hand[0].setHand(Card::ACD, Card::JAD);
	hand[1].setHand(Card::ACS, Card::TED);
	testBestHand(pool, hand,
    		"two pairs on board. players have same first kicker but player 1 has a better second kicker", { { 0, 1 } });
}



void testPairs()
{
	std::cout << "PAIR TEST" << std::endl;
	Card aHighishBoard[5] = { Card::SEH, Card::KIH, Card::QUC, Card::FOS, Card::EID };
	Pool pool(aHighishBoard);

	Card initCards[2][2] = {{ Card::KIS, Card::TWH }, { Card::TWH, Card::THH }};
	Hand hand[2] = { initCards[0], initCards[1] };

	testBestHand(pool, hand,
    		"highish board. Player 0 has king pair, player 1 has nothing", { { 0, -1 } });

	hand[1].setHand(Card::TWH, Card::QUS);
	testBestHand(pool, hand,
    		"highish board. Player 0 has king pair, player 1 has queen pair", { { 0, -1 } });

	hand[1].setHand(Card::KIC, Card::JAD);
	testBestHand(pool, hand,
    		"highish board. Both have kings, player 1 has jack kicker", { { 1, -1 } });

	hand[0].setHand(Card::KIS, Card::JAC);
	testBestHand(pool, hand,
    		"highish board. Both have kings, and same kicker", { { 0, 1 } });


	hand[0].setHand(Card::ACC, Card::ACS);
	hand[1].setHand(Card::KIC, Card::JAD);
	testBestHand(pool, hand,
    		"highis board. player 1 has kings. player 0 has pocket aces", { { 0, -1 } });

	Card pairOnBoard[5] = { Card::KIS, Card::KIH, Card::QUC, Card::THS, Card::EID };
	pool.setCards(pairOnBoard);

	hand[0].setHand(Card::SEC, Card::SIC);
	hand[1].setHand(Card::JAS, Card::SES);
	testBestHand(pool, hand,
    		"pair on board. player one has jack kicker", { { 1, -1 } });

	hand[0].setHand(Card::JAC, Card::TEC);
	hand[1].setHand(Card::JAS, Card::SES);
	testBestHand(pool, hand,
    		"pair on board. both have jack kicker but player 0 also has a 10", { { 0, -1 } });

	Card lowPair[5] = { Card::TWC, Card::EID, Card::JAH, Card::KIS, Card::ACC };
	pool.setCards(lowPair);
	
	hand[0].setHand(Card::TWD, Card::FID);
	hand[1].setHand(Card::TWH, Card::FOH);
	testBestHand(pool, hand,
    		"low pair on board. Testing for a tie!", { { 0, 1 } });
}

void testHighCards() 
{
	std::cout << "HIGH CARD TEST" << std::endl;
	Card aHighishBoard[5] = { Card::SEH, Card::KIH, Card::QUC, Card::ACS, Card::EID };
	Pool pool(aHighishBoard);

	Card initCards[2][2] = { {Card::TWH, Card::THH}, {Card::FOH, Card::SIH} };
	Hand hand[2] = { initCards[0], initCards[1] };
	testBestHand(pool, hand,
    		"highish board. Neither have a higher kicker", { { 0, 1 } });

	hand[0].setHand(Card::TED, Card::TWH);
	testBestHand(pool, hand,
		"highish board. Player 0 has one higher kicker", { { 0, -1 } });

	hand[1].setHand(Card::JAD, Card::SIS);
	testBestHand(pool, hand,
		"highish board. Player 0 has one higher kicker but player 1 has an even higher kicker", { { 1, -1 } });

	Card lowerBoard[5] = { Card::SEH, Card::TWH, Card::THC, Card::JAS, Card::EIS };
	pool.setCards(lowerBoard);

	hand[0].setHand(Card::KIS, Card::QUS);
	hand[1].setHand(Card::ACS, Card::KIS);
	testBestHand(pool, hand,
		"lowish board. both have over kickers but player 1 has a higher one", { { 1, -1 } });

	hand[0].setHand(Card::QUS, Card::TED);
	hand[1].setHand(Card::QUD, Card::NID);
	testBestHand(pool, hand,
		"lowish board. both have same highest kicker but player 0 has a higher second kicker", { { 0, -1 } });

	hand[0].setHand(Card::QUS, Card::TED);
	hand[1].setHand(Card::QUD, Card::TES);
	testBestHand(pool, hand,
		"lowish board. both have same kickers", { { 0, 1 } });

	std::cout << "\n\n" << std::endl;
}

void testCalcPreFlopEquity(Hand* hands, unsigned char numHands, Deck& deck){
	std::vector<float> equity = calcPreFlopEquity(hands, numHands, deck);
	std::cout << "All in preflop results in: " << std::endl;
		
	for (unsigned int i = 0; i < equity.size() - 1; i++)
	{
		std::cout << getCardsString(hands[i].cards[0]) << " + " << getCardsString(hands[i].cards[1]) 
			<< ". Equity: " << equity[i] * 100 << "%" << std::endl;
	}
	std::cout << "Tie likelihood: " << equity[equity.size() - 1] * 100 << "%" << std::endl;
}

void testTie() 
{
	Deck deck(5);
	const unsigned int numTrials = 20;
	const unsigned int numHands = 2;

	Hand hands[numHands];
	//hands[0].setHand(Card::QUC, Card::JAD);
	//hands[1].setHand(Card::JAC, Card::QUD);
	hands[0].setHand(Card::TWH, Card::THH);
	hands[1].setHand(Card::TWD, Card::FOD);

	char* bestHands = (char*)alloca(sizeof(char) * 2);
	Pool pool(deck);

	for (int i = 0; i < numTrials; i++) {
		getBestHands(pool, hands, 2, bestHands);
		printPool(pool);
		if (bestHands[1] == -1) {
			std::cout << "Player " << (int)bestHands[0] << " took the W\n" << std::endl;
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

	for (unsigned int i = 0; i < numTrials; i++)
	{
		Deck deck;
		Hand* hands = new Hand[numHands];
		hands[0].setHand(deck.getNextHandCard(), deck.getNextHandCard());
		hands[1].setHand(deck.getNextHandCard(), deck.getNextHandCard());

		testCalcPreFlopEquity(hands, numHands, deck);
		delete[] hands;
		deck.resetEntireDeck();
	}
}

void testDeck()
{
	testNextCard();
	testRandomCardDist();
}

void testBestHand() 
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

void testPokerSim()
{
	//testDeck();
	//testBestHand();
	//testTie();

	testBestHandAndEquity(2, 3);
	//testBestHandAndEquity(6, 3);
}
