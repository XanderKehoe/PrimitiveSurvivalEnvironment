#include "AnimalStateManager.h"

#include "AnimalStateWander.h"
#include "AnimalStateFlee.h"
#include "AnimalStateAttack.h"
#include "AnimalStateDead.h"

AnimalStateManager::AnimalStateManager(Animal* animal, HumanAgentBase* humanAgent)
{
	wanderState = new AnimalStateWander(this, humanAgent);
	fleeState = new AnimalStateFlee(this, humanAgent);
	attackState = new AnimalStateAttack(this, humanAgent);
	deadState = new AnimalStateDead(this, humanAgent);

	this->animal = animal;

	currentState = wanderState;

	currentState->EnterState();
}

AnimalStateManager::~AnimalStateManager()
{
}

void AnimalStateManager::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	currentState->Update(level);
}

void AnimalStateManager::ChangeState(AnimalStateBase* state)
{
	currentState = state;

	state->EnterState();
}

void AnimalStateManager::PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	currentState->PostEvent(eventType, level);
}
