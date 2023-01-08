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

	UpdateResult Update(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], ActionType actionType, std::list<class Animal*> animalList); // returns the reward gained and if episode finished in this step
	void Render() override;
	int GetEntityType() override;

	bool TakeDamage(float amount, bool fromBow = false, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE] = nullptr) override;
	void Respawn() override;
	virtual std::vector<float> GetObservationsFloat(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
	virtual std::vector<int> GetObservationsInt(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
	bool TakeAction(ActionType action, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], std::list<class Animal*> animalList); // returns true if action was successfully taken, false otherwise.

	Inventory* GetInventory() { return inventory; }
	bool IsSneaking() { return sneaking; }
	bool IsHidingInBush();
	bool IsOnStartVisibility() { return startVisibilityCount > 0; }
protected:
	float hunger = 100; // the health of the human agent, entity dies if this reaches 0.
	const float MAX_HUNGER = 100;
	float thirst = 100; // the health of the human agent, entity dies if this reaches 0.
	const float MAX_THIRST = 100;
	float weariness = 100; // the health of the human agent, entity is forced asleep if this reaches 0.
	const float MAX_WEARINESS = 100;
	bool sneaking = false;
	unsigned short startVisibilityCount = 20; // Agent has a X turn headstart where they are invisible to hostile animals, turns to 0 if agent attacks an animal.
	unsigned const short START_VISIBILITY = 20;
	unsigned const short BOW_COOLDOWN_MAX = 4;
	unsigned short currentBowCooldown = 0;

	// tracks where the agent start in this episode.
	int startGridX;
	int startGridY;
private:
	bool Interact(DirectionType directionType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);

	float reward = 0;
	bool done = false;

	unsigned int step = 0;
	static const int MAX_STEPS = 20000;

	unsigned const short BOW_MAX_RANGE = 4;
	const float BOW_DAMAGE = 15.0F;

	float const MEAT_HUNGER_RESTORE_AMOUNT = 50.0F;
	float const BERRY_HUNGER_RESTORE_AMOUNT = 2.5F;
	
	void ReduceHunger(float amount);

	void ToggleSneaking();
	bool ShootBow(std::list<class Animal*> animalList, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
};

