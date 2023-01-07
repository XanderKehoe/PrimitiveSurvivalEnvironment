#pragma once
#include "Entity.h"
#include "AnimalType.h"
#include "AnimalStateManager.h"
#include "HumanAgentBase.h"

class Animal : public Entity
{
public:
	Animal(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~Animal();

	AnimalType animalType;

	void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent);
	void Render() override;

	bool TakeDamage(float amount) override;
	bool IsHostile() { return isHostile; }

	unsigned short GetSightRange() { return sightRange; }

	bool IsWithinSightRange(int modifiedSightRange, Entity* entity);
private:
	class AnimalStateManager* stateManager;

	bool isHostile;
	unsigned short sightRange;

	void FillInventory();
};

