#pragma once

#include "AnimalStateBase.h"
class AnimalStateFlee : public AnimalStateBase
{
public:
	AnimalStateFlee();
	~AnimalStateFlee();

	void EnterState(AnimalStateManager* manager, Entity* target = nullptr) override;
	void Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
};
