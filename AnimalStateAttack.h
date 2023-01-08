#pragma once

#include "AnimalStateBase.h"
class AnimalStateAttack : public AnimalStateBase
{
public:
	AnimalStateAttack(AnimalStateManager* manager, HumanAgentBase* humanAgent);
	~AnimalStateAttack();

	void EnterState() override;
	void Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;
	void PostEvent(AnimalEventType eventType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) override;

	void UpdatePath(AnimalStateManager* manager, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent);
private:
	int targetGridXPos = -1;
	int targetGridYPos = -1;

	AnimalStateManager* manager;
	HumanAgentBase* humanAgent;

	std::vector<DirectionType> targetPath;
};
