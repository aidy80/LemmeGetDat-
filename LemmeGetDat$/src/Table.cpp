#include "Table.h"

Table::Table(int numPlayers) :
	numPlayers(numPlayers),
	hands(new Hand[numPlayers]),
	currTurn(0),
	dealer(-1),
	stacks(new int[numPlayers]),
	preAllInStacks(new int[numPlayers]),
	firstToAct(new int[(int)Phase::Count]),
	raiseNum(0),
	raiser(new int[5 * (int)Phase::Count]), //Fix later to be optimal
	currHighBet(new int[5 * (int)Phase::Count]), //fix later to be optimal
	pot(numPlayers),
	pool(), deck(), 
	winners(1, numPlayers),
	phase(Phase::PREFLOP)
{
	startNewGame();
}

void Table::startNewGame() 
{
	deck.resetEntireDeck();

	dealer = checkPlayerOverflow(++dealer);
	currTurn = getUTG();

	for (int i = 0; i < numPlayers; i++)
	{
		hands[i].newHand(deck);
		stacks[i] = Table::START_STACK;
	}

	int smallBlind = getSmallBlind();
	int bigBlind = checkPlayerOverflow(smallBlind + 1);
	stacks[smallBlind] -= SMALL_BLIND;
	stacks[bigBlind] -= BIG_BLIND;

	raiseNum = 0;
	currHighBet[raiseNum] = BIG_BLIND;
	raiser[raiseNum] = bigBlind;

	firstToAct[0] = currTurn;
	pot.reset(numPlayers);
#ifdef _DEBUG
	for (int i = 1; i < (int)phase; i++) {
		firstToAct[i] = -1;
	}
	assert(pot.amount == SMALL_BLIND + BIG_BLIND);
	assert(TRAVERSER == 0);
#endif
	pool.newPool(deck);
	phase = Phase::PREFLOP;

	winners.resetArray();
	for (int i = 0; i < numPlayers - 1; i++) {
		winners.set(i, 0, TRAVERSER);
		winners.set(i, 1, i + 1);
	}
	getBestHands(pool, hands, winners);
}

Table::~Table()
{
	delete[]hands;
	delete[]stacks;
	delete[]raiser;
	delete[]currHighBet;
	delete[]firstToAct;
}

int Table::showdownValue() 
{
#ifdef _DEBUG
	assert(TRAVERSER == 0);
#endif
	int numTie = 0;
	for (int i = TRAVERSER + 1; i < numPlayers; i++)
	{
		if (!pot.folded[i]) 
		{
			if (winners.get(i - 1, 0) == TRAVERSER) 
			{
				if (winners.get(i - 1, 1) != -1) 
				{
					numTie += 1;
				}
			} else 
			{
				return lossValue();
			}
		}
	}

	if (numTie != 0) 
	{
		return tieValue(numTie);
	}

	return winValue();
}

int Table::nextTurn() 
{
	currTurn = nextUnfoldedPlayer();

	if (currTurn == raiser[(int)phase]) 
	{
		if (pot.numFolded == numPlayers - 1) 
		{
			return winValue();
		}
		
		switch(phase)
		{
		case Phase::PREFLOP:
			phase = Phase::FLOP;
			break;
		case Phase::FLOP:
			phase = Phase::TURN;
			break;
		case Phase::TURN:
			phase = Phase::RIVER;
			break;
		case Phase::RIVER:
			return showdownValue();
			break;
		} 
		currTurn = calcFirstToAct();
		firstToAct[(int)phase] = currTurn;

		raiseNum++;
		raiser[raiseNum] = currTurn;
		currHighBet[raiseNum] = 0;
	}
	return NOT_FINISHED;
}

void Table::prevTurn() 
{
	if (currTurn == firstToAct[(int)phase] && currHighBet[(int)phase] == 0)
	{
		switch (phase) 
		{
		case Phase::RIVER:
			phase = Phase::TURN;
			break;
		case Phase::TURN:
			phase = Phase::FLOP;
			break;
		case Phase::FLOP:
			phase = Phase::PREFLOP;
			break;
		case Phase::PREFLOP:
			startNewGame();
			return;
		}
		raiseNum--;
		currTurn = raiser[raiseNum];
	}

	currTurn = prevUnfoldedPlayer();
}

