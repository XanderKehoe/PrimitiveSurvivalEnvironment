#include "AnimalStateFlee.h"

AnimalStateFlee::AnimalStateFlee()
{
}

AnimalStateFlee::~AnimalStateFlee()
{
}

void AnimalStateFlee::EnterState(AnimalStateManager* manager, Entity* target)
{
	if (target == nullptr) 
	{
		throw std::logic_error("Target cannot be null for AnimalStateFlee::EnterState");
	}
}

void AnimalStateFlee::Update(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{

}

