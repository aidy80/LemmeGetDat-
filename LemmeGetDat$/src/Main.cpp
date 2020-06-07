#include "pch.h"
#include "Card.h"
#include "BHunitTests.h"

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
	std::cout << std::chrono::duration<float, std::milli>(start - end).count() << " ms" << std::endl;
	return 0;
}