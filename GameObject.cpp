#include "GameObject.h"
#include "Camera.h"

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

// Constructor for already defined textures in child class.
GameObject::GameObject(SDL_Renderer* ren, int initXPos, int initYPos, int tileSize)
{
	ConstructorHelper(ren, initXPos, initYPos, tileSize);
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

void GameObject::Render()
{
	destRect.x = xpos - Camera::xPos;
	destRect.y = ypos - Camera::yPos;
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
