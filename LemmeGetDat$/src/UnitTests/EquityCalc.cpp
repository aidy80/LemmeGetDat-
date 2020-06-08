#include "EquityCalc.h"

static std::mutex numWinLock;
static std::mutex numTieLock;

constexpr unsigned int NUM_TRIALS = 1712304 * 10; // C(48, 5)
constexpr unsigned int NUM_THREADS = 8;
constexpr unsigned int NUM_TRIALS_PER_THREAD = NUM_TRIALS / NUM_THREADS;

void checkWinners(TwoDimArray& winners, Hand* hands, int numHands, Pool& pool) {
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
}

void sampleHands(int threadId, int numTrials, unsigned int* numberOfWins, unsigned int* numberOfTies, Deck deck, Hand* hands, int numHands) 
{
	TwoDimArray winners(1, numHands);
	Pool pool;
	deck.changeSeed(threadId * (int)time(NULL));
	
	for (int i = 0; i < numTrials; i++) {
		for (int j = 0; j < numHands; j++) {
			winners.set(0, j, j);
		}
		pool.newPool(deck);
		getBestHands(pool, hands, winners);

#ifdef _DEBUG
		checkWinners(winners, hands, numHands, pool);
#endif

		for (int j = 0; j < numHands && winners.get(0,j) != -1; j++) {
			if (j == 0) {
				if (winners.get(0, j + 1) == -1) {
					numWinLock.lock();
					numberOfWins[winners.get(0, j)]++;
					numWinLock.unlock();
				}
				else {
					numTieLock.lock();
					numberOfTies[winners.get(0, j)]++;
					numTieLock.unlock();
				}
			}
			else {
				numTieLock.lock();
				numberOfTies[winners.get(0, j)]++;
				numTieLock.unlock();
			}
		}
	}
}

std::vector<float> calcPreFlopEquity(Hand* hands, int numHands, Deck& deck)
{
	//TODO Deal with same card appearing twice
	//Idea: create a hashtable for each card, mark when it appears. If the card is produced again, 
	//just call the random card function again.
	//Nother idea: Create a deck class whose random function will only pull from remaining cards. <--Better

	unsigned int* numberOfWins = (unsigned int*)alloca(numHands * sizeof(unsigned int));
	unsigned int* numberOfTies = (unsigned int*)alloca(numHands * sizeof(unsigned int));

	for (int i = 0; i < numHands; i++) {
		numberOfWins[i] = 0;
		numberOfTies[i] = 0;
	}

#ifdef MULTITHREAD
	std::vector<std::thread> threads;
	threads.reserve(NUM_THREADS);
	for (int i = 0; i < NUM_THREADS; i++) 
	{
		threads.emplace_back(std::thread(sampleHands, i, NUM_TRIALS_PER_THREAD, numberOfWins, numberOfTies, deck, hands, numHands));
	}

	for (std::thread& currThread : threads) 
	{
		currThread.join();
	}
#else 
	sampleHands(1, NUM_TRIALS, numberOfWins, numberOfTies, deck, hands, numHands);
#endif

	std::vector<float> equity;
	equity.reserve(numHands * 2);

	for (int i = 0; i < numHands; i++)
	{
		equity.push_back((float)numberOfWins[i] / (float)NUM_TRIALS);
	}
	for (int i = 0; i < numHands; i++)
	{
		equity.push_back((float)numberOfTies[i] / (float)NUM_TRIALS);
	}

	return equity;
}