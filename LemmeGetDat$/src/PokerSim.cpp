#include "PokerSim.h"
#include "Instrumentor.h"

std::vector<float> calcPreFlopEquity(Hand* hands, unsigned char numHands, Deck& deck)
{
	//TODO Deal with same card appearing twice
	//Idea: create a hashtable for each card, mark when it appears. If the card is produced again, 
	//just call the random card function again.
	//Nother idea: Create a deck class whose random function will only pull from remaining cards. <--Better

	assert(numHands == 2);
	constexpr unsigned int numTrials = 1712304; // C(48, 5)
	Pool pool(deck);
	unsigned int* numberOfWins = (unsigned int*)alloca(numHands * sizeof(unsigned int));
	unsigned int numberOfTies = 0;

	for (int i = 0; i < numHands; i++) {
		numberOfWins[i] = 0;
	}
	
	{
		InstrumentationTimer inst("longLoop");
		char* winners = (char*)alloca(sizeof(char) * numHands);
		for (unsigned int i = 0; i < numTrials; i++) {
			pool.newPool(deck);
			getBestHands(pool, hands, numHands, winners);

			for (int j = 0; j < numHands && winners[j] != -1; j++) {
				if (j == 0) {
					numberOfWins[winners[j]]++;
				}
				else if (j == 1) {
					numberOfTies++;
					numberOfWins[0]--;
				}
			}
		}
	}

	std::vector<float> equity;
	equity.reserve(numHands);

	for (unsigned int i = 0; i < numHands; i++)
	{
		equity.push_back((float)numberOfWins[i] / (float)numTrials);
	}
	equity.push_back((float)numberOfTies / (float)numTrials);

	return equity;
}
