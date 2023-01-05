#include "Item.h"

Item::Item(ItemType itemType, int maxAmount, int maxAmountSackUpgrade)
{
	this->itemType = itemType;
	this->maxAmount = maxAmount;
	this->maxAmountSackUpgrade = maxAmountSackUpgrade;
}

Item::~Item()
{
}

void Item::Use()
{
	// By default, items will not have any Use as some do not have any. This can be overwritten for items that do have a Use.
}
