#include <array>
#include "HumanAgentBase.h"
#include "TileType.h"
#include "Tile.h"
#include "RewardType.h"
#include "Crafting.h"
#include "Animal.h"

HumanAgentBase::HumanAgentBase(TextureLoadType textureLoadType, SDL_Renderer* ren, 
	unsigned long initXPos, unsigned long initYPos)
	: Entity(textureLoadType, ren, initXPos, initYPos)
{
	health = 100;
	MAX_HEALTH = 100;
	attackDamage = 5;

	MOVES_PER_UPDATE = 1;

	inventory->isHuman = true;

	startGridX = initXPos;
	startGridY = initYPos;
}

HumanAgentBase::~HumanAgentBase()
{

}

UpdateResult HumanAgentBase::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], ActionType actionType, std::list<Animal*> animalList)
{
	if (startVisibilityCount > 0) // slowly countdown temp visability blocking for hostile animals
		startVisibilityCount--;
	//else if (startVisibilityCount == 1)
		//printf("Visibility block disables in 1 step\n");

	if (currentBowCooldown > 0)
		currentBowCooldown--;

	ReduceHunger(BASE_HUNGER_DRAIN);

	Entity::Update(level);

	TakeAction(actionType, level, animalList);

	CheckHearing(animalList);

	if (!IsDead())
		reward += RewardType::BEING_ALIVE;

	step++;
	if (step > MAX_STEPS) 
	{
		printf("MAX_STEPS reached, starting new episode");
		reward += RewardType::SURVIVED_ENTIRE_EPISODE;
		done = true;
	}

	UpdateResult updateResult;
	updateResult.reward = reward;
	updateResult.done = done;

	if (done) 
	{
		Respawn();
	}

	reward = 0; // resetting reward

	return updateResult;
}

bool HumanAgentBase::TakeDamage(float amount, bool fromBow, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	reward += RewardType::TAKE_DAMAGE * (amount / MAX_HEALTH);

	bool killed = Entity::TakeDamage(amount);
	if (killed) 
	{
		reward += RewardType::DEATH;
		done = true;
	}

	return killed;
}

void HumanAgentBase::Respawn() 
{
	Entity::Respawn();

	hunger = MAX_HUNGER;
	thirst = MAX_THIRST;
	weariness = MAX_WEARINESS;
	inventory->ResetInventory();

	startVisibilityCount = START_VISIBILITY;

	startGridX = gridXPos;
	startGridY = gridYPos;

	step = 0;
	done = false;
}

bool HumanAgentBase::Move(DirectionType directionType, bool isHuman, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	bool moved = Entity::Move(directionType, isHuman, level);

	if (moved) 
	{
		ReduceHunger(MOVE_HUNGER_DRAIN);

		if (sneaking)
			movedWhileSneaking = true;
	}
	else if (sneaking)
		movedWhileSneaking = false;

	return moved;
}

bool HumanAgentBase::CanMove()
{
	if (movesLeft == 0)
		return false;

	if (!sneaking)
		return true;
	else 
	{
		if (!movedWhileSneaking) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
}

bool HumanAgentBase::TakeAction(ActionType action, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], std::list<Animal*> animalList)
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
			if (CanMove())
				actionWasValid = Move(DirectionType::DOWN, true, level);
			else
				actionWasValid = false;
			break;
		}
		case ActionType::MOVE_LEFT:
		{
			if (CanMove())
				actionWasValid = Move(DirectionType::LEFT, true, level);
			else
				actionWasValid = false;
			break;
		}
		case ActionType::MOVE_UP:
		{
			if (CanMove())
				actionWasValid = Move(DirectionType::UP, true, level);
			else
				actionWasValid = false;
			break;
		}
		case ActionType::MOVE_RIGHT:
		{
			if (CanMove())
				actionWasValid = Move(DirectionType::RIGHT, true, level);
			else
				actionWasValid = false;
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
			{
				reward += RewardType::CRAFT_TOOL;
				inventory->PerformSackUpgrade();
				printf("\tSuccessfully Crafted Sack!\n");
			}
			break;
		}
		case ActionType::CRAFT_SPEAR:
		{
			actionWasValid = Crafting::CraftItem(ItemType::SPEAR, inventory);
			if (actionWasValid) 
			{
				reward += RewardType::CRAFT_TOOL;
				printf("\tSuccessfully Crafted Spear!\n");
			}
			break;
		}
		case ActionType::CRAFT_BOW:
		{
			actionWasValid = Crafting::CraftItem(ItemType::BOW, inventory);
			if (actionWasValid) 
			{
				reward += RewardType::CRAFT_TOOL;
				printf("\tSuccessfully Crafted Bow!\n");
			}
			break;
		}
		case ActionType::CRAFT_ARROW:
		{
			actionWasValid = Crafting::CraftItem(ItemType::ARROW, inventory);
			if (actionWasValid) 
			{
				reward += RewardType::CRAFT_ARROW;
				printf("\tSuccessfully Crafted Arrow!\n");
			}
			break;
		}
		case ActionType::CRAFT_AXE:
		{
			actionWasValid = Crafting::CraftItem(ItemType::AXE, inventory);
			if (actionWasValid)
			{
				reward += RewardType::CRAFT_TOOL;
				printf("\tSuccessfully Crafted Axe!\n");
			}
			break;
		}
		case ActionType::CRAFT_HAMMER:
		{
			actionWasValid = Crafting::CraftItem(ItemType::HAMMER, inventory);
			if (actionWasValid) 
			{
				reward += RewardType::CRAFT_TOOL;
				printf("\tSuccessfully Crafted Hammer!\n");
			}
			break;
		}
		case ActionType::CRAFT_WALL:
		{
			actionWasValid = Crafting::CraftItem(ItemType::WALL, inventory);
			if (actionWasValid)
				reward += RewardType::CRAFT_WALL;
			break;
		}
		case ActionType::TOGGLE_SNEAK: 
		{
			actionWasValid = true;
			ToggleSneaking();
			break;
		}
		case ActionType::SHOOT_BOW:
		{
			actionWasValid = ShootBow(animalList, level);
			if (actionWasValid) 
			{
				printf("\tSuccessfully used the bow!\n");
			}
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
			startVisibilityCount = 0; // attacking an entity disabled start visibility

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

void HumanAgentBase::ReduceHunger(float amount)
{
	hunger -= amount;

	if (hunger < MEAT_HUNGER_RESTORE_AMOUNT) 
	{
		if (inventory->GetItemTypeAmount(ItemType::MEAT) > 0) 
		{
			inventory->RemoveItemFromInventory(ItemType::MEAT, 1);
			hunger += MEAT_HUNGER_RESTORE_AMOUNT;
		}
		else if (inventory->GetItemTypeAmount(ItemType::BERRY) > 0)
		{
			inventory->RemoveItemFromInventory(ItemType::BERRY, 1);
			hunger += BERRY_HUNGER_RESTORE_AMOUNT;
		}
	}
	
	if (hunger <= 0) 
	{
		hunger = 0;
		bool killed = TakeDamage(STARVATION_DAMAGE_AMOUNT);
		if (killed) 
		{
			printf("Agent died due to starvation... %d\n", SDL_GetTicks());
		}
	}
}

void HumanAgentBase::CheckHearing(std::list<Animal*> animalList)
{
	// reset hearing
	hearAnimal = false;
	hearHostileAnimal = false;
	hearPassiveAnimal = false;

	for (Animal* a : animalList) 
	{
		if (a->IsDead())
			continue;

		int distance = abs(gridXPos - a->GetGridXPos()) + abs(gridYPos - a->GetGridYPos());
		if (distance < SPECIFIC_HEARING_RANGE)
		{
			if (a->IsHostile()) 
			{
				//printf("Hear hostile animal\n");
				hearAnimal = true;
				hearHostileAnimal = true;
			}
			else 
			{
				//printf("Hear passive animal\n");
				hearAnimal = true;
				hearPassiveAnimal = true;
			}
		}
		else if (distance < GENERAL_HEARING_RANGE) 
		{
			//printf("Hear some animal\n");
			hearAnimal = true;
		}

		if (hearAnimal && hearHostileAnimal && hearPassiveAnimal)
			break;
	}
}

void HumanAgentBase::ToggleSneaking()
{
	sneaking = !sneaking;
	if (!sneaking)
		movedWhileSneaking = false;
}

bool HumanAgentBase::ShootBow(std::list<Animal*> animalList, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	if (currentBowCooldown > 0)
		return false; // cooldown is still active, need to wait.

	if (inventory->GetItemTypeAmount(ItemType::BOW) == 0 || inventory->GetItemTypeAmount(ItemType::ARROW) == 0)
		return false; // no bow and/or no arrows

	int closestDistance = INT_MAX;
	Animal* closestAnimal = nullptr;

	for (Animal* a : animalList)
	{
		if (a->IsDead())
			continue;

		int distance = abs(gridXPos - a->GetGridXPos()) + abs(gridYPos - a->GetGridYPos());
		if (distance < closestDistance) 
		{
			closestDistance = distance;
			closestAnimal = a;
		}
	}

	if (closestDistance > BOW_MAX_RANGE)
		return false;
	else 
	{
		// May need to implement something here to 'alert' animals they've taken damage, so they just don't wander around after being shot.
		closestAnimal->TakeDamage(BOW_DAMAGE, true, level);
		inventory->RemoveItemFromInventory(ItemType::ARROW, 1);
		startVisibilityCount = 0; // turn off visibility block if damaging other entities.
		return true;
	}

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

int HumanAgentBase::GetEntityType()
{
	printf("ERROR, HumanAgentBase::GetEntityType() called, but shouldn't have...");
	throw std::logic_error("Invalid Functionality");
}



