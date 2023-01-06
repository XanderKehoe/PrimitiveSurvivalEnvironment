#include "AnimalStateManager.h"

#include "AnimalStateWander.h"
#include "AnimalStateFlee.h"
#include "AnimalStateAttack.h"

AnimalStateManager::AnimalStateManager(Animal* animal)
{
	wanderState = new AnimalStateWander();
	fleeState = new AnimalStateFlee();
	attackState = new AnimalStateAttack();

	this->animal = animal;

	currentState = wanderState;

	currentState->EnterState(this);
}

AnimalStateManager::~AnimalStateManager()
{
}

void AnimalStateManager::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	currentState->Update(this, level);
}

void AnimalStateManager::ChangeState(AnimalStateBase* state)
{
	currentState = state;
	state->EnterState(this);
}
