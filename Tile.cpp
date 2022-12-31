#include "Tile.h"

Tile::Tile(TileType tileType, SDL_Renderer* ren, int initXPos, int initYPos): GameObject(ren, initXPos, initYPos)
{
	switch (tileType)
	{
		case TileType::PLAIN: 
		{
			objTexture = TextureManager::LoadTextureByType(LoadTextureType::TILE_PLAIN, ren);
			break;
		}
		case TileType::BUSH_BERRY:
		{
			objTexture = TextureManager::LoadTextureByType(LoadTextureType::TILE_BUSH_BERRY, ren);
			break;
		}
		case TileType::BUSH_FIBER:
		{
			objTexture = TextureManager::LoadTextureByType(LoadTextureType::TILE_BUSH_FIBER, ren);
			break;
		}
		case TileType::ROCK:
		{
			objTexture = TextureManager::LoadTextureByType(LoadTextureType::TILE_ROCK, ren);
			break;
		}
		case TileType::TREE:
		{
			objTexture = TextureManager::LoadTextureByType(LoadTextureType::TILE_TREE, ren);
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


