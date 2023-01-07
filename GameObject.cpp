#include "GameObject.h"
#include "Camera.h"


GameObject::GameObject(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos)
{
	objTexture = TextureManager::LoadTextureByType(textureLoadType);

	ConstructorHelper(ren, initXPos, initYPos);
}

// Constructor for already defined textures in child class.
GameObject::GameObject(SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos)
{
	ConstructorHelper(ren, initXPos, initYPos);
}


void GameObject::ConstructorHelper(SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos)
{
	renderer = ren;

	srcRect.h = 256;
	srcRect.w = 256;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = Config::TILE_SIZE;
	destRect.w = Config::TILE_SIZE;

	gridXPos = initXPos;
	gridYPos = initYPos;
}

GameObject::~GameObject()
{

}

void GameObject::Render()
{
	destRect.h = Config::TILE_SIZE / Camera::zoom;
	destRect.w = Config::TILE_SIZE / Camera::zoom;
	destRect.x = (gridXPos * (Config::TILE_SIZE / Camera::zoom)) - Camera::xPos;
	destRect.y = (gridYPos * (Config::TILE_SIZE / Camera::zoom)) - Camera::yPos;
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
