#pragma once
#include "AnimalStateBase.h"

class AnimalStateDead : public AnimalStateBase
{
public:
	AnimalStateDead();
	~AnimalStateDead();

	void EnterState(AnimalStateManager* manager) override;
	void Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent) override;
private:
	unsigned const int DEATH_TIME_MAX = 600;
	unsigned int timeTillRespawn = 0;
};

