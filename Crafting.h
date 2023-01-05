#pragma once
#include "ItemType.h"
#include "Inventory.h"
class Crafting
{
public:
	static bool CraftItem(ItemType itemType, Inventory* inventory);
	static bool CanCraftItem(ItemType itemType, Inventory* inventory);
	static std::map<ItemType, int> GetCraftingRecipe(ItemType itemType);
private:
};