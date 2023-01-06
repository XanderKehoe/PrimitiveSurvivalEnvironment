#pragma once
#include "Animal.h"
#include "Tile.h"

class AnimalStateManager
{
public:
	AnimalStateManager(Animal* animal);
	~AnimalStateManager();

	Animal* animal;
	class AnimalStateBase* currentState;
	
	class AnimalStateBase* wanderState;
	class AnimalStateBase* fleeState;
	class AnimalStateBase* attackState;

	void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
	void ChangeState(class AnimalStateBase* state);
};

