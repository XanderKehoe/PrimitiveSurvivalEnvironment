#pragma once
#include "AnimalStateManager.h"
#include "AnimalEventType.h"

class AnimalStateBase
{
public:
	virtual void EnterState() = 0;
	virtual void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
	virtual void PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
protected:
	const float HEALTH_FLEE_THRESHOLD = 0.3F; // determines the health percentage that this (hostile) animal should start fleeing at

	bool CanSeeHuman(AnimalStateManager* manager, HumanAgentBase* humanAgent, bool alreadySeen);
};

