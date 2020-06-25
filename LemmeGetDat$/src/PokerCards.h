#pragma once

#include "Deck.h"

constexpr int NUM_FLOP_CARDS = 3;
constexpr int NUM_POOL_CARDS = 5;

constexpr int FLOP1_FLAG = 0x1;
constexpr int FLOP2_FLAG = 0x2;
constexpr int FLOP3_FLAG = 0x4;
constexpr int TURN_FLAG = 0x8;
constexpr int RIVER_FLAG = 0x10;

constexpr int INFO_KEY_SIZE = 160;

constexpr int POOL_BIT_PACK[5] = { FLOP1_FLAG, FLOP2_FLAG, FLOP3_FLAG, TURN_FLAG, RIVER_FLAG };

/*Representation of the current phase of a game*/
enum class Phase
{
	PREFLOP, FLOP, TURN, RIVER, Count
};

/*Representation of a players hand*/
struct Hand
{
	Card cards[2];

	/*Deal a random hand from the deck*/
	Hand(Deck& deck) : cards{deck.getNextHandCard(), deck.getNextHandCard()} {}

	/*Create a specific, desired hand*/
	Hand(const Card* initHand) : cards{ initHand[0], initHand[1] } {}
	Hand(const CardEnum* initHand) : cards{ { getCardsNumber(initHand[0]), getCardsSuit(initHand[0]) },
									  { getCardsNumber(initHand[1]), getCardsSuit(initHand[1]) } } {}
	Hand(const CardEnum card1, const CardEnum card2) : cards{ { getCardsNumber(card1), getCardsSuit(card1) },
														      { getCardsNumber(card2), getCardsSuit(card2) } } {}
	Hand() : cards() {}

	/*Different ways to change the cards in a given hand*/
	void setHand(const Card* newHand) {
		cards[0] = newHand[0];
		cards[1] = newHand[1];
	}

	void setHand(const Card newCard1, const Card newCard2) {
		cards[0] = newCard1;
		cards[1] = newCard2;
	}

	void setHand(const CardEnum* newHand) {
		cards[0].number = getCardsNumber(newHand[0]);
		cards[1].number = getCardsNumber(newHand[1]);

		cards[0].suit = getCardsSuit(newHand[0]);
		cards[1].suit = getCardsSuit(newHand[1]);
	}
	
	/*Deal a new random hand from the deck*/
	void newHand(Deck& deck) 
	{
		//Make sure that resetEntireDeck was called before this
		cards[0] = deck.getNextHandCard();
		cards[1] = deck.getNextHandCard();
	}
};

enum class HandID 
{
	TWTW, TWTHo, TWFOo, TWFIo, TWSIo, TWSEo, TWEIo, TWNIo, TWTEo, TWJAo, TWQUo, TWKIo, TWACo, 
	THTH, THFOo, THFIo, THSIo, THSEo, THEIo, THNIo, THTEo, THJAo, THQUo, THKIo, THACo, 
	FOFO, FOFIo, FOSIo, FOSEo, FOEIo, FONIo, FOTEo, FOJAo, FOQUo, FOKIo, FOACo, 
	FIFI, FISIo, FISEo, FIEIo, FINIo, FITEo, FIJAo, FIQUo, FIKIo, FIACo, 
	SISI, SISEo, SIEIo, SINIo, SITEo, SIJAo, SIQUo, SIKIo, SIACo, 
	SESE, SEEIo, SENIo, SETEo, SEJAo, SEQUo, SEKIo, SEACo, 
	EIEI, EINIo, EITEo, EIJAo, EIQUo, EIKIo, EIACo, 
	NINI, NITEo, NIJAo, NIQUo, NIKIo, NIACo, 
	TETE, TEJAo, TEQUo, TEKIo, TEACo, 
	JAJA, JAQUo, JAKIo, JAACo, 
	QUQU, QUKIo, QUACo, 
	KIKI, KIACo, 
	ACAC,
	TWTHs, TWFOs, TWFIs, TWSIs, TWSEs, TWEIs, TWNIs, TWTEs, TWJAs, TWQUs, TWKIs, TWACs, 
	THFOs, THFIs, THSIs, THSEs, THEIs, THNIs, THTEs, THJAs, THQUs, THKIs, THACs, 
	FOFIs, FOSIs, FOSEs, FOEIs, FONIs, FOTEs, FOJAs, FOQUs, FOKIs, FOACs, 
	FISIs, FISEs, FIEIs, FINIs, FITEs, FIJAs, FIQUs, FIKIs, FIACs, 
	SISEs, SIEIs, SINIs, SITEs, SIJAs, SIQUs, SIKIs, SIACs, 
	SEEIs, SENIs, SETEs, SEJAs, SEQUs, SEKIs, SEACs, 
	EINIs, EITEs, EIJAs, EIQUs, EIKIs, EIACs, 
	NITEs, NIJAs, NIQUs, NIKIs, NIACs, 
	TEJAs, TEQUs, TEKIs, TEACs, 
	JAQUs, JAKIs, JAACs, 
	QUKIs, QUACs, 
	KIACs,
	NUM_HAND_COMBOS
};

/*A different representation of the pools*/
struct PoolCardNames
{
	Card flop[3];
	Card turn;
	Card river;
};

struct Pool {
	/*Union makes it possible to access pool cards sequentially or by name*/
	union
	{
		PoolCardNames names;
		Card cards[5];
	};

	/*Deal a random pool from the deck*/
	Pool(Deck& d) : cards{ d.getNextPoolCard(), d.getNextPoolCard(),
						  d.getNextPoolCard(), d.getNextPoolCard(), d.getNextPoolCard() } {}

	/*Use a very specific pool*/
	Pool(const Card* initCards) : cards{ initCards[0], initCards[1], initCards[2], initCards[3], initCards[4] } {}
	Pool(const CardEnum* initCards)
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i].number = getCardsNumber(initCards[i]);
			cards[i].suit = getCardsSuit(initCards[i]);
		}
	}

	/*Copy and assignment constructors*/
	Pool(const Pool& obj)
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = obj.cards[i];
		}
	}

	Pool& operator=(const Pool& obj)
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = obj.cards[i];
		}
		return *this;
	}

	Pool() : cards() {}

	/*Put the old pool back into the deak then deal a new pool from the deck*/
	void newPool(Deck& deck) {
		deck.resetPool();
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = deck.getNextPoolCard();
		}
	}

	/*Set a new pool of cards from a desired list*/
	void setCards(const CardEnum* newCards) {
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i].number = getCardsNumber(newCards[i]);
			cards[i].suit = getCardsSuit(newCards[i]);
		}
	}

	void setCards(const Card* newCards) {
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = newCards[i];
		}
	}
};

/*Retrieve the id for a given pool*/
void getPoolID(Pool& pool, Phase currPhase, std::bitset<INFO_KEY_SIZE>& infoKey);

/*Retrieve the id for a given hand*/
HandID getHandID(Hand& currHand);

/*Print the string representation of the pool*/
void printPool(const Pool& pool, int numCards);

/*Print the string representation for a hand*/
void printHand(const Hand& hand);

inline int numCardsInPhase(Phase phase)
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
