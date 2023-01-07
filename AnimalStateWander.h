#pragma once
#include "AnimalStateBase.h"

class AnimalStateWander: public AnimalStateBase
{
public:
	AnimalStateWander();
	~AnimalStateWander();

	short wanderPathX = 0;
	short wanderPathY = 0;

	unsigned short currentIdle = 0;
	unsigned const short MAX_IDLE = 5;

	void EnterState(AnimalStateManager* manager) override;
	void Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent) override;

	void FollowWanderPath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
	void GenerateWanderPath();
};

