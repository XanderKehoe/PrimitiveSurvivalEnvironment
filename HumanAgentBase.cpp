#include "HumanAgentBase.h"

HumanAgentBase::HumanAgentBase(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize): Entity(texturesheet, ren, initXPos, initYPos, tileSize)
{

}

HumanAgentBase::~HumanAgentBase()
{
}

void HumanAgentBase::Update() 
{
	xpos += 1;
	// not implemented 
}

void HumanAgentBase::Render() 
{
	destRect.x = xpos;
	destRect.y = ypos;
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}

