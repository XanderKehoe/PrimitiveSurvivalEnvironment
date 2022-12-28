#pragma once

class Entity;
class GameObject;

#include "GameObject.h"
//#include "Tile.h"


class Entity : public GameObject
{
public:
	Entity(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos);
	~Entity();

	virtual void Update() override = 0;

protected:
	unsigned short health = 100;
	unsigned short moveTimerMax = 1; // how long entity has to wait to move again.
	unsigned short moveCurrentCooldown = 0; // if 0, entity can move

	//Tile* currentTile = nullptr;

	bool canMove() { return moveCurrentCooldown == 0; }
	void move();
};
