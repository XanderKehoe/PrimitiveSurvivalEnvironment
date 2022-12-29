#pragma once
#include "SDL.h"
#include "TextureManager.h"

enum class TileType { PLAIN, CLIFF, WATER, BUSH_BERRY, BUSH_FIBER, TREE, ROCK, DOOR, WALL };

class GameObject
{
public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize);
	GameObject(SDL_Texture* texture, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize);
	GameObject(SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize);
	void ConstructorHelper(SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize);
	~GameObject();

	virtual void Update() = 0;
	virtual void Render();

protected:
	unsigned long int xpos = 0;
	unsigned long int ypos = 0;

	int originalTileSize;

	SDL_Rect destRect;
	SDL_Rect srcRect;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* objTexture = nullptr;
};
