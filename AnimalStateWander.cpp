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

}

void AnimalStateWander::Update(AnimalStateManager* manager)
{
	bool seeHuman = false; // PLACEHOLDER
	if (seeHuman)
	{
		if (manager->animal->IsHostile())
		{
			// manager->ChangeState(manager->attackState);
			// figure out best way to pass target info here
		}
		else {

		}
	}
}

void AnimalStateWander::FollowWanderPath(AnimalStateManager* manager)
{
	if (wanderPathX == 0 && wanderPathY == 0) 
	{
		GenerateWanderPath();
	}
	else if (wanderPathX != 0) 
	{
		if (wanderPathX < 0) 
		{

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
