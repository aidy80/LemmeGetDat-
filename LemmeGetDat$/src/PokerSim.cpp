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

void PokerSim::MCCFR()
{
	traverseMCCFR(ActionClass::NULL_ACTION);
}

void PokerSim::numActSeq(int& count, int depth)
{
	//int firstIllegal = table.firstIllegalAction();
	//int firstLegal = table.firstLegalAction();
	//for (int i = firstLegal; i < firstIllegal; i++)

	int player = table.getCurrTurn();
	ActionClass* acts = table.getLegalActions();
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
			numActSeq(count, depth + 1);
		}
		else {
			count++;
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

void PokerSim::traverseMCCFR(ActionClass initAction)
{
	Regret* currReg;
	if (initAction == ActionClass::NULL_ACTION) 
	{
		currReg = infoset.getRegrets(table.getUTG(), actSeq, lenActSeq);
	}
}
