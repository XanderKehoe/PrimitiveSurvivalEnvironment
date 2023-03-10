#pragma once
#include <map>
#include "Item.h"
#include "ItemType.h"


class Inventory
{
public:
    Inventory(bool isHuman);
    ~Inventory();

    bool isHuman; // if false, allows AddItemToInventory to ignore max item amount;

    int AddItemToInventory(ItemType itemType, int amount); // Adds item to inventory, returns number of items that were NOT added (max inventory amount).
    int RemoveItemFromInventory(ItemType itemType, int amount); // Removes item from inventory, returns number of items that were NOT removed (less than 0).
    int GetItemTypeAmount(ItemType itemType); // returns the current amount of this item type in the inventory.
    bool IsFullOfItemType(ItemType itemType); // returns true is inventory is full of this item type
    void PerformSackUpgrade();
    void ResetInventory();

    std::map<ItemType, Item*> GetItems() { return items; }
private:
    std::map<ItemType, Item*> items;

    void InitializeItemInInventory(ItemType itemType, int maxAmount, int maxAmountSackUpgrade);
    void PrintInventory();
};
