#include "HumanAgentBase.h"

HumanAgentBase::HumanAgentBase(const char* texturesheet, SDL_Renderer* ren, unsigned long  initXPos, unsigned long  initYPos, unsigned int tileSize): Entity(texturesheet, ren, initXPos, initYPos, tileSize)
{

}

HumanAgentBase::~HumanAgentBase()
{
}

void HumanAgentBase::Update() 
{
	// xpos += 1;
	// not implemented 
}

