#include "Table.h"

Table::Table(int numPlayers) :
	numPlayers(numPlayers),
	hands(new Hand[numPlayers]),
	currTurn(0),
	dealer(-1),
	stacks(new int[numPlayers + 1]),
	folded(new bool[numPlayers]),
	numFolded(0),
	firstToAct(new int[(int)Phase::Count]),
	raiseNum(0),
	raiser(new int[200 * (int)Phase::Count]), //Fix later to be optimal
	currHighBet(new int[200 * (int)Phase::Count]), //fix later to be optimal
	pots(new int[500 * (int)Phase::Count]), //Fix later to be optimal
	pool(), deck(), 
	winners(1, numPlayers),
	phase(Phase::PREFLOP)
{
	startNewGame();

	raiseRaise = 0;
	checkRaise = 0;
	newTurnRaise = 0;
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
		folded[i] = false;
	}
	numFolded = 0;

	int smallBlind = getSmallBlind();
	int bigBlind = checkPlayerOverflow(smallBlind + 1);
	stacks[smallBlind] -= SMALL_BLIND;
	stacks[bigBlind] -= BIG_BLIND;
	stacks[numPlayers] = START_STACK - BIG_BLIND;

	raiseNum = 0;
	currHighBet[raiseNum] = BIG_BLIND;
	raiser[raiseNum] = numPlayers;
	potNum = 0;
	pots[potNum] = SMALL_BLIND + BIG_BLIND;

	firstToAct[0] = currTurn;
#ifdef _DEBUG
	for (int i = 1; i < (int)phase; i++) {
		firstToAct[i] = -1;
	}
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
		if (!folded[i]) 
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

	if (currTurn == raiser[raiseNum]) 
	{
		if (numFolded == numPlayers - 1) 
		{
			return winValue();
		} else if (phase == Phase::RIVER || stacks[raiser[raiseNum]] == 0) 
		{
			return showdownValue();
		}
		
		phase = (Phase)((int)(phase) + 1);
		currTurn = calcFirstToAct();
		firstToAct[(int)phase] = currTurn;

		raiseNum++;
		raiser[raiseNum] = currTurn;
		newTurnRaise++;
		currHighBet[raiseNum] = -1;
	}
	return NOT_FINISHED;
}

void Table::testPrevPhase() 
{
	if (currHighBet[raiseNum] == -1)
	{
		phase = (Phase)((int)(phase) - 1);
#ifdef _DEBUG
		assert((int)(phase) != -1);
#endif
		raiseNum--;
		newTurnRaise--;
		currTurn = raiser[raiseNum];
	}
}

void Table::prevTurn() 
{
	if (currHighBet[raiseNum] == -1)
	{
		phase = (Phase)((int)(phase) - 1);
#ifdef _DEBUG
		assert((int)(phase) != -1);
#endif
		raiseNum--;
		newTurnRaise--;
		currTurn = raiser[raiseNum];
	}

	currTurn = prevUnfoldedPlayer();
}

inline void Table::raise(int player, int raiseSize) 
{
#ifdef _DEBUG
	assert(raiseSize >= 0);
	assert(! (phase != Phase::PREFLOP && 
			  stacks[currTurn] - raiseSize > START_STACK - BIG_BLIND));
#endif
	potNum++;
	pots[potNum] = pots[potNum - 1] + raiseSize;
	stacks[player] -= raiseSize;

	raiseNum++;
	raiseRaise++;
	raiser[raiseNum] = player;
	currHighBet[raiseNum] = raiseSize;
}

