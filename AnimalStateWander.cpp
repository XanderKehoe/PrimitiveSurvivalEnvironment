#include "AnimalStateWander.h"
#include <random>

AnimalStateWander::AnimalStateWander()
{
}

AnimalStateWander::~AnimalStateWander()
{
}

void AnimalStateWander::EnterState(AnimalStateManager* manager, Entity* target)
{
	GenerateWanderPath();
}

void AnimalStateWander::Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	FollowWanderPath(manager, level);

	bool seeHuman = false; // PLACEHOLDER
	if (seeHuman)
	{
		if (manager->animal->IsHostile())
		{
			manager->ChangeState(manager->attackState);
			// figure out best way to pass target info here
		}
		else 
		{
			manager->ChangeState(manager->fleeState);
			// figure out best way to pass target info here
		}
	}
}

void AnimalStateWander::FollowWanderPath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	if (wanderPathX == 0 && wanderPathY == 0) // Reached 'destination', generate a new path to follow
	{
		GenerateWanderPath();
	}
	else if (wanderPathX == 0) // only need to move in y direction
	{
		if (wanderPathY < 0) 
		{
			manager->animal->Move(DirectionType::DOWN, false, level);
			wanderPathY++;
		}
		else 
		{
			manager->animal->Move(DirectionType::UP, false, level);
			wanderPathY--;
		}
	}
	else if (wanderPathY == 0) // only need to move in x direction
	{
		if (wanderPathX < 0)
		{
			manager->animal->Move(DirectionType::RIGHT, false, level);
			wanderPathX++;
		}
		else
		{
			manager->animal->Move(DirectionType::LEFT, false, level);
			wanderPathX--;
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
