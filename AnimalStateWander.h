#pragma once
#include "AnimalStateBase.h"

class AnimalStateWander: public AnimalStateBase
{
public:
	AnimalStateWander(AnimalStateManager* manager, HumanAgentBase* humanAgent);
	~AnimalStateWander();

	short wanderPathX = 0;
	short wanderPathY = 0;

	unsigned short currentIdle = 0;
	unsigned const short MAX_IDLE = 5;

	void EnterState() override;
	void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
	void PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;

private:
	AnimalStateManager* manager;
	HumanAgentBase* humanAgent;

	void HandleSeeHuman(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent);
	void FollowWanderPath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
	void GenerateWanderPath();
};

