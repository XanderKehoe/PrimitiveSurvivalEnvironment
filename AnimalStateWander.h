#pragma once
#include "AnimalStateBase.h"

class AnimalStateWander: public AnimalStateBase
{
public:
	AnimalStateWander();
	~AnimalStateWander();

	int wanderPathX = 0;
	int wanderPathY = 0;

	void EnterState(AnimalStateManager* manager) override;
	void Update(AnimalStateManager* manager) override;

	void FollowWanderPath(AnimalStateManager* manager);
	void GenerateWanderPath();
};

