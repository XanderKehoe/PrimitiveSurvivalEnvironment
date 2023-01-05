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
	inventory = new Inventory();
}

HumanAgentBase::~HumanAgentBase()
{

}

UpdateResult HumanAgentBase::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], ActionType actionType)
{
	reward = 0; // resetting reward

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

bool HumanAgentBase::TakeAction(ActionType action, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	bool actionWasValid;
	// TODO
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

		/* correct code, but leaving this out for now to test just grabbing berries
		// check if tile is interactable
		if (thisTile->available && (
			tileType == TileType::BUSH_BERRY ||
			tileType == TileType::BUSH_FIBER || 
			tileType == TileType::ROCK ||
			tileType == TileType::TREE))
		{
			thisTile->HumanInteract(this);
			return true;
		}
		*/
		if (thisTile->available && (
			tileType == TileType::BUSH_BERRY)) 
		{
			thisTile->HumanInteract(this);
			reward += RewardType::PICKUP_BERRY;
			return true;
		}
		else if (thisTile->attachedEntity != NULL) // check if tile contains an entity that be be interacted with
		{
			// TODO
			return false; // PLACEHOLDER
		}
		else 
			return false;

	}
	else // tile is out of bounds.
		return false;
}



