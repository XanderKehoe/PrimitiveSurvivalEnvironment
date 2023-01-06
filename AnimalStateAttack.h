#pragma once

#include "AnimalStateBase.h"
class AnimalStateAttack : public AnimalStateBase
{
public:
	AnimalStateAttack();
	~AnimalStateAttack();

	void EnterState(AnimalStateManager* manager, Entity* target = nullptr) override;
	void Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
};
