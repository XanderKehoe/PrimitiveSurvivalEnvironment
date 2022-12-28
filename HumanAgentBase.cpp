#include "HumanAgentBase.h"

HumanAgentBase::HumanAgentBase(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos): Entity(texturesheet, ren, initXPos, initYPos)
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

