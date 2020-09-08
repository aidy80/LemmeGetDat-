#include "pch.h"
#include "Card.h"
#include "PokerSim.h"
#include "UnitTests/TestBestHand.h"
#include "UnitTests/TestDeck.h"
#include "UnitTests/TestInfoSet.h"

constexpr int NUM_PLAYERS = 6;

void runUnitTests()
{
	//testDeck();
	//testBestHand();
	testInfoSet();
}

void countNumActSeq()
{
	PokerSim mccfr(NUM_PLAYERS);
	int count = 0;
	int maxDepth = -1;
	mccfr.numActSeq(count, 0, maxDepth);
	std::cout << "The number of action sequences found was " << count << " with a max depth of " << maxDepth << std::endl;
}

int main()
{
	auto start = std::chrono::steady_clock::now();

	runUnitTests();
	//countNumActSeq();

	auto end = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration<float, std::milli>(start - end).count() << " ms" << std::endl;
	return 0;
}