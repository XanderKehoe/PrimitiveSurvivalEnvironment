#pragma once
#include "ItemType.h"
class Item
{
public:
	Item(ItemType itemType, int maxAmount, int maxAmountSackUpgrade);
	~Item();

	ItemType itemType;
	int currentAmount = 0;
	int maxAmount;
	int maxAmountSackUpgrade;

	virtual void Use();
};

