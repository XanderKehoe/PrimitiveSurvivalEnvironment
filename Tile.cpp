#include "Tile.h"

Tile::Tile(TileType tileType, SDL_Renderer* ren, int initXPos, int initYPos): GameObject(tileType, ren, initXPos, initYPos)
{

}

bool Tile::HumanInteractionAvailable()
{
	if (available)
		return true;

	return false;
}

bool Tile::HumanInteract(class HumanAgentBase* humanAgent)
{
	if (available)
	{
		// Determine if human agent has corresponding tool for resource type.
		// Roll dice to determine if human agent should receive resource type.
		if (currentResourceCount == 0)
		{
			available = false;
			currentResourceCount = STARTING_RESOURCE_COUNT;
			currentRespawnTime = RESOURCE_RESPAWN_TIME;
		}

		return true;
	}
	else
		return false;
}

void Tile::Update() {
	if (!available)
	{
		if (currentRespawnTime > 0)
			currentRespawnTime--;
		else
			available = true;
	}
}

void Tile::LoadTextures(SDL_Renderer* renderer)
{
	/*
	textureSheetBushBerry = TextureManager::LoadTexture("Textures/TempBushBerry.png", renderer);
	textureSheetBushFiber = TextureManager::LoadTexture("Textures/TempBushFiber.png", renderer);
	textureSheetRock = TextureManager::LoadTexture("Textures/TempGrassTileWithBoulder.png", renderer);
	textureSheetTree = TextureManager::LoadTexture("Textures/TempGrassTileWithTree.png", renderer);
	*/
}


