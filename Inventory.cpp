#include "Inventory.h"
#include <stdexcept>

Inventory::Inventory()
{
	InitializeItemInInventory(ItemType::SACK, 1);
	InitializeItemInInventory(ItemType::SPEAR, 1);
	InitializeItemInInventory(ItemType::BOW, 1);
	InitializeItemInInventory(ItemType::ARROW, 10);
	InitializeItemInInventory(ItemType::AXE, 1);
	InitializeItemInInventory(ItemType::HAMMER, 1);
	InitializeItemInInventory(ItemType::BERRY, 20);
	InitializeItemInInventory(ItemType::FIBER, 20);
	InitializeItemInInventory(ItemType::MEAT, 5);
	InitializeItemInInventory(ItemType::WOOD, 5);
	InitializeItemInInventory(ItemType::ROCK, 5);
}

Inventory::~Inventory()
{
}

int Inventory::AddItemToInventory(ItemType itemType, int amount)
{
	if (items.find(itemType) != items.end()) 
	{
		// Item already exist in inventory, add to additional existing slot
		Item* currentItem = items.at(itemType);
		int currentAmount = currentItem->currentAmount;
		int maxAmount = currentItem->maxAmount;

		if (currentAmount + amount > maxAmount) 
		{
			currentItem->currentAmount = maxAmount;
			return currentAmount + amount - maxAmount;
		}
		else 
		{
			currentItem->currentAmount += amount;
			return 0;
		}
	}
	else 
	{
		throw std::logic_error("Did not find item in inventory");
	}
}

int Inventory::RemoveItemFromInventory(ItemType itemType, int amount)
{
	if (items.find(itemType) != items.end())
	{
		Item* currentItem = items.at(itemType);
		int currentAmount = currentItem->currentAmount;

		if (currentAmount - amount < 0)
		{
			currentItem->currentAmount = 0;
			return (currentAmount - amount) * -1;
		}
		else
		{
			currentItem->currentAmount -= amount;
			return 0;
		}

	}
	else 
	{
		throw std::logic_error("Did not find item in inventory");
	}
}

int Inventory::GetItemTypeAmount(ItemType itemType)
{
	if (items.find(itemType) != items.end()) 
	{
		return items.at(itemType)->currentAmount;
	}
	else
	{
		throw std::logic_error("Did not find item in inventory");
	}
}

void Inventory::InitializeItemInInventory(ItemType itemType, int maxAmount)
{
	items.insert({ itemType, new Item(itemType, maxAmount) });
}

void Inventory::PrintInventory() 
{
	std::map<ItemType, Item*>::iterator it;

	for (it = items.begin(); it != items.end(); it++) 
	{
		printf("Item: %d Amount %d", it->first, GetItemTypeAmount(it->first));
	}
}