/*Processes every legal possible action except fold for the current turn then increments to the next turn*/
int Table::processAction(const ActionClass act) 
{ 
#ifdef _DEBUG
	assert(raiseNum == raiseRaise + checkRaise + newTurnRaise);
#endif
	switch (act)
	{
	case ActionClass::FOLD:
		folded[currTurn] = true;
		numFolded++;

		if (currTurn == TRAVERSER) {
			currTurn = nextUnfoldedPlayer();
			return stacks[TRAVERSER] - START_STACK;
		} else if (numFolded == numPlayers - 1) 
		{
			return winValue();
		}
		break;
	case ActionClass::CALL:
		if (currHighBet[raiseNum] == -1) 
		{
			raiseNum++;
			checkRaise++;
			raiser[raiseNum] = currTurn;
			currHighBet[raiseNum] = 0;
		}

		potNum++;
		pots[potNum] = stacks[currTurn] - stacks[raiser[raiseNum]] + pots[potNum - 1];
		stacks[currTurn] = stacks[raiser[raiseNum]];

		if (phase == Phase::PREFLOP && raiseNum == 0)  
		{
			raiseNum++;
			checkRaise++;
			raiser[raiseNum] = currTurn;
		}

		#ifdef _DEBUG
			assert(stacks[currTurn] <= START_STACK - BIG_BLIND);
		#endif
		break;
	case ActionClass::ALL_IN:
		raise(currTurn, stacks[currTurn]);
		break;
	case ActionClass::RAISE_HALF:
		raise(currTurn, raiseHalfSize());
		break;
	case ActionClass::RAISE_POT:
		raise(currTurn, raisePotSize());
		break;
	}
#ifdef _DEBUG
	//for (int i = 0; i < numPlayers; i++) 
	//{
	//	assert(stacks[i] > -1);
	//}
#endif

	return nextTurn();
}

inline void Table::unRaise(int player) 
{
#ifdef _DEBUG
	assert(! (phase != Phase::PREFLOP && 
			  stacks[currTurn] + pots[potNum] - pots[potNum - 1] > START_STACK - BIG_BLIND));
#endif
	stacks[player] += pots[potNum] - pots[potNum - 1];
	raiseNum--;
	raiseRaise--;
	potNum--;
}

void Table::unProcessAction(const ActionClass act, const int player)
{
#ifdef _DEBUG
	assert(raiseNum == raiseRaise + checkRaise + newTurnRaise);
#endif
	switch (act)
	{
	case ActionClass::FOLD:
		testPrevPhase();
		folded[player] = false;
		numFolded--;
		currTurn = player;
		break;
	case ActionClass::CALL:
		testPrevPhase();
		currTurn = prevUnfoldedPlayer();

		#ifdef _DEBUG
			assert(player == currTurn);
		#endif

		if (firstToAct[(int)phase] == currTurn && currHighBet[raiseNum - 1] == -1)
		{
			raiseNum--;
			checkRaise--;
		}

		#ifdef _DEBUG
			assert(! (phase != Phase::PREFLOP && stacks[currTurn] + pots[potNum] - pots[potNum - 1] > START_STACK - BIG_BLIND));
		#endif

		stacks[currTurn] += pots[potNum] - pots[potNum - 1];
		potNum--;

		if (phase == Phase::PREFLOP && raiseNum == 1 && raiser[raiseNum] == currTurn)  
		{
			raiseNum--;
			checkRaise--;
			raiser[raiseNum] = numPlayers;
		}

		break;
	default: 
		testPrevPhase();
		currTurn = prevUnfoldedPlayer();

		unRaise(currTurn);
		break;
	}
}

int Table::calcFirstToAct()
{
	for (int i = dealer + 1; i < numPlayers; i++)
	{
		if (!folded[i])
		{
			return i;
		}
	}

	for (int i = 0; i < dealer + 1; i++)
	{
		if (!folded[i]) 
		{
			return i;
		}
	}
	return -1;
}

int Table::firstLegalAction()
{
	if (currHighBet[raiseNum] == 0 || currHighBet[raiseNum] == -1) 
	{
		return 1;
	} else 
	{
		return 0;
	}
}

