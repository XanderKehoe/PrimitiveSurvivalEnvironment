#include "AnimalStateDead.h"

AnimalStateDead::AnimalStateDead()
{
}

AnimalStateDead::~AnimalStateDead()
{
}

void AnimalStateDead::EnterState(AnimalStateManager* manager)
{
	timeTillRespawn = DEATH_TIME_MAX;
}

void AnimalStateDead::Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent)
{
	if (timeTillRespawn == 0 && abs(humanAgent->GetGridXPos() - manager->animal->GetGridXPos()) > 20 && abs(humanAgent->GetGridYPos() - manager->animal->GetGridYPos()))
	{
		manager->animal->Respawn();
		manager->ChangeState(manager->wanderState);
	}
	else 
	{
		if (timeTillRespawn > 0)
			timeTillRespawn--;
	}
}