#include "TextureManager.h"
#include "Game.h"

// forward declaration (resolves linker error)
SDL_Texture* TextureManager::tilePlain;
SDL_Texture* TextureManager::tileBushBerry;
SDL_Texture* TextureManager::tileBushFiber;
SDL_Texture* TextureManager::tileRock;
SDL_Texture* TextureManager::tileRockDemolished;
SDL_Texture* TextureManager::tileTree;
SDL_Texture* TextureManager::tileTreeStump;
SDL_Texture* TextureManager::entityHuman;

void TextureManager::InitializeTextures(SDL_Renderer* ren)
{
	tilePlain = LoadTexture("Textures/GrassTilePlain.png", ren);
	tileBushBerry = LoadTexture("Textures/GrassTileWithBushBerry.png", ren);
	tileBushFiber = LoadTexture("Textures/GrassTileWithBushFiber.png", ren);
	tileRock = LoadTexture("Textures/GrassTileWithRock.png", ren);
	tileRockDemolished = LoadTexture("Textures/TempCliffTile.png", ren);
	tileTree = LoadTexture("Textures/GrassTileWithTree.png", ren);
	tileTreeStump = LoadTexture("Textures/GrassTileWithTreeStump.png", ren);
	entityHuman = LoadTexture("Textures/HumanAgent.png", ren);
}

SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer* ren)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

SDL_Texture* TextureManager::LoadTextureByType(TextureLoadType textureLoadType)
{
	switch (textureLoadType) 
	{
		case TextureLoadType::TILE_PLAIN:
		{
			return tilePlain;
		}
		case TextureLoadType::TILE_BUSH_BERRY:
		{
			return tileBushBerry;
		}
		case TextureLoadType::TILE_BUSH_FIBER:
		{
			return tileBushFiber;
		}
		case TextureLoadType::TILE_ROCK:
		{
			return tileRock;
		}
		case TextureLoadType::TILE_ROCK_DEMOLISHED:
		{
			return tileRockDemolished;
		}
		case TextureLoadType::TILE_TREE:
		{
			return tileTree;
		}
		case TextureLoadType::TILE_TREE_STUMP:
		{
			return tileTreeStump;
		}
		case TextureLoadType::ENTITY_HUMAN: 
		{
			return entityHuman;
		}

		default:
			throw std::logic_error("Not Implemented");
	}
}