#include "AnimalStateManager.h"

#include "AnimalStateWander.h"
#include "AnimalStateFlee.h"
#include "AnimalStateAttack.h"
#include "AnimalStateDead.h"

AnimalStateManager::AnimalStateManager(Animal* animal)
{
	wanderState = new AnimalStateWander();
	fleeState = new AnimalStateFlee();
	attackState = new AnimalStateAttack();
	deadState = new AnimalStateDead();

	this->animal = animal;

	currentState = wanderState;

	currentState->EnterState(this);
}

AnimalStateManager::~AnimalStateManager()
{
}

void AnimalStateManager::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent)
{
	currentState->Update(this, level, humanAgent);
}

void AnimalStateManager::ChangeState(AnimalStateBase* state)
{
	currentState = state;

	state->EnterState(this);
}
