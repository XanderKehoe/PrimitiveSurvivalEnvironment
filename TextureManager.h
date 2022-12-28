#pragma once
#include "Game.h"

enum class LoadTextureType { PLAIN_TILE };

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);
	static SDL_Texture* LoadTextureByType(LoadTextureType loadTextureType, SDL_Renderer* ren);
};