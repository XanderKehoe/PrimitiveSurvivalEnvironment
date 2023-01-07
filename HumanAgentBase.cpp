#include <array>
#include "HumanAgentBase.h"
#include "TileType.h"
#include "Tile.h"
#include "RewardType.h"
#include "Crafting.h"

HumanAgentBase::HumanAgentBase(TextureLoadType textureLoadType, SDL_Renderer* ren, 
	unsigned long initXPos, unsigned long initYPos)
	: Entity(textureLoadType, ren, initXPos, initYPos)
{
	health = 100;
	MAX_HEALTH = 100;
	attackDamage = 5;

	moveTimerMax = 3;

	inventory->isHuman = true;
}

HumanAgentBase::~HumanAgentBase()
{

}

UpdateResult HumanAgentBase::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], ActionType actionType)
{
	reward = 0; // resetting reward
	if (startVisibilityCount > 0) // slowly countdown temp visability blocking for hostile animals
		startVisibilityCount--;

	Entity::Update(level);

	this->TakeAction(actionType, level);

	bool done = false;
	step++;
	if (step > MAX_STEPS) 
	{
		done = true;
		// In future, call reset function here
		step = 0;
	}

	UpdateResult updateResult;
	updateResult.reward = reward;
	updateResult.done = done;

	return updateResult;
}

bool HumanAgentBase::TakeDamage(float amount)
{
	bool killed = Entity::TakeDamage(amount);
	if (killed) 
	{
		reward += RewardType::DEATH;
		Respawn();
		// INSERT EPISODE RESET HERE
	}

	return killed;
}

bool HumanAgentBase::TakeAction(ActionType action, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	bool actionWasValid;

	switch (action) 
	{
		case ActionType::NOTHING:
		{
			actionWasValid = true;
			break;
		}
		case ActionType::MOVE_DOWN: 
		{
			actionWasValid = Move(DirectionType::DOWN, true, level);
			break;
		}
		case ActionType::MOVE_LEFT:
		{
			actionWasValid = Move(DirectionType::LEFT, true, level);
			break;
		}
		case ActionType::MOVE_UP:
		{
			actionWasValid = Move(DirectionType::UP, true, level);
			break;
		}
		case ActionType::MOVE_RIGHT:
		{
			actionWasValid = Move(DirectionType::RIGHT, true, level);
			break;
		}
		case ActionType::INTERACT_DOWN:
		{
			actionWasValid = Interact(DirectionType::DOWN, level);
			break;
		}
		case ActionType::INTERACT_LEFT:
		{
			actionWasValid = Interact(DirectionType::LEFT, level);
			break;
		}
		case ActionType::INTERACT_UP:
		{
			actionWasValid = Interact(DirectionType::UP, level);
			break;
		}
		case ActionType::INTERACT_RIGHT:
		{
			actionWasValid = Interact(DirectionType::RIGHT, level);
			break;
		}
		case ActionType::CRAFT_SACK:
		{
			actionWasValid = Crafting::CraftItem(ItemType::SACK, inventory);
			if (actionWasValid)
				inventory->PerformSackUpgrade();
			break;
		}
		case ActionType::CRAFT_SPEAR:
		{
			actionWasValid = Crafting::CraftItem(ItemType::SPEAR, inventory);
			break;
		}
		case ActionType::CRAFT_BOW:
		{
			actionWasValid = Crafting::CraftItem(ItemType::BOW, inventory);
			break;
		}
		case ActionType::CRAFT_ARROW:
		{
			actionWasValid = Crafting::CraftItem(ItemType::ARROW, inventory);
			break;
		}
		case ActionType::CRAFT_AXE:
		{
			actionWasValid = Crafting::CraftItem(ItemType::AXE, inventory);
			break;
		}
		case ActionType::CRAFT_HAMMER:
		{
			actionWasValid = Crafting::CraftItem(ItemType::HAMMER, inventory);
			break;
		}
		case ActionType::CRAFT_WALL:
		{
			actionWasValid = Crafting::CraftItem(ItemType::WALL, inventory);
			break;
		}
		case ActionType::TOGGLE_SNEAK: 
		{
			actionWasValid = true;
			sneaking = !sneaking;
			break;
		}

		default: 
		{
			throw std::logic_error("Not Implemeneted");
		}
	}

	if (!actionWasValid)
	{
		//printf("Invalid Action\n");
		reward += RewardType::INVALID_ACTION;
		return false;
	}
	else 
	{
		//if (action != ActionType::NOTHING)
			//printf("Valid Action\n");
		return true;
	}
}

