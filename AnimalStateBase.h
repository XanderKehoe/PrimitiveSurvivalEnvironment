#pragma once
#include "AnimalStateManager.h"

class AnimalStateBase
{
public:
	virtual void EnterState(class AnimalStateManager* manager) = 0;
	virtual void Update(class AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent) = 0;
};

