#pragma once

#include "Table.h"
#include "InfoSet.h"
#include "BestHand.h"

constexpr int MAX_ACTIONS = 50;

class PokerSim 
{
private:
	InfoSet infoset;
	Table table;
	ActionClass* actSeq;
	int lenActSeq;
public:
	PokerSim(int numPlayers);
	PokerSim() = delete;

	~PokerSim();

	void MCCFR();

	void traverseMCCFR(ActionClass action);
	
	void traverseMCCFRpruned(ActionClass action);

	/*Recursively the value of a given action
	and each of the possible following actions*/
	int getValue(ActionClass action);

	/*Return the number of possible action sequence combos. Must pass in count = 0 for the starting value to get accurate results*/
	void numActSeq(int& count, int depth);

	void updateRegrets(ActionClass action);

	Regret* getAvailableActions();
};
