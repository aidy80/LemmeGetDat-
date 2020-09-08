#pragma once

#include "pch.h"
#include "Random.h"
#include "PokerCards.h"
#include "Simp2DArray.h"
#include "CombMath.h"

typedef float Regret;

/*
enum class ActionClass
{
	FOLD, CALL, ALL_IN, RAISE_QUARTER, RAISE_HALF, RAISE_THREE_QUARTERS, RAISE_POT, NUM_ACTIONS_POST_FLOP, OPEN, BET3, BET4, NUM_ACTIONS_PRE_FLOP, NULL_ACTION
};
*/

enum class ActionClass
{
	NULL_ACTION, FOLD, CALL, ALL_IN, OPEN1, OPEN2, OPEN3, BET3, BET4, NUM_ACTIONS 
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
	NUM_HAND_COMBOS,
	GARBAGE_HAND
};

const int NUM_GARB_HANDS = 70;
const int NUM_REAL_HANDS = (int)HandID::NUM_HAND_COMBOS - NUM_GARB_HANDS;

const HandID GARBAGE_HANDS[NUM_GARB_HANDS] = {HandID::TWTHo, HandID::TWFOo, HandID::TWFIo, HandID::TWSIo, HandID::TWSEo, HandID::TWEIo, 
										HandID::TWNIo, HandID::TWTEo, HandID::TWJAo, HandID::TWQUo, HandID::TWKIo,
										HandID::THFOo, HandID::THFIo, HandID::THSIo, HandID::THSEo, HandID::THEIo,
										HandID::THNIo, HandID::THTEo, HandID::THJAo, HandID::THQUo, HandID::THKIo,
										HandID::FOFIo, HandID::FOSIo, HandID::FOSEo, HandID::FOEIo, HandID::FONIo,
										HandID::FOTEo, HandID::FOJAo, HandID::FOQUo, HandID::FOKIo, HandID::FISEo, 
										HandID::FIEIo, HandID::FINIo, HandID::FITEo, HandID::FIJAo, HandID::FIQUo, 
										HandID::SINIo, HandID::SITEo, HandID::SIJAo, HandID::SIQUo, HandID::TWFIs,
										HandID::TWSIs, HandID::TWSEs, HandID::TWEIs, HandID::TWNIs, HandID::TWTEs, 
										HandID::TWJAs, HandID::TWQUs, HandID::THEIs, HandID::THNIs, HandID::THTEs, 
										HandID::THJAs, HandID::FONIs, HandID::FOTEs,
										//Less Bad but still pretty garbage
										HandID::FISIo, HandID::FIKIo, HandID::SISEo, HandID::SIEIo, HandID::SIKIo,
										HandID::SENIo, HandID::SETEo, HandID::SEJAo, HandID::THSIs, HandID::THSEs,
										HandID::FOSEs, HandID::FOEIs, HandID::FINIs, HandID::FITEs, HandID::FOJAs,
										HandID::TWFOs};

void printAction(ActionClass action);

class InfoSet
{
public:
	static const int ACT_BITS = 3;
	static const int ACT_KEY_SIZE = 150;
	static const int CARD_KEY_SIZE = 20;
	static const int SUIT_BITS = 1;
	static const int INFO_KEY_SIZE = CARD_KEY_SIZE + ACT_KEY_SIZE;

	typedef std::bitset<InfoSet::INFO_KEY_SIZE> BitSet;
private:
	std::unordered_map<BitSet, Simp2DArray<Regret>, std::hash<BitSet>> regrets;
	std::unordered_set<HandID> garbageHands;

	void initRegret(const BitSet& infoKey, int numLegalActions);

public:
	InfoSet();
	~InfoSet();

	InfoSet(const InfoSet&) = delete;
	InfoSet& operator=(const InfoSet&) = delete;

	static void addAction(BitSet& infoKey, ActionClass act, int depth);

	static void removeAction(BitSet& infoKey, int depth);

	void updateHandDist(const BitSet& infoKey, int currTurn, std::vector<float>& oppoHandLikeli);

	void getStrat(const BitSet& infoKey, HandID hand, std::vector<float>& strat);

	void writeRegrets();

	/*Retrieve the id for a given pool and add it to the passed infoKey*/
	static void getPoolID(Pool& pool, Phase currPhase, BitSet& infoKey);

	/*Retrieve the id for a given hand*/
	static HandID getHandID(Hand& currHand);

	void getAllPoolID(std::vector<BitSet>& allPools);

	void getAllHandID(std::vector<HandID>& allHands);

	static void printHandID(HandID hand);

	static void printPoolID(const BitSet& pool);
	
	static void printKeyInfo(const BitSet& key);
};
