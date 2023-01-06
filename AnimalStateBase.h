#pragma once
#include "AnimalStateManager.h"

class AnimalStateBase
{
public:
	virtual void EnterState(class AnimalStateManager* manager, Entity* target = nullptr) = 0;
	virtual void Update(class AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
};

