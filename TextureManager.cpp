#include "TextureManager.h"
#include "Game.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer* ren) 
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

SDL_Texture* TextureManager::LoadTextureByType(LoadTextureType loadTextureType, SDL_Renderer* ren)
{
	switch (loadTextureType) 
	{
		case LoadTextureType::TILE_PLAIN: 
		{
			return LoadTexture("Textures/GrassTilePlain.png", ren);
		}
		case LoadTextureType::TILE_BUSH_BERRY:
		{
			return LoadTexture("Textures/GrassTileWithBushBerry.png", ren);
		}
		case LoadTextureType::TILE_BUSH_FIBER:
		{
			return LoadTexture("Textures/GrassTileWithBushFiber.png", ren);
		}
		case LoadTextureType::TILE_ROCK:
		{
			return LoadTexture("Textures/GrassTileWithRock.png", ren);
		}
		case LoadTextureType::TILE_TREE:
		{
			return LoadTexture("Textures/GrassTileWithTree.png", ren);
		}

		default:
			throw std::logic_error("Not Implemented");
	}
}