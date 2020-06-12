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

void PokerSim::numActSeq(int& count)
{
	for (int i = 0; i < table.firstIllegalAction(); i++)
	{
		std::cout << "Player " << table.getCurrTurn() << " ";
		printAction((ActionClass)i);
		int value = table.processAction((ActionClass)i);
		table.printTable();
		if (value == NOT_FINISHED) {
			numActSeq(count);
			table.unProcessAction((ActionClass)i);
		} else {
			count++;
		}
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
