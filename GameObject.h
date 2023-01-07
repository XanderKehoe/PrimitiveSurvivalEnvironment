#pragma once
#include "SDL.h"
#include "TextureManager.h"
#include "Game.h"

class GameObject
{
public:
	GameObject(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	GameObject(SDL_Texture* texture, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	GameObject(SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	void ConstructorHelper(SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~GameObject();

	virtual void Update(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) = 0;
	virtual void Render();

	int GetGridXPos() { return gridXPos; }
	int GetGridYPos() { return gridYPos; }
protected:
	int gridXPos = 0;
	int gridYPos = 0;

	SDL_Rect destRect;
	SDL_Rect srcRect;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* objTexture = nullptr;
};
