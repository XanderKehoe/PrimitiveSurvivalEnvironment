#pragma once
#include "Entity.h"
class Animal : public Entity
{
public:
	Animal();
	~Animal();

	bool IsHostile() { return isHostile; }
private:
	bool isHostile = false;
};

