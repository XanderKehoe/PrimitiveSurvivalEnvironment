#include "AnimalStateWander.h"
#include <random>
#include "PathFinder.h"

AnimalStateWander::AnimalStateWander(AnimalStateManager* manager, HumanAgentBase* humanAgent)
{
	this->manager = manager;
	this->humanAgent = humanAgent;
}

AnimalStateWander::~AnimalStateWander()
{
}

void AnimalStateWander::EnterState()
{
	GenerateWanderPath();
}

void AnimalStateWander::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	FollowWanderPath(manager, level);

	if (CanSeeHuman(manager, humanAgent, false))
	{
		HandleSeeHuman(manager, level, humanAgent);
	}
}

void AnimalStateWander::PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	switch (eventType) 
	{
		case AnimalEventType::TAKE_DAMAGE: 
		{
			if (CanSeeHuman(manager, humanAgent, false)) 
			{
				HandleSeeHuman(manager, level, humanAgent);
			}
			else 
			{
				manager->ChangeState(manager->fleeState);
			}
			break;
		}
	}
}

void AnimalStateWander::HandleSeeHuman(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent)
{
	if (manager->animal->IsHostile() && manager->animal->GetCurrentHealth() / manager->animal->GetMaxHealth() > HEALTH_FLEE_THRESHOLD)
	{
		std::vector<DirectionType> targetPath = PathFinder::GetPath(level, manager->animal->GetGridXPos(), manager->animal->GetGridYPos(), humanAgent->GetGridXPos(), humanAgent->GetGridYPos());
		if (!targetPath.empty())
		{
			manager->ChangeState(manager->attackState);
		}
		//else
			//printf("Tried changing to attack state but no path...\n");
	}
	else
	{
		manager->ChangeState(manager->fleeState);
	}
}

void AnimalStateWander::FollowWanderPath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	if (wanderPathX == 0 && wanderPathY == 0) // Reached 'destination', generate a new path to follow
	{
		if (currentIdle == 0) 
		{
			GenerateWanderPath();
			currentIdle = MAX_IDLE;
		}
		else
			currentIdle--;
	}

	float randVal = (float) rand() / RAND_MAX;

	if (manager->animal->CanMove())
	{
		if (wanderPathX == 0 || randVal < 0) // Move in Y dir
		{
			if (wanderPathY < 0)
			{
				bool success = manager->animal->Move(DirectionType::DOWN, false, level);
				if (!success)
					GenerateWanderPath();
				else
					wanderPathY++;
			}
			else
			{
				bool success = manager->animal->Move(DirectionType::UP, false, level);
				if (!success)
					GenerateWanderPath();
				else
					wanderPathY--;
			}
		}
		else // move in X dir
		{
			if (wanderPathX < 0)
			{
				bool success = manager->animal->Move(DirectionType::RIGHT, false, level);
				if (!success)
					GenerateWanderPath();
				else
					wanderPathX++;
			}
			else
			{
				bool success = manager->animal->Move(DirectionType::LEFT, false, level);
				if (!success)
					GenerateWanderPath();
				else
					wanderPathX--;
			}
		}
	}
}

void AnimalStateWander::GenerateWanderPath()
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(-10, 10); // define the range

	wanderPathX = distr(gen);
	wanderPathY = distr(gen);
}
