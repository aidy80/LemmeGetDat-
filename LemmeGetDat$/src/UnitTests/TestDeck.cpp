#include "TestDeck.h"

void testCardEnum()
{
	std::cout << "ENUM TEST" << std::endl;

	std::cout << "My hand is " << (int)(char)CardEnum::TWD << " and " << (int)(char)CardEnum::KIS << std::endl;
	std::cout << "Expected: 0 and 50" << std::endl;

	std::cout << "My hand is " << (int)(char)CardEnum::THC << " and " << (int)(char)CardEnum::TWH << std::endl;
	std::cout << "Expected: 27 and 13\n\n" << std::endl;
}

void testPrintCard()
{
	std::cout << "PRINT CARD TEST" << std::endl;
	std::cout << "Expected: Ace of Clubs. Actual: " << getCardsString(CardEnum::ACC) << std::endl;
	std::cout << "Expected: Three of Hearts. Actual: " << getCardsString(CardEnum::THH) << std::endl;
	std::cout << "Expected: Six of Diamonds. Actual: " << getCardsString(CardEnum::SID) << std::endl;
	std::cout << "Expected: Jack of Spades. Actual: " << getCardsString(CardEnum::JAS) << "\n\n" << std::endl;
}


/* PokerSim.cpp */
void testNextCard()
{
	std::cout << "RANDOM CARD TEST" << std::endl; 
	std::unordered_map<CardEnum, bool> seenCards;
	Deck deck(5);
	for (int i = 0; i < NUM_CARDS_IN_DECK; i++)
	{
		deck.printDeck();
		Card newCard = deck.getNextPoolCard();
		std::cout << "This is another random card: " << getCardsString(newCard) << std::endl;
		assert(!seenCards[CardToEnum(newCard)]);
		seenCards[CardToEnum(newCard)] = true;
	}
	std::cout << std::endl;
}

void testRandomCardDist()
{
	const int numTrails = (int)pow(10, 7);
	Deck deck(5);

	std::vector<int> cardDist(NUM_CARDS_IN_DECK, 0);

	for (int i = 0; i < numTrails; i++)
	{
		Card newCard = deck.getNextPoolCard();
		cardDist[(int)CardToEnum(newCard)]++;
		deck.resetEntireDeck();
		if (i % 1000000 == 0) {
			std::cout << i << std::endl;
		}
	}

	std::vector<float> cardPercentDist(52, 0);

	for (int i = 0; i < NUM_CARDS_IN_DECK; i++) {
		cardPercentDist[i] = (float)cardDist[i] / (float)numTrails;
		std::cout << cardPercentDist[i] << " ";
	}
	std::cout << std::endl;

	float mean = (float)numTrails/(float)NUM_CARDS_IN_DECK;
	float variance = 0;
	int totCheck = 0;
	for (int i = 0; i < NUM_CARDS_IN_DECK; i++)
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

void testDeck()
{
	testCardEnum();
	testPrintCard();
	testNextCard();
	testRandomCardDist();
}