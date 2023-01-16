#pragma once
#include "HumanAgentBase.h"
/*
* This class serves to provide custom observations while inheriting the same behaviors (updates, actions, rewards, etc.) from the HumanAgentBase class
*/
class HumanAgent : public HumanAgentBase
{
public:
	HumanAgent(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos);
	~HumanAgent();

	std::vector<float> GetObservationsFloat(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;

	std::vector<int> GetObservationsInt(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
private:
	static const int OBS_VIEW_LENGTH = 3; // how many tiles the agent can see out from where it is standing, should be an even number.
};

