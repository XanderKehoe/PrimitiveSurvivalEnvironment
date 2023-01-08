#pragma once

#include "AnimalStateBase.h"
class AnimalStateFlee : public AnimalStateBase
{
public:
	AnimalStateFlee(AnimalStateManager* manager, HumanAgentBase* humanAgent);
	~AnimalStateFlee();

	void EnterState() override;
	void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
	void PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;

	void UpdatePath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent);
private:
	AnimalStateManager* manager;
	HumanAgentBase* humanAgent;

	std::vector<DirectionType> targetPath;

	bool pathInitialized;

	bool TryNewPath(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], int startX, int startY, int endX, int endY);
};