int Table::firstIllegalAction()
{
	if (stacks[currTurn] <= currHighBet[raiseNum]) 
	{
		return (int)ActionClass::ALL_IN;
	} 
	else if (phase == Phase::PREFLOP) 
	{
		return (int)ActionClass::RAISE_HALF;
	}
	else {
		if (stacks[currTurn] <= currHighBet[raiseNum])
		{
			return (int)ActionClass::ALL_IN;
		}
		else if (raiseHalfSize() > stacks[currTurn])
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
	std::cout << "The pot is " << pots[potNum] << " with " << numPlayers - numFolded << " players still in: ";
	int stillIn = 0;
	for (int i = 0; i < numPlayers; i++)
	{
		if (!folded[i]) {
			std::cout << i;

			stillIn++;
			if (stillIn != numPlayers - numFolded) 
			{
				std::cout << ", ";
			} else 
			{
				std::cout << std::endl;
			}
		}
	}
#ifdef _DEBUG
	//assert(stillIn == numPlayers - numFolded);
#endif
	
	std::cout << "Current raiser is player " << raiser[raiseNum] << " with a bet of " << currHighBet[raiseNum] << "\n";
	std::cout << "raiseRaise: " << raiseRaise << ". checkRaise " << checkRaise << ". newTurnRaise: " << newTurnRaise << ". numRaise: " << raiseNum << " potNum: " << potNum << "." << std::endl;
}

void Table::printTable() 
{
	//printCards();
	printTurn();
	printMoney();
	std::cout << std::endl;
}

int Table::checkPlayerOverflow(int playerIndex) const
{
	return playerIndex == numPlayers ? 0 : playerIndex;
}

int Table::checkPlayerUnderflow(int playerIndex) const
{
	return playerIndex < 0 ? numPlayers - 1 : playerIndex;
}

int Table::getStacksIndex(Phase phase, int player) const
{
	return numPlayers * (int)phase + player;
}

int Table::nextUnfoldedPlayer()
{
#ifdef _DEBUG
	bool allFolded = true;
	for (int i = 0; i < numPlayers; i++) 
	{
		if (!folded[i])
		{
			allFolded = false;
			break;
		}
	}
	assert(!allFolded);
#endif
	int nextUnfolded = currTurn;
	do 
	{
		nextUnfolded = checkPlayerOverflow(++nextUnfolded);
	} while (folded[nextUnfolded]);
	return nextUnfolded;
}

int Table::prevUnfoldedPlayer()
{
#ifdef _DEBUG
	bool allFolded = true;
	for (int i = 0; i < numPlayers; i++) 
	{
		if (!folded[i])
		{
			allFolded = false;
			break;
		}
	}
	assert(!allFolded);
#endif
	int prevUnfolded = currTurn;
	do 
	{
		prevUnfolded = checkPlayerUnderflow(--prevUnfolded);
	} while (folded[prevUnfolded]);
	return prevUnfolded;
}

int Table::getSmallBlind() const 
{
	return (dealer == numPlayers - 1) ? 0 : dealer + 1;
}

int Table::getUTG() const 
{
	return dealer - 3 > -1 ? dealer - 3 : dealer + 3;
}

int Table::getCurrTurn() const
{
	return currTurn;
}

Phase Table::getPhase() const
{
	return phase;
}

int Table::getNumFolded() const
{
	return numFolded;
}

int Table::getFolded(int i) const
{
	return folded[i];
}

bool Table::leftToAct() 
{
	if (phase == Phase::RIVER && nextUnfoldedPlayer() == raiser[raiseNum]) 
	{
			return false;
	}
	return true;
}

int Table::winValue()
{
	return stacks[TRAVERSER] - START_STACK + pots[potNum];
}

int Table::tieValue(int numTies) 
{
	int potValue;
	if (numTies == 2) 
	{
		potValue = pots[potNum] >> 1;
	}
	else {
		potValue = pots[potNum] / numTies;
	}
	return stacks[TRAVERSER] - START_STACK + potValue;
}

/*To be inlined*/
int Table::lossValue()
{
	return stacks[TRAVERSER] - START_STACK;
}

int Table::raiseHalfSize()
{
	if (currHighBet[raiseNum] == -1) 
	{
		return pots[potNum] >> 1;
	}
	return 2 * currHighBet[raiseNum] + (pots[potNum] / 2);
}

int Table::raisePotSize()
{
	if (currHighBet[raiseNum] == -1) 
	{
		return pots[potNum];
	}
	return 2 * currHighBet[raiseNum] + pots[potNum];
}