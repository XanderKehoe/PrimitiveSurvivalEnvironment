#pragma once
#include "SDL.h"
#include "TextureManager.h"

enum class TileType { PLAIN, CLIFF, WATER, BUSH_BERRY, BUSH_FIBER, TREE, ROCK, DOOR, WALL };

class GameObject
{
public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize);
	GameObject(SDL_Texture* texture, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize);
	GameObject(SDL_Renderer* ren, int initXPos, int initYPos, int tileSize);
	void ConstructorHelper(SDL_Renderer* ren, int initXPos, int initYPos, int tileSize);
	~GameObject();

	virtual void Update() = 0;
	virtual void Render();

protected:
	int xpos = 0;
	int ypos = 0;

	SDL_Rect destRect;
	SDL_Rect srcRect;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* objTexture = nullptr;
};
