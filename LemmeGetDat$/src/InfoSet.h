#pragma once

#include "Random.h"
#include "pch.h"

class Table;

typedef int32_t Regret;

enum class ActionClass
{
	FOLD, CALL, ALL_IN, RAISE_HALF, RAISE_POT, NUM_ACTIONS, NULL_ACTION
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
