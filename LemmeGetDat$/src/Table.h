#pragma once

#include "Deck.h"
#include "PokerCards.h"
#include "InfoSet.h"
#include "BestHand.h"
#include "TwoDimArray.h"

constexpr int P1 = 0x1;
constexpr int P2 = 0x2;
constexpr int P3 = 0x4;
constexpr int P4 = 0x8;
constexpr int P5 = 0x10;
constexpr int P6 = 0x20;

constexpr int PLAYER_BIT_PACK[6] = { P1, P2, P3, P4, P5, P6 };

constexpr int NOT_FINISHED = SHRT_MAX;

/*Representation of the current phase of a game*/
enum class Phase
{
	PREFLOP, FLOP, TURN, RIVER, Count
};

/*State of the pot during one turn in the game*/
struct Pot
{
	int amount;
	bool* folded;
	int numFolded = 0;
	const int TOTAL_BLINDS = 3;

	Pot() : folded(nullptr), amount(0) {}

	Pot(const int numPlayers) : folded(new bool[numPlayers]), amount(0)
	{	
		reset(numPlayers);
	}

	~Pot()
	{
		delete[]folded;
	}

	void reset(const int numPlayers) 
	{
		amount = TOTAL_BLINDS;
		for (int i = 0; i < numPlayers; i++)
		{
			folded[i] = false;
		}
		numFolded = 0;
	}
};

/*Implementation of the "Table". 
Holds all state data: the hands, the players stacks, 
the pool, etc. Additionally, it will hold 
the key information about previous states such that 
(lossless) backwards recursion becomes possible*/
class Table
{
private:
	static constexpr int SMALL_BLIND = 1;
	static constexpr int BIG_BLIND = SMALL_BLIND * 2;
	static constexpr int START_STACK = 200;
	static constexpr int OPEN_SIZE = 3 * BIG_BLIND;
	static constexpr int BET3_SIZE = 10 * BIG_BLIND;
	static constexpr int BET4_SIZE = 22 * BIG_BLIND;

	static constexpr int ALL_IN_THRES = 33 * BIG_BLIND;

	Hand* hands;

	int numPlayers;
	int dealer;
	int currTurn;

	int* stacks;
	int* firstToAct;
	bool* folded;
	int numFolded;

	int potNum;
	int* pots;

	int raiseNum;
	int* raiser; 
	int* currHighBet; 

	int raiseRaise;
	int checkRaise;
	int newTurnRaise;

	//Pot pot;

	Deck deck;
	Pool pool;

	TwoDimArray winners;

	Phase phase;

	/*Returns the number of pool cards players have access to in a given phase*/
	static int numCardsInPhase(Phase phase);

	/*If playerIndex == numPlayer return 0, otherwise playerIndex*/
	int checkPlayerOverflow(int playerIndex) const;

	/*If playerIndex < 0 return numPlayer - 1, otherwise playerIndex*/
	int checkPlayerUnderflow(int playerIndex) const;

	/*Returns the index of the small blind*/
	int getSmallBlind() const;

	/*Function to increment to the next players turn. (does not change stacks of fold states - only currTurn and PHASE). 
	If it folds around to the raiser, return the value*/
	int nextTurn();

	/*Function to revert the table state backwards to the previous turn (does not change stacks or fold states - only currTurn and PHASE)*/
	void prevTurn();

	/*Return the stacks index corresponding to the phase and player index*/
	int getStacksIndex(Phase phase, int player) const;

	/*Return the next unfolded player after currTurn*/
	int nextUnfoldedPlayer();

	/*Return the next unfolded player after currTurn*/
	int prevUnfoldedPlayer();

	/*Calc the first (unfolded) person to act on the phase this is called*/
	int calcFirstToAct();

	/*Update the table after a player raises*/
	void raise(int player, int raiseSize);

	/*reverse the update to the table from the last raise*/
	void unRaise(int player);

	/*See whether there are any other players who can still make legal actions*/
	bool leftToAct();

	/*Return value of traverser at showdown*/
	int showdownValue();

	/*Return value of traverser when all but one player folds*/
	int winValue(int playerNum);

	int raiseHalfSize();

	int raisePotSize();

	int raiseQurtSize();

	int raiseThrQurtSize();

	void testPrevPhase();

	int numRaisesThisPhase();

	friend int InfoSet::getInfoId(const Table& table) const;
public:
	Table(int numPlayers);
	~Table();

	Table(const Table&) = delete;
	Table& operator=(const Table&) = delete;

	/*Gives initial values used at the beginning of a game*/
	void startNewGame();

	/*Going in forward recursion, gives user ability to progress game state by making a given player act. Does NOT return the value of the action*/
	int processAction(const ActionClass act);
	
	/*Made for back tracking recursion, gives user ability to undo an action. unlike processAction, this specifically requires the player in question
	(ideally that they did last altho there is no check)*/
	void unProcessAction(const ActionClass act, const int player);

	/*With the list of actionClasses with the largest being the largest (possibly illegal) raise size, 
	  return the index of the last element which is illegal */
	//int firstIllegalAction();

	ActionClass* getLegalActions();

	/*With the list of actionclasses, return the first action u can do. Realistically, this just prevents someone from folding when they can just call */
	//int firstLegalAction();

	int getUTG() const;
	int getCurrTurn() const;
	int getNumFolded() const;
	int getFolded(int i) const;
	Phase getPhase() const;

	/*Print the information in the table. Used for testing*/
	void printCards();
	void printTurn();
	void printMoney();
	void printTable();
};

inline int Table::numCardsInPhase(Phase phase)
{
#ifdef _DEBUG
	assert(phase != Phase::Count);
#endif
	switch(phase)
	{
	case Phase::PREFLOP:
		return 0;
	case Phase::FLOP:
		return 3;
	case Phase::TURN:
		return 4;
	case Phase::RIVER:
		return 5;
	}
	return -1;
}

