#pragma once
#include <vector>
#include "Entity.h"
#include "ActionType.h"
#include "Inventory.h"

class HumanAgentBase : public Entity
{
public:
	HumanAgentBase(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~HumanAgentBase();

	float Update(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], ActionType actionType); // returns the reward gained in this step
	virtual std::vector<int> GetObservations(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
	bool TakeAction(ActionType action, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]); // returns true if action was successfully taken, false otherwise.

	Inventory* GetInventory() { return inventory; }
private:
	unsigned short hunger = 100; // the health of the human agent, entity dies if this reaches 0.
	unsigned short thirst = 100; // the health of the human agent, entity dies if this reaches 0.
	unsigned short weariness = 100; // the health of the human agent, entity is forced asleep if this reaches 0.

	Inventory* inventory;

	bool Interact(DirectionType directionType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);

	bool printedObservations = false; // DELETE ME

	float reward = 0;
};

