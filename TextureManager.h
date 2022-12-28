#pragma once
#include "Game.h"

enum class LoadTextureType { TILE_PLAIN, TILE_BUSH_BERRY, TILE_BUSH_FIBER, TILE_ROCK, TILE_TREE };

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);
	static SDL_Texture* LoadTextureByType(LoadTextureType loadTextureType, SDL_Renderer* ren);
};