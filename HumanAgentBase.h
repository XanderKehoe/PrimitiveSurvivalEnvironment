#pragma once

#include "Entity.h"

class HumanAgentBase : public Entity
{
public:
	HumanAgentBase(const char* texturesheet, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, unsigned int tileSize);
	~HumanAgentBase();

	void Update() override;
private:
	unsigned short hunger = 100; // the health of the human agent, entity dies if this reaches 0.
	unsigned short thirst = 100; // the health of the human agent, entity dies if this reaches 0.
	unsigned short weariness = 100; // the health of the human agent, entity is forced asleep if this reaches 0.
};

