#include "GameObject.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize)
{
	objTexture = TextureManager::LoadTexture(texturesheet, ren);

	ConstructorHelper(ren, initXPos, initYPos, tileSize);
}

GameObject::GameObject(SDL_Texture* texture, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize)
{
	objTexture = texture;

	ConstructorHelper(ren, initXPos, initYPos, tileSize);
}

GameObject::GameObject(TileType tileType, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize)
{
	ConstructorHelper(ren, initXPos, initYPos, tileSize);

	switch (tileType) 
	{
		case TileType::PLAIN: 
		{
			objTexture = TextureManager::LoadTexture("Textures/TempGrassTile.png", ren);
			break;
		}

		default:
			throw std::logic_error("Not Implemeneted");
	}
}


void GameObject::ConstructorHelper(SDL_Renderer* ren, int initXPos, int initYPos, int tileSize)
{
	renderer = ren;

	srcRect.h = 256;
	srcRect.w = 256;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = tileSize;
	destRect.w = tileSize;

	xpos = initXPos;
	ypos = initYPos;
}

GameObject::~GameObject()
{

}
