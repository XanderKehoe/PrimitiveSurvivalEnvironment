#pragma once
#include <vector>
#include "Entity.h"
#include "ActionType.h"
#include "UpdateResult.h"

class HumanAgentBase : public Entity
{
public:
	HumanAgentBase(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~HumanAgentBase();

	UpdateResult Update(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], ActionType actionType); // returns the reward gained in this step
	void Render() override;
	bool TakeDamage(float amount) override;
	virtual std::vector<float> GetObservationsFloat(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
	virtual std::vector<int> GetObservationsInt(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
	bool TakeAction(ActionType action, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]); // returns true if action was successfully taken, false otherwise.

	Inventory* GetInventory() { return inventory; }
	bool IsSneaking() { return sneaking; }
	bool IsHidingInBush();
protected:
	float hunger = 100; // the health of the human agent, entity dies if this reaches 0.
	const float MAX_HUNGER = 100;
	float thirst = 100; // the health of the human agent, entity dies if this reaches 0.
	const float MAX_THIRST = 100;
	float weariness = 100; // the health of the human agent, entity is forced asleep if this reaches 0.
	const float MAX_WEARINESS = 100;
	bool sneaking = false;
	unsigned short startVisibilityCount = 20; // Agent has a X turn headstart where they are invisible to hostile animals, turns to 0 if agent attacks an animal.
private:
	bool Interact(DirectionType directionType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);

	float reward = 0;

	unsigned int step = 0;
	static const int MAX_STEPS = 5000;
};

