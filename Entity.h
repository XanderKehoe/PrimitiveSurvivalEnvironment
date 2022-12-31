#pragma once

#include "GameObject.h"

class Entity : public GameObject
{
public:
	Entity(const char* texturesheet, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~Entity();

	virtual void Update() override = 0;

protected:
	unsigned short health = 100; // the health of the entity, entity dies if this reaches 0.
	unsigned short moveTimerMax = 1; // how long entity has to wait to move again.
	unsigned short moveCurrentCooldown = 0; // if 0, entity can move

	//Tile* currentTile = nullptr;

	bool canMove() { return moveCurrentCooldown == 0; }
};
