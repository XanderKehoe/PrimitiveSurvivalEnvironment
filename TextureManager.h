#pragma once
#include "Game.h"
#include "TextureLoadTypes.h"

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);
	static SDL_Texture* LoadTextureByType(TextureLoadType textureLoadType, SDL_Renderer* ren);
};