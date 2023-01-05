#pragma once
#include "HumanAgentBase.h"
#include "Config.h"
#include "TileType.h"

class Tile
{
public:
	Tile(TileType tileType, SDL_Renderer* ren, int gridXPos, int gridYPos);
	~Tile();

	Entity* attachedEntity = nullptr;

	TileType tileType; // the type of this tile
	bool available = true; // determines if this tile available to interact with, based on tile type.
	unsigned short currentResourceCount = 5; // the number of resources this tile currently has, only applicable for TREE, ROCK, and BUSH types.
	const unsigned short STARTING_RESOURCE_COUNT = 5; // the number of resources this tile starts with, only applicable for TREE, ROCK, and BUSH types.
	unsigned int currentRespawnTime = 0; // When unavailable, this variable counts down to 0 and when it does, this tile becomes available for interaction again.
	const unsigned int RESOURCE_RESPAWN_TIME = 600; // The value currentRespawnTime is set to when the available bool is set from true -> false.

	bool HumanInteractionAvailable(); // is this tile interactable by humans?
	bool HumanInteract(class HumanAgentBase* humanAgent); // attempts human interaction of this tile, returns true for success, returns false otherwise. // forward declaration required here

	void Update();
	void Render();

	ItemType GetResourceType();
private:
	SDL_Texture* tileTexture;
	SDL_Texture* tileDepletedTexture = nullptr;

	int gridXPos;
	int gridYPos;

	SDL_Rect destRect;
	SDL_Rect srcRect;

	SDL_Renderer* renderer = nullptr;
};



