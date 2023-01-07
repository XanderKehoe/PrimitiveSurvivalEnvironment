#pragma once

#include "AnimalStateBase.h"
class AnimalStateFlee : public AnimalStateBase
{
public:
	AnimalStateFlee();
	~AnimalStateFlee();

	void EnterState(AnimalStateManager* manager) override;
	void Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent) override;
};
