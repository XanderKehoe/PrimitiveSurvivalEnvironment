#pragma once
#include "AnimalStateBase.h"

class AnimalStateDead : public AnimalStateBase
{
public:
	AnimalStateDead(AnimalStateManager* manager, HumanAgentBase* humanAgent);
	~AnimalStateDead();

	void EnterState() override;
	void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
	void PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
private:
	unsigned const int DEATH_TIME_MAX = 600;
	unsigned int timeTillRespawn = 0;

	AnimalStateManager* manager;
	HumanAgentBase* humanAgent;
};

