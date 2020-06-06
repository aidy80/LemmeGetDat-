#include "PokerSim.h"

std::vector<float> calcPreFlopEquity(Hand* hands, int numHands, Deck& deck)
{
	//TODO Deal with same card appearing twice
	//Idea: create a hashtable for each card, mark when it appears. If the card is produced again, 
	//just call the random card function again.
	//Nother idea: Create a deck class whose random function will only pull from remaining cards. <--Better

	constexpr unsigned int numTrials = 1712304; // C(48, 5)
	Pool pool(deck);
	unsigned int* numberOfWins = (unsigned int*)alloca(numHands * sizeof(unsigned int));
	unsigned int* numberOfTies = (unsigned int*)alloca(numHands * sizeof(unsigned int));

	for (int i = 0; i < numHands; i++) {
		numberOfWins[i] = 0;
		numberOfTies[i] = 0;
	}

	TwoDimArray winners(1, numHands);
	
	for (unsigned int i = 0; i < numTrials; i++) {
		for (int j = 0; j < numHands; j++) {
			winners.set(0, j, j);
		}
		pool.newPool(deck);
		getBestHands(pool, hands, winners);

#ifdef _DEBUG
		if (winners.get(0, 0) == -1) //Error check
		{
			printPool(pool);
			for (int m = 0; m < numHands; m++) {
				printHand(hands[m]);
			}
			std::cout << "The winners are: ";
			for (int m = 0; m < numHands; m++) {
				std::cout << (int)winners.get(0, m) << " ";
			}
			std::cout << std::endl;
		}
#endif

		for (int j = 0; j < numHands && winners.get(0,j) != -1; j++) {
			if (j == 0) {
				if (winners.get(0, j + 1) == -1) {
					numberOfWins[winners.get(0, j)]++;
				}
				else {
					numberOfTies[winners.get(0, j)]++;
				}
			}
			else {
				numberOfTies[winners.get(0, j)]++;
			}
		}
	}

	std::vector<float> equity;
	equity.reserve(numHands * 2);

	for (int i = 0; i < numHands; i++)
	{
		equity.push_back((float)numberOfWins[i] / (float)numTrials);
	}
	for (int i = 0; i < numHands; i++)
	{
		equity.push_back((float)numberOfTies[i] / (float)numTrials);
	}

	return equity;
}
