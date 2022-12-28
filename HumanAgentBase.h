#pragma once

#include "Entity.h"

class HumanAgentBase : public Entity
{
public:
	HumanAgentBase(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos);
	~HumanAgentBase();

	void Update();
private:
	unsigned short hunger = 100;
	unsigned short thirst = 100;
	unsigned short weariness = 100;
};