inline void Table::raise(int player, int raiseSize) 
{
	pot.amount += raiseSize;
	stacks[player] -= raiseSize;

	raiseNum++;
	raiser[raiseNum] = player;
	currHighBet[raiseNum] = raiseSize;
}

/*Processes every legal possible action except fold for the current turn then increments to the next turn*/
int Table::processAction(const ActionClass act) 
{ 
	switch (act)
	{
	case ActionClass::FOLD:
		if (currTurn == TRAVERSER) {
			return stacks[TRAVERSER] - START_STACK;
		}
		pot.folded[currTurn] = true;
		pot.numFolded++;
		break;
	case ActionClass::CALL:
		pot.amount += stacks[currTurn] - stacks[raiser[raiseNum]];
		stacks[currTurn] = stacks[raiser[raiseNum]];
		break;
	case ActionClass::ALL_IN:
		preAllInStacks[currTurn] = stacks[currTurn];
		raise(currTurn, stacks[currTurn]);
		break;
	case ActionClass::RAISE_HALF:
		raise(currTurn, raiseHalfSize());
		break;
	case ActionClass::RAISE_POT:
		raise(currTurn, raisePotSize());
		break;
	}
	nextTurn();
	return NOT_FINISHED;
}

inline void Table::unRaise(int player) 
{
	int newRaiseNum = raiseNum - 1;
	int raiseSize = currHighBet[raiseNum] - currHighBet[newRaiseNum];
	pot.amount -= raiseSize;
	stacks[player] += raiseSize;

	raiseNum = newRaiseNum;
}

void Table::unProcessAction(const ActionClass act)
{
	switch (act)
	{
	case ActionClass::FOLD:
		if (currTurn == TRAVERSER) 
		{
			return;
		}
		pot.folded[currTurn] = false;
		pot.numFolded--;
		break;
	case ActionClass::CALL:
		stacks[currTurn] += currHighBet[(int)phase];
		pot.amount -= currHighBet[(int)phase];
		break;
	case ActionClass::ALL_IN:
		pot.amount -= preAllInStacks[currTurn];
		stacks[currTurn] = preAllInStacks[currTurn];
		raiseNum--;
		break;
	case ActionClass::RAISE_HALF:
		unRaise(currTurn);
		break;
	case ActionClass::RAISE_POT:
		unRaise(currTurn);
		break;
	}
	prevTurn();
}

int Table::calcFirstToAct()
{
	for (int i = dealer + 1; i < numPlayers; i++)
	{
		if (!pot.folded[i])
		{
			return i;
		}
	}

	for (int i = 0; i < dealer + 1; i++)
	{
		if (!pot.folded[i]) 
		{
			return i;
		}
	}
	return -1;
}


int Table::firstIllegalAction()
{
	if (raiseHalfSize() > stacks[currTurn])
	{
		return (int)ActionClass::RAISE_HALF;
	}
	else if (raisePotSize() > stacks[currTurn]) 
	{
		return (int)ActionClass::RAISE_POT;
	}
	else {
		return (int)ActionClass::NUM_ACTIONS;
	}
}

void Table::printTurn() 
{
	std::cout << "The dealer is: " << dealer << std::endl;
	std::cout << "The phase is ";
	switch (phase)
	{
	case Phase::PREFLOP:
		std::cout << "preflop";
		break;
	case Phase::FLOP:
		std::cout << "flop";
		break;
	case Phase::TURN:
		std::cout << "turn";
		break;
	case Phase::RIVER:
		std::cout << "river";
		break;
	}
	std::cout << std::endl;
}

void Table::printCards()
{
	for (int i = 0; i < numPlayers; i++) 
	{
		printHand(hands[i]);
	}
	printPool(pool, numCardsInPhase(phase));
}

void Table::printMoney()
{
	std::cout << "Stack sizes are: ";
	for (int i = 0; i < numPlayers; i++)
	{
	std::cout << stacks[i] << ", ";
	}
	std::cout << "\n";
	std::cout << "The pot is " << pot.amount << " with players ";
	for (int i = 0; i < numPlayers; i++)
	{
	if (!pot.folded[i]) {
	std::cout << i << ", ";
	}
	}
	std::cout << "still in" << std::endl;
	
	std::cout << "Current raiser is player " << raiser[raiseNum] << " with a bet of " << currHighBet[raiseNum] << "\n";
}

void Table::printTable() 
{
	printCards();
	printTurn();
	printMoney();
	std::cout << std::endl;
}