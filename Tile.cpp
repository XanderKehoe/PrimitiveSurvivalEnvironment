#include "Tile.h"
#include "Camera.h"

Tile::Tile(TileType tileType, SDL_Renderer* ren, int gridXPos, int gridYPos)
{
	switch (tileType)
	{
		case TileType::PLAIN: 
		{
			tileTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_PLAIN);
			break;
		}
		case TileType::BUSH_BERRY:
		{
			tileTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_BUSH_BERRY);
			tileDepletedTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_PLAIN);
			break;
		}
		case TileType::BUSH_FIBER:
		{
			tileTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_BUSH_FIBER);
			tileDepletedTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_PLAIN);
			break;
		}
		case TileType::ROCK:
		{
			tileTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_ROCK);
			tileDepletedTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_ROCK_DEMOLISHED);
			break;
		}
		case TileType::TREE:
		{
			tileTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_TREE);
			tileDepletedTexture = TextureManager::LoadTextureByType(TextureLoadType::TILE_TREE_STUMP);
			break;
		}

		default:
			throw std::logic_error("Not Implemented");
	}

	this->tileType = tileType;

	this->gridXPos = gridXPos;
	this->gridYPos = gridYPos;

	srcRect.h = 256;
	srcRect.w = 256;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = Config::TILE_SIZE;
	destRect.w = Config::TILE_SIZE;

	renderer = ren;
}

bool Tile::HumanInteractionAvailable()
{
	if (available)
		return true;
	else
		return false;
}

bool Tile::HumanInteract(class HumanAgentBase* humanAgent)
{
	if (available)
	{
		Inventory* inventory = humanAgent->GetInventory();

		bool hasTool = false;
		switch (GetResourceType()) 
		{
			case ItemType::ROCK: 
			{
				if (inventory->GetItemTypeAmount(ItemType::HAMMER) > 0) 
				{
					hasTool = true;
				}
				break;
			}
			case ItemType::WOOD:
			{
				if (inventory->GetItemTypeAmount(ItemType::AXE) > 0)
				{
					hasTool = true;
				}
				break;
			}
		}
		
		// Roll dice to determine if human agent should receive resource type, or use tool if available.
		int leftover = 0;
		if (hasTool || (float) rand() / RAND_MAX < 0.2)
		{
			currentResourceCount--;
			leftover = inventory->AddItemToInventory(GetResourceType(), 1);
			//printf("resourceType: %d currentResourceCount: %d leftover: %d\n", GetResourceType(), currentResourceCount, leftover);
		}

		if (currentResourceCount == 0)
		{
			available = false;
			currentResourceCount = STARTING_RESOURCE_COUNT;
			currentRespawnTime = RESOURCE_RESPAWN_TIME;
		}

		if (leftover == 0)
			return true;
		else
			return false;
	}
	else
		return false;
}

bool Tile::IsWalkable(bool isHuman)
{
	if ((tileType == TileType::BUSH_BERRY ||
		tileType == TileType::BUSH_FIBER ||
		tileType == TileType::PLAIN ||
		(tileType == TileType::DOOR && isHuman)))
		//&& attachedEntity == nullptr)
	{
		return true;
	}
	else
		return false;
}

void Tile::Update() 
{
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
	const bool PATH_FIND_DEBUG = false;
	const bool FLEE_PATH_DEBUG = false;

	destRect.h = Config::TILE_SIZE / Camera::zoom;
	destRect.w = Config::TILE_SIZE / Camera::zoom;
	destRect.x = ((gridXPos * Config::TILE_SIZE) / Camera::zoom) - Camera::xPos;
	destRect.y = ((gridYPos * Config::TILE_SIZE) / Camera::zoom) - Camera::yPos;

	if (visited && PATH_FIND_DEBUG) 
	{
		switch (cameFrom) 
		{
			case DirectionType::UP:
				SDL_RenderCopy(renderer, TextureManager::LoadTextureByType(TextureLoadType::DEBUG_UP), &srcRect, &destRect);
				break;
			case DirectionType::DOWN:
				SDL_RenderCopy(renderer, TextureManager::LoadTextureByType(TextureLoadType::DEBUG_DOWN), &srcRect, &destRect);
				break;
			case DirectionType::LEFT:
				SDL_RenderCopy(renderer, TextureManager::LoadTextureByType(TextureLoadType::DEBUG_LEFT), &srcRect, &destRect);
				break;
			case DirectionType::RIGHT:
				SDL_RenderCopy(renderer, TextureManager::LoadTextureByType(TextureLoadType::DEBUG_RIGHT), &srcRect, &destRect);
				break;
		}
		
		return;
	}
	else if (FLEE_PATH_DEBUG && debug) 
	{
		SDL_RenderCopy(renderer, TextureManager::LoadTextureByType(TextureLoadType::DEBUG), &srcRect, &destRect);
		return;
	}

	if (!available && GetResourceType() != ItemType::NONE)
		SDL_RenderCopy(renderer, tileDepletedTexture, &srcRect, &destRect);
	else
		SDL_RenderCopy(renderer, tileTexture, &srcRect, &destRect);
}

ItemType Tile::GetResourceType()
{
	switch (tileType)
	{
		case TileType::BUSH_BERRY:
		{
			return ItemType::BERRY;
		}
		case TileType::BUSH_FIBER:
		{
			return ItemType::FIBER;
		}
		case TileType::ROCK:
		{
			return ItemType::ROCK;
		}
		case TileType::TREE:
		{
			return ItemType::WOOD;
		}

		default: 
		{
			return ItemType::NONE;
		}
	}
}


