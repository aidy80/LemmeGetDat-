#include "pch.h"
#include "Card.h"
#include "UnitTests/TestBestHand.h"
#include "UnitTests/TestDeck.h"

void runUnitTests()
{
	//testDeck();
	runBHUnitTests();
}

int main()
{
	auto start = std::chrono::steady_clock::now();
	runUnitTests();
	auto end = std::chrono::steady_clock::now();
	std::cout << "ushort size: " << sizeof(unsigned short) << ". Max ushort: " << USHRT_MAX << std::endl;
	std::cout << std::chrono::duration<float, std::milli>(start - end).count() << " ms" << std::endl;
	return 0;
}