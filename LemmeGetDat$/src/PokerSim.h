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

	/*Find a solution to poker. Specifically, given a hand, 
	pool cards, action sequences, and the players position, find the 
	optimal distribution of correct decisions.*/
	void findSolution();

	/*Return the number of possible action sequence combos. Must pass in count = 0 for the starting value to get accurate results*/
	void numActSeq(int& count, int depth, int& maxDepth);
};
