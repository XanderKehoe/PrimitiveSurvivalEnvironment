#include "AnimalStateAttack.h"

AnimalStateAttack::AnimalStateAttack()
{
}

AnimalStateAttack::~AnimalStateAttack()
{
}

void AnimalStateAttack::EnterState(AnimalStateManager* manager, Entity* target)
{
	if (target == nullptr)
	{
		throw std::logic_error("Target cannot be null for AnimalStateAttack::EnterState");
	}
}

void AnimalStateAttack::Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{

}
