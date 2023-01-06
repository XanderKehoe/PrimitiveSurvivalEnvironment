#pragma once
#include "Entity.h"
#include "Inventory.h"
#include "AnimalType.h"

class Animal : public Entity
{
public:
	Animal(AnimalType animalType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~Animal();

	bool IsHostile() { return isHostile; }
private:
	Inventory inventory;

	bool isHostile = false;
	int attackDamage = 0;
};

