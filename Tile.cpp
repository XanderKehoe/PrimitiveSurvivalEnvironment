#include "Tile.h"

Tile::Tile(TileType tileType, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize): GameObject(tileType, ren, initXPos, initYPos, tileSize)
{
	switch (tileType)
	{
		case TileType::PLAIN: 
		{
			objTexture = TextureManager::LoadTextureByType(LoadTextureType::PLAIN_TILE, ren);
			break;
		}

		default:
			throw std::logic_error("Not Implemented");
	}
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

void Tile::Render()
{
	destRect.x = xpos;
	destRect.y = ypos;
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}


