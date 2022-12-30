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

	TileType tileType; // the type of this tile
	bool available = true; // determines if this tile available to interact with, based on tile type.
	unsigned short currentResourceCount = 5; // the number of resources this tile currently has, only applicable for TREE, ROCK, and BUSH types.
	const unsigned short STARTING_RESOURCE_COUNT = 5; // the number of resources this tile starts with, only applicable for TREE, ROCK, and BUSH types.
	unsigned int currentRespawnTime = 0; // When unavailable, this variable counts down to 0 and when it does, this tile becomes available for interaction again.
	const unsigned int RESOURCE_RESPAWN_TIME = 600; // The value currentRespawnTime is set to when the available bool is set from true -> false.
};



