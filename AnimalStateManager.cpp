#include "AnimalStateManager.h"

#include "AnimalStateWander.h"
#include "AnimalStateAttack.h"

AnimalStateManager::AnimalStateManager(Animal* animal)
{
	wanderState = new AnimalStateWander();
	//fleeState = new AnimalStateFlee();
	attackState = new AnimalStateAttack();

	this->animal = animal;

	currentState = wanderState;

	currentState->EnterState(this);
}

AnimalStateManager::~AnimalStateManager()
{
}

void AnimalStateManager::Update()
{
	currentState->Update(this);
}

void AnimalStateManager::ChangeState(AnimalStateBase* state)
{
	currentState = state;
	state->EnterState(this);
}
