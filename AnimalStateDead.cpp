#include "AnimalStateDead.h"

AnimalStateDead::AnimalStateDead(AnimalStateManager* manager, HumanAgentBase* humanAgent)
{
	this->manager = manager;
	this->humanAgent = humanAgent;
}

AnimalStateDead::~AnimalStateDead()
{
}

void AnimalStateDead::EnterState()
{
	timeTillRespawn = DEATH_TIME_MAX;
	if (!manager->animal->IsDead())
		printf("WARNING, dead state entered but animal is not dead...\n");
}

void AnimalStateDead::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
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

void AnimalStateDead::PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{

}
