#include "pch.h"
#include "Card.h"
#include "UnitTests.h"
#include "Instrumentor.h"

void runUnitTests()
{
	//testCard();
	Instrumentor::Get().BeginSession("Profile");
	testPokerSim();
	Instrumentor::Get().EndSession();
}

int main()
{
	fast_srand((unsigned int)time(NULL));
	srand(time(NULL));
	runUnitTests();
	return 0;
}