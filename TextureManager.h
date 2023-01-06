#pragma once
#include "Game.h"
#include "TextureLoadTypes.h"

class TextureManager {
public:
	static void InitializeTextures(SDL_Renderer* ren); // Must be called before any other use of this class
	static SDL_Texture* LoadTextureByType(TextureLoadType textureLoadType);
private:
	static SDL_Texture* tilePlain;
	static SDL_Texture* tileBushBerry;
	static SDL_Texture* tileBushFiber;
	static SDL_Texture* tileRock;
	static SDL_Texture* tileRockDemolished;
	static SDL_Texture* tileTree;
	static SDL_Texture* tileTreeStump;
	static SDL_Texture* entityHuman;

	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);
};