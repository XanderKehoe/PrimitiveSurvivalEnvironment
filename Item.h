#pragma once
#include "ItemType.h"
class Item
{
public:
	Item(ItemType itemType, int maxAmount);
	~Item();

	ItemType itemType;
	int currentAmount = 0;
	int maxAmount;

	virtual void Use();
};

