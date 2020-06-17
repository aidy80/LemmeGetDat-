#pragma once

#include "Random.h"
#include "pch.h"

class Table;

typedef int32_t Regret;

enum class ActionClass
{
	FOLD, CALL, ALL_IN, RAISE_QUARTER, RAISE_HALF, RAISE_THREE_QUARTERS, RAISE_POT, NUM_ACTIONS_POST_FLOP, OPEN, BET3, BET4, NUM_ACTIONS_PRE_FLOP, NULL_ACTION
};

void printAction(ActionClass action);

class InfoSet
{
private:
	static const int MAX_INFO_ID = 3;
	static const int MAX_ID = 3;

	Regret** regrets;
	Random generator;


public:
	InfoSet();
	~InfoSet();

	InfoSet(const InfoSet&) = delete;
	InfoSet& operator=(const InfoSet&) = delete;

	Regret* getRegrets(const Table& table, const ActionClass*, int numActions);

	ActionClass makeDecision(Regret* probDist);

	int getInfoId(const Table& table) const;

	int getActionSeqId(const ActionClass*, const int numActions) const;
};
