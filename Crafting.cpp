#include "Crafting.h"
#include <stdexcept>

bool Crafting::CraftItem(ItemType itemType, Inventory* inventory)
{
    if (CanCraftItem(itemType, inventory) && !inventory->IsFullOfItemType(itemType)) 
    {
        std::map<ItemType, int> craftingRecipe = GetCraftingRecipe(itemType);

        std::map<ItemType, int>::iterator it;
        for (it = craftingRecipe.begin(); it != craftingRecipe.end(); it++)
        {
            ItemType craftingRecipeItem = it->first;
            int craftingRecipeAmount = it->second;

            inventory->RemoveItemFromInventory(craftingRecipeItem, craftingRecipeAmount);
        }

        inventory->AddItemToInventory(itemType, 1);
        printf("Successfully crafted itemType: %d\n", itemType);
        return true;
    }
    else 
    {
        printf("Failed to craft itemType: %d\n", itemType);
        return false;
    }
}

bool Crafting::CanCraftItem(ItemType itemType, Inventory* inventory)
{
    std::map<ItemType, int> craftingRecipe = GetCraftingRecipe(itemType);

    std::map<ItemType, int>::iterator it;
    for (it = craftingRecipe.begin(); it != craftingRecipe.end(); it++) 
    {
        ItemType craftingRecipeItem = it->first;
        int craftingRecipeAmount = it->second;

        if (inventory->GetItemTypeAmount(craftingRecipeItem) < craftingRecipeAmount)
            return false;
    }

    return true;
}

std::map<ItemType, int> Crafting::GetCraftingRecipe(ItemType itemType)
{
    std::map<ItemType, int> craftingRecipe;

    switch (itemType)
    {
        case ItemType::SACK: 
        {
            craftingRecipe[ItemType::FIBER] = 20;
            break;
        }
        case ItemType::SPEAR:
        {
            craftingRecipe[ItemType::ROCK] = 3;
            craftingRecipe[ItemType::WOOD] = 5;
            craftingRecipe[ItemType::FIBER] = 10;
            break;
        }
        case ItemType::BOW:
        {
            craftingRecipe[ItemType::WOOD] = 5;
            craftingRecipe[ItemType::FIBER] = 15;
            break;
        }
        case ItemType::ARROW:
        {
            craftingRecipe[ItemType::ROCK] = 1;
            craftingRecipe[ItemType::WOOD] = 1;
            craftingRecipe[ItemType::FIBER] = 1;
            break;
        }
        case ItemType::AXE:
        {
            craftingRecipe[ItemType::ROCK] = 3;
            craftingRecipe[ItemType::WOOD] = 5;
            craftingRecipe[ItemType::FIBER] = 10;
            break;
        }
        case ItemType::HAMMER:
        {
            craftingRecipe[ItemType::ROCK] = 5;
            craftingRecipe[ItemType::WOOD] = 5;
            craftingRecipe[ItemType::FIBER] = 10;
            break;
        }
        case ItemType::WALL:
        {
            craftingRecipe[ItemType::WOOD] = 5;
            craftingRecipe[ItemType::FIBER] = 5;
            break;
        }

        default:
            throw std::logic_error("Not implemented");
    }

    return craftingRecipe;
}
