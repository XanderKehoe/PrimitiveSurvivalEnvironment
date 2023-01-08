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
	virtual int GetEntityType() = 0;

	/*
	Attempts to move the character by x/y respectively, accounts for movement cooldown and 'blocking' (tiles that entities can't enter) tile types.
	*/
	virtual bool Move(DirectionType directionType, bool isHuman, class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]);
	bool CanMove() { return moveCurrentCooldown == 0; }

	virtual bool TakeDamage(float amount, bool fromBow = false, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE] = nullptr);
	virtual void Respawn();
	virtual bool IsDead() { return isDead; }

	Inventory* GetInventory() { return inventory; }
	float GetAttackDamage() { return attackDamage; }
	float GetCurrentHealth() { return health; }
	float GetMaxHealth() { return MAX_HEALTH; }
	Tile* GetAttachedTile() { return attachedTile; }
	
protected:
	float health; // the health of the entity, entity dies if this reaches 0.
	float MAX_HEALTH; // the max health of the entity, health will slowly regenerate over time up to a max of this value.
	float attackDamage; // how much damage this entity can deal to another entity

	

	Inventory* inventory;

	unsigned short moveTimerMax; // how long entity has to wait to move again.
	unsigned short moveCurrentCooldown = 0; // if 0, entity can move

	static bool GridPosOutOfBounds(int gridX, int gridY);
private:
	bool isDead = false;
	static const bool OVERRIDE_MOVE_COOLDOWN = false;
	Tile* attachedTile = nullptr;
};