bool HumanAgentBase::IsHidingInBush()
{
	if (GetAttachedTile() != nullptr)
	{
		if (IsSneaking() && GetAttachedTile()->available && 
			(GetAttachedTile()->GetResourceType() == ItemType::BERRY || GetAttachedTile()->GetResourceType() == ItemType::FIBER))
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool HumanAgentBase::Interact(DirectionType directionType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) 
{
	std::array<int, 2> xy = DirectionTypeConverter::TypeToXY(directionType);
	int x = xy[0];
	int y = xy[1];

	// check if tile is out of bounds.
	if (!GridPosOutOfBounds(gridXPos + x, gridYPos + y))
	{
		Tile* thisTile = level[gridXPos + x][gridYPos + y];
		TileType tileType = thisTile->tileType;

		// check if tile is interactable
		if (thisTile->available && (
			tileType == TileType::BUSH_BERRY ||
			tileType == TileType::BUSH_FIBER || 
			tileType == TileType::ROCK ||
			tileType == TileType::TREE))
		{
			bool success = thisTile->HumanInteract(this); // returns false if inventory is full for this item type

			if (success)
			{
				if (tileType == TileType::BUSH_BERRY)
					reward += RewardType::PICKUP_BERRY;
				else
					reward += RewardType::PICKUP_RESOURCE;
			}

			return true;
		}
		else if (thisTile->attachedEntity != nullptr) // check if tile contains an entity that be be interacted with
		{
			Entity* interactingEntity = thisTile->attachedEntity;
			float damageAmount = attackDamage;
			if (inventory->GetItemTypeAmount(ItemType::SPEAR))
				damageAmount *= 4;

			bool killed = interactingEntity->TakeDamage(damageAmount);
			if (killed) 
			{
				std::map<ItemType, Item*>::iterator it;
				std::map<ItemType, Item*> killedInventory = interactingEntity->GetInventory()->GetItems();
				
				for (it = killedInventory.begin(); it != killedInventory.end(); it++) 
				{
					if (it->second->currentAmount > 0) 
					{
						int leftover = inventory->AddItemToInventory(it->first, it->second->currentAmount);

						if (it->first == ItemType::MEAT) 
						{
							reward += RewardType::PICKUP_MEAT * (it->second->currentAmount - leftover);
							//printf("picked up meat! %d\n", it->second->currentAmount);
						}
					}
				}
			}

			return true;
		}
		else 
			return false;

	}
	else // tile is out of bounds.
		return false;
}

void HumanAgentBase::Render() 
{
	bool hasSpear = inventory->GetItemTypeAmount(ItemType::SPEAR) == 1;
	if (sneaking) 
	{
		if (hasSpear)
			objTexture = TextureManager::LoadTextureByType(TextureLoadType::ENTITY_HUMAN_WITH_SPEAR_CROUCH);
		else
			objTexture = TextureManager::LoadTextureByType(TextureLoadType::ENTITY_HUMAN_CROUCH);
	}
	else 
	{
		if (hasSpear)
			objTexture = TextureManager::LoadTextureByType(TextureLoadType::ENTITY_HUMAN_WITH_SPEAR);
		else
			objTexture = TextureManager::LoadTextureByType(TextureLoadType::ENTITY_HUMAN);
	}

	GameObject::Render();
}



