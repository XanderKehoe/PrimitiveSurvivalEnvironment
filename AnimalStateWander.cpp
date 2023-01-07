#include "AnimalStateWander.h"
#include <random>

AnimalStateWander::AnimalStateWander()
{
}

AnimalStateWander::~AnimalStateWander()
{
}

void AnimalStateWander::EnterState(AnimalStateManager* manager)
{
	GenerateWanderPath();
}

void AnimalStateWander::Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent)
{
	FollowWanderPath(manager, level);

	bool seeHuman = false;
	int modifiedSightRange = manager->animal->GetSightRange();
	if (!humanAgent->IsHidingInBush()) 
	{
		if (humanAgent->IsSneaking())
			modifiedSightRange /= 2;

		if (manager->animal->IsWithinSightRange(modifiedSightRange, humanAgent))
			seeHuman = true;
	}

	if (seeHuman)
	{
		printf("I see a human! %d %d %d\n", modifiedSightRange, humanAgent->GetGridXPos(), humanAgent->GetGridYPos());
		if (manager->animal->IsHostile())
		{
			// manager->ChangeState(manager->attackState);
		}
		else 
		{
			//manager->ChangeState(manager->fleeState);
		}
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
