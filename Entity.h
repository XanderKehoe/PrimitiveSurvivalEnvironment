#pragma once

#include "GameObject.h"
#include "DirectionType.h"
#include "Inventory.h"

class Entity : public GameObject
{
public:
	Entity(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~Entity();

	virtual void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
	virtual void Render();

	/*
	Attempts to move the character by x/y respectively, accounts for movement cooldown and 'blocking' (tiles that entities can't enter) tile types.
	*/
	virtual bool Move(DirectionType directionType, bool isHuman, class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
	virtual bool TakeDamage(float amount);
	bool CanMove() { return moveCurrentCooldown == 0; }
	Inventory* GetInventory() { return inventory; }
	Tile* GetAttachedTile() { return attachedTile; }
	void Respawn();
protected:
	float health; // the health of the entity, entity dies if this reaches 0.
	float MAX_HEALTH; // the max health of the entity, health will slowly regenerate over time up to a max of this value.
	float attackDamage; // how much damage this entity can deal to another entity

	bool isDead = false;

	Inventory* inventory;

	unsigned short moveTimerMax; // how long entity has to wait to move again.
	unsigned short moveCurrentCooldown = 0; // if 0, entity can move

	static bool GridPosOutOfBounds(int gridX, int gridY);
private:
	static const bool OVERRIDE_MOVE_COOLDOWN = true;
	Tile* attachedTile = nullptr;
};
