#pragma once

#include "AnimalStateBase.h"
class AnimalStateAttack : public AnimalStateBase
{
public:
	AnimalStateAttack();
	~AnimalStateAttack();

	void EnterState(AnimalStateManager* manager) override;
	void Update(AnimalStateManager* manager) override;
};
