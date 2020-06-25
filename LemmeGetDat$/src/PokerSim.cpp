#include "PokerSim.h"

PokerSim::PokerSim(int numPlayers) :
	infoset(),
	table(numPlayers),
	actSeq(new ActionClass[MAX_ACTIONS]),
	lenActSeq(0)
{
}

PokerSim::~PokerSim()
{
	delete[]actSeq;
}

void PokerSim::findSolution()
{}

void PokerSim::numActSeq(int& count, int depth, int& maxDepth)
{
	int player = table.getCurrTurn();
	ActionClass* acts = table.getLegalActions();
	if (depth > maxDepth) 
	{
		maxDepth = depth;
	}
	for (ActionClass* currAct = acts; *currAct != ActionClass::NULL_ACTION; currAct++)
	{
		#ifdef VERBOSE
			std::cout << "Player " << table.getCurrTurn() << " ";
			printAction(*currAct);
		#endif

		int value = table.processAction(*currAct);

		#ifdef VERBOSE
			std::cout << "Depth: " << depth << " Count: " << count << std::endl;
			table.printTable();
		#endif VERBOSE

		if (value == NOT_FINISHED) {
			count++;

			numActSeq(count, depth + 1, maxDepth);
		}
		else {
			//count++;
			#ifdef VERBOSE
				std::cout << "The traverser recieved " << value << " blinds" << std::endl;
				std::cout << "CURR COUNT " << count << "\n" << std::endl;
			#endif
		}

		table.unProcessAction(*currAct, player);

		#ifdef VERBOSE
			std::cout << "Player " << table.getCurrTurn() << " reverse ";
			printAction(*currAct);
			std::cout << "Depth: " << depth << " Count: " << count << std::endl;
			table.printTable();
		#endif VERBOSE
	}
	delete[]acts;
}
