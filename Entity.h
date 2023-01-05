#pragma once

#include "GameObject.h"
#include "DirectionType.h"

class Entity : public GameObject
{
public:
	Entity(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~Entity();

	virtual void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;

	/*
	Attempts to move the character by x/y respectively, accounts for movement cooldown and 'blocking' (tiles that entities can't enter) tile types.
	*/
	virtual bool Move(DirectionType directionType, bool isHuman, class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
	static bool GridPosOutOfBounds(int gridX, int gridY);

protected:
	unsigned short health = 100; // the health of the entity, entity dies if this reaches 0.
	unsigned const short MAX_HEALTH = 100; // the max health of the entity, health will slowly regenerate over time up to a max of this value.
	unsigned short moveTimerMax = 1; // how long entity has to wait to move again.
	unsigned short moveCurrentCooldown = 0; // if 0, entity can move

	//Tile* currentTile = nullptr;

	bool CanMove() { return moveCurrentCooldown == 0; }
private:
	static const bool OVERRIDE_MOVE_COOLDOWN = true;
};
