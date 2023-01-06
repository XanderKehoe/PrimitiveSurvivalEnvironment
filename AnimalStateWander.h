#pragma once
#include "AnimalStateBase.h"

class AnimalStateWander: public AnimalStateBase
{
public:
	AnimalStateWander();
	~AnimalStateWander();

	int wanderPathX = 0;
	int wanderPathY = 0;

	void EnterState(AnimalStateManager* manager, Entity* target = nullptr) override;
	void Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;

	void FollowWanderPath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
	void GenerateWanderPath();
};

