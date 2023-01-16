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
	bool Move(DirectionType directionType, bool isHuman, class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
	bool CanMove() override;
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
	bool movedWhileSneaking = false;
	unsigned short startVisibilityCount = 20; // Agent has a X turn headstart where they are invisible to hostile animals, turns to 0 if agent attacks an animal.
	unsigned const short START_VISIBILITY = 40;
	unsigned const short BOW_COOLDOWN_MAX = 4;
	unsigned short currentBowCooldown = 0;
	bool hearAnimal = false;
	bool hearHostileAnimal = false;
	bool hearPassiveAnimal = false;

	// tracks where the agent start in this episode.
	int startGridX;
	int startGridY;
private:
	

	float reward = 0;
	bool done = false;

	unsigned const short GENERAL_HEARING_RANGE = 20; // actual range is sqrt(val)
	unsigned const short SPECIFIC_HEARING_RANGE = 15;  // actual range is sqrt(val)
	const float BASE_HUNGER_DRAIN = 0.0625F;
	const float MOVE_HUNGER_DRAIN = 0.09375F;
	const float STARVATION_DAMAGE_AMOUNT = 2.5F;
	
	unsigned int step = 0;
	static const int MAX_STEPS = 20000;

	unsigned const short BOW_MAX_RANGE = 4;
	const float BOW_DAMAGE = 15.0F;

	float const MEAT_HUNGER_RESTORE_AMOUNT = 50.0F;
	float const BERRY_HUNGER_RESTORE_AMOUNT = 2.5F;
	
	bool Interact(DirectionType directionType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);

	void ReduceHunger(float amount);
	void CheckHearing(std::list<Animal*> animalList);

	void ToggleSneaking();
	bool ShootBow(std::list<class Animal*> animalList, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
};

