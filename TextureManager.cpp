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
		case LoadTextureType::PLAIN_TILE: 
		{
			return LoadTexture("Textures/TempGrassTile.png", ren);
		}

		default:
			throw std::logic_error("Not Implemented");
	}
}