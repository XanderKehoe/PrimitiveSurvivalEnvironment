#include "GameObject.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos)
{
	objTexture = TextureManager::LoadTexture(texturesheet, ren);

	ConstructorHelper(ren, initXPos, initYPos);
}

GameObject::GameObject(SDL_Texture* texture, SDL_Renderer* ren, int initXPos, int initYPos)
{
	objTexture = texture;

	ConstructorHelper(ren, initXPos, initYPos);
}

GameObject::GameObject(TileType tileType, SDL_Renderer* ren, int initXPos, int initYPos)
{
	switch (tileType) 
	{
		case TileType::PLAIN: 
		{
			objTexture = TextureManager::LoadTexture("Textures/TempGrassTile.png", ren);
		}

		default:
			throw std::logic_error("Not Implemeneted");
	}

	ConstructorHelper(ren, initXPos, initYPos);
}


void GameObject::ConstructorHelper(SDL_Renderer* ren, int initXPos, int initYPos)
{
	renderer = ren;

	srcRect.h = 256;
	srcRect.w = 256;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = 64;
	destRect.w = 64;

	xpos = initXPos;
	ypos = initYPos;
}

GameObject::~GameObject()
{
}

void GameObject::Render()
{
	destRect.x = xpos;
	destRect.y = ypos;
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
