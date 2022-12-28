#pragma once
#include "HumanAgentBase.h"

class Tile : public GameObject
{
public:
	Tile(TileType tileType, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize);
	~Tile();

	Entity *containedEntity = nullptr;

	bool HumanInteractionAvailable(); // is this tile interactable by humans?
	bool HumanInteract(class HumanAgentBase* humanAgent); // attempts human interaction of this tile, returns true for success, returns false otherwise. // forward declaration required here

	void Update() override;

	TileType tileResourceType;
	bool available = true;
	unsigned short currentResourceCount = 5;
	const unsigned short STARTING_RESOURCE_COUNT = 5;
	unsigned int currentRespawnTime = 0;
	const unsigned int RESOURCE_RESPAWN_TIME = 600;
};



