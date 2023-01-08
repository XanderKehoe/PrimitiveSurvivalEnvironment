#pragma once
#include "Animal.h"
#include "Tile.h"
#include "AnimalEventType.h"

class AnimalStateManager
{
public:
	AnimalStateManager(Animal* animal, HumanAgentBase* humanAgent);
	~AnimalStateManager();

	Animal* animal;
	class AnimalStateBase* currentState;
	
	class AnimalStateBase* wanderState;
	class AnimalStateBase* fleeState;
	class AnimalStateBase* attackState;
	class AnimalStateBase* deadState;

	void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
	void ChangeState(class AnimalStateBase* state);
	void PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
};

