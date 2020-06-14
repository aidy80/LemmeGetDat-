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
	int player = table.getCurrTurn();
	int firstIllegal = table.firstIllegalAction();
	int firstLegal = table.firstLegalAction();
	//for (int i = firstLegal; i < firstIllegal; i++)

	for (int i = table.firstLegalAction(); i < table.firstIllegalAction(); i++)
	{
		//std::cout << "Player " << table.getCurrTurn() << " ";
		//printAction((ActionClass)i);
		int value = table.processAction((ActionClass)i);
		//table.printTable();

		if (value == NOT_FINISHED) {
			numActSeq(count, depth + 1);
		} else {
			count++;
			//std::cout << "The traverser recieved " << value << " blinds" << std::endl;
			//std::cout << "CURR COUNT " << count << "\n" << std::endl;
		}

		if (count == 22171 && depth == 13 && i == 1)  
		{
			std::cout << "HERE\n";
		}

		//std::cout << "Player " << player << " reverse ";
		//printAction((ActionClass)i);
		table.unProcessAction((ActionClass)i, player);
		//table.printTable();
	}
}

void PokerSim::traverseMCCFR(ActionClass initAction)
{
	Regret* currReg;
	if (initAction == ActionClass::NULL_ACTION) 
	{
		currReg = infoset.getRegrets(table.getUTG(), actSeq, lenActSeq);
	}
}


