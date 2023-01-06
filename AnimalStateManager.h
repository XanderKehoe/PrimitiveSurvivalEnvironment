#pragma once
#include "Animal.h"

class AnimalStateManager
{
public:
	AnimalStateManager(Animal* animal);
	~AnimalStateManager();

	Animal* animal;
	class AnimalStateBase* currentState;
	
	class AnimalStateBase* wanderState;
	//class AnimalStateBase* fleeState;
	class AnimalStateBase* attackState;

	void Update();
	void ChangeState(class AnimalStateBase* state);
};

