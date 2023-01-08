#include "AnimalStateAttack.h"
#include "PathFinder.h"

AnimalStateAttack::AnimalStateAttack(AnimalStateManager* manager,  HumanAgentBase* humanAgent)
{
	this->manager = manager;
	this->humanAgent = humanAgent;
}

AnimalStateAttack::~AnimalStateAttack()
{
}

void AnimalStateAttack::EnterState()
{
	//printf("Entered attack state\n");
}

void AnimalStateAttack::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	bool debug = false;

	if (!CanSeeHuman(manager, humanAgent, true)) 
	{
		// lost sight of target, return to wandering
		if (debug)
			printf("Lost sight, returning to wander state\n");
		manager->ChangeState(manager->wanderState);
		return;
	}

	if (manager->animal->GetCurrentHealth() / manager->animal->GetMaxHealth() <= HEALTH_FLEE_THRESHOLD)
	{
		// lost too much health, flee.
		if (debug)
			printf("Lost too much health, entering flee state\n");
		manager->ChangeState(manager->fleeState);
		return;
	}

	if (humanAgent->IsOnStartVisibility())
	{
		// target has visibility shield, go back to wandering.
		if (debug)
			printf("Visibility block is active, returning to wandering state\n");
		manager->ChangeState(manager->wanderState);
		return;
	}

	if (targetGridXPos != humanAgent->GetGridXPos() || targetGridYPos != humanAgent->GetGridYPos())
	{
		if (debug)
			printf("Attack State: target moved, updating path\n");
		UpdatePath(manager, level, humanAgent);
	}

	if (!targetPath.empty() && manager->animal->CanMove()) 
	{
		if (debug)
			printf("\tAttack State: Have Path and can move\n");
		DirectionType nextDirection = targetPath.back();
		targetPath.pop_back();

		bool success = manager->animal->Move(nextDirection, false, level);
		if (!success) 
		{
			if (debug)
				printf("\t\tAttack State: Tried movement but was unsuccessful (target is in range?)\n");
			if (abs(manager->animal->GetGridXPos() - targetGridXPos) == 1 || abs(manager->animal->GetGridYPos() - targetGridYPos))
				humanAgent->TakeDamage(manager->animal->GetAttackDamage());
			UpdatePath(manager, level, humanAgent);
		}
		else 
		{
			if (debug)
				printf("\t\tAttack State: Moved\n");
		}
	}
	else if (targetPath.empty()) 
	{
		if (debug)
			printf("\tAttack State: targetPath is empty, updating path...\n");
		UpdatePath(manager, level, humanAgent);
	}
	else if (!manager->animal->CanMove())
	{
		if (debug)
			printf("\tAttack State: Move Cooldown is Active\n");
	}
}

void AnimalStateAttack::PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
}

void AnimalStateAttack::UpdatePath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent) 
{
	targetPath = PathFinder::GetPath(level, manager->animal->GetGridXPos(), manager->animal->GetGridYPos(), humanAgent->GetGridXPos(), humanAgent->GetGridYPos());

	targetGridXPos = humanAgent->GetGridXPos();
	targetGridYPos = humanAgent->GetGridYPos();

	if (targetPath.empty() || PathFinder::Dist(manager->animal->GetGridXPos(), manager->animal->GetGridYPos(), targetGridXPos, targetGridYPos) > manager->animal->GetSightRange() * 1.5)
	{
		// no longer able to reach target or target is 50% farther than sight distance, go back to wandering.
		// printf("Path is unreachable, returning to wander state\n");
		manager->ChangeState(manager->wanderState);
		return;
	}
}
