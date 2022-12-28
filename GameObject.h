#pragma once
#include "SDL.h"
#include "TextureManager.h"

enum class TileType { PLAIN, CLIFF, WATER, BUSH_BERRY, BUSH_FIBER, TREE, ROCK, DOOR, WALL };

class GameObject
{
public:
	GameObject(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos);
	GameObject(SDL_Texture* texture, SDL_Renderer* ren, int initXPos, int initYPos);
	GameObject(TileType tileType, SDL_Renderer* ren, int initXPos, int initYPos);
	void ConstructorHelper(SDL_Renderer* ren, int initXPos, int initYPos);
	~GameObject();

	virtual void Update() = 0;
	void Render();

protected:
	int xpos = 0;
	int ypos = 0;

private:
	SDL_Texture* objTexture = nullptr;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Renderer* renderer = nullptr;
};
