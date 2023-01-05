#pragma once
#include "ItemType.h"
#include "Inventory.h"
class Crafting
{
public:
	static bool CraftItem(ItemType itemType, Inventory* inventory); // Currently only supports crafting 1 item at a time, will need to be updated in the future if deciding to change this.
	static bool CanCraftItem(ItemType itemType, Inventory* inventory);
	static std::map<ItemType, int> GetCraftingRecipe(ItemType itemType);
private:
};