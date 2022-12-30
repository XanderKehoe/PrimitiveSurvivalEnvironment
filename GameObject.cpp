#include "GameObject.h"
#include "Camera.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize)
{
	objTexture = TextureManager::LoadTexture(texturesheet, ren);

	ConstructorHelper(ren, initXPos, initYPos, tileSize);
}

GameObject::GameObject(SDL_Texture* texture, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize)
{
	objTexture = texture;

	ConstructorHelper(ren, initXPos, initYPos, tileSize);
}

// Constructor for already defined textures in child class.
GameObject::GameObject(SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize)
{
	ConstructorHelper(ren, initXPos, initYPos, tileSize);
}


void GameObject::ConstructorHelper(SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize)
{
	renderer = ren;

	srcRect.h = 256;
	srcRect.w = 256;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = tileSize;
	destRect.w = tileSize;

	originalTileSize = tileSize;

	gridXPos = initXPos;
	gridYPos = initYPos;
}

GameObject::~GameObject()
{

}

void GameObject::Render()
{
	destRect.h = originalTileSize / Camera::zoom;
	destRect.w = originalTileSize / Camera::zoom;
	destRect.x = ((gridXPos * originalTileSize) / Camera::zoom) - Camera::xPos;
	destRect.y = ((gridYPos * originalTileSize) / Camera::zoom) - Camera::yPos;
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}

void GameObject::Move(int x, int y) 
{
	gridXPos += x;
	gridYPos += y;

	if (gridXPos < 0)
		gridXPos = 0;

	if (gridYPos < 0)
		gridYPos = 0;

	if (gridXPos > Game::MAP_SIZE - 1)
		gridXPos = Game::MAP_SIZE - 1;

	if (gridYPos > Game::MAP_SIZE - 1)
		gridYPos = Game::MAP_SIZE - 1;

	std::cout << "Moving to [" << gridXPos << "][" << gridYPos << "]" << std::endl;
}
