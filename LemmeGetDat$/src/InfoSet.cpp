#include "InfoSet.h"
#include "Table.h"

int InfoSet::getInfoId(const Table& table) const
{

	return 0;
}

int InfoSet::getActionSeqId(const ActionClass*, const int numActions) const
{

	return 0;
}

InfoSet::InfoSet() : regrets(new Regret*[MAX_ID]), generator()
{
	for (int i = 0; i < MAX_ID; i++) 
	{
		regrets[i] = nullptr;
	}
}
	
InfoSet::~InfoSet() 
{
	for (int i = 0; i < MAX_ID; i++)
	{
		if (regrets[i] != nullptr)
		{
			delete[] regrets[i];
		}
	}
	delete []regrets;
}

Regret* InfoSet::getRegrets(const Table& table, const ActionClass* actSeq, int numActions)
{
	int id = getInfoId(table) + getActionSeqId(actSeq, numActions) * MAX_INFO_ID;

	if (regrets[id] == nullptr) {
		int newListSize = (int)ActionClass::NUM_ACTIONS_POST_FLOP;
		regrets[id] = new Regret[newListSize];
		for (int i = 0; i < newListSize; i++)
		{
			regrets[id][i] = 0;
		}
	}

	return regrets[id];
}

void printAction(ActionClass action)
{
	switch(action)
	{
	case ActionClass::FOLD:
		std::cout << "folds";
		break;
	case ActionClass::CALL:
		std::cout << "calls";
		break;
	case ActionClass::ALL_IN:
		std::cout << "goes all in";
		break;
	case ActionClass::RAISE_HALF:
		std::cout << "raises half pot";
		break;
	case ActionClass::RAISE_POT:
		std::cout << "raises pot";
		break;
	case ActionClass::OPEN:
		std::cout << "open raises";
		break;
	case ActionClass::BET3:
		std::cout << "3 bets";
		break;
	case ActionClass::BET4:
		std::cout << "4 bets";
		break;
	default:
		std::cout << "Other action: " << (int)action;
		break;
	}
	std::cout << std::endl;
}
