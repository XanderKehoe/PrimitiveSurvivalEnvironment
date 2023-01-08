#include "Animal.h"
#include "AnimalStateDead.h"

Animal::Animal(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos, HumanAgentBase* humanAgent) : Entity( textureLoadType, ren, initXPos, initYPos)
{
	// Derive animal type from textureLoadType
	switch (textureLoadType)
	{
		case TextureLoadType::ENTITY_RABBIT: 
		{
			animalType = AnimalType::RABBIT;

			health = 20;
			MAX_HEALTH = 20;
			isHostile = false;
			attackDamage = 0;
			sightRange = 4;
			moveTimerMax = 3;

			FillInventory();
			break;
		}
		case TextureLoadType::ENTITY_DEER:
		{
			animalType = AnimalType::DEER;

			health = 40;
			MAX_HEALTH = 40;
			isHostile = false;
			attackDamage = 0;
			sightRange = 6;
			moveTimerMax = 2;

			FillInventory();
			break;
		}
		case TextureLoadType::ENTITY_ELK:
		{
			animalType = AnimalType::ELK;

			health = 150;
			MAX_HEALTH = 150;
			isHostile = false;
			attackDamage = 0;
			sightRange = 8;
			moveTimerMax = 1;

			FillInventory();
			break;
		}
		case TextureLoadType::ENTITY_GOOSE:
		{
			animalType = AnimalType::GOOSE;

			health = 30;
			MAX_HEALTH = 30;
			isHostile = true;
			attackDamage = 0;
			sightRange = 4;
			moveTimerMax = 3;

			FillInventory();
			break;
		}
		case TextureLoadType::ENTITY_WOLF:
		{
			animalType = AnimalType::WOLF;

			health = 70;
			MAX_HEALTH = 70;
			isHostile = true;
			attackDamage = 15;
			sightRange = 6;
			moveTimerMax = 1;

			FillInventory();
			break;
		}
		case TextureLoadType::ENTITY_BEAR:
		{
			animalType = AnimalType::BEAR;

			health = 125;
			MAX_HEALTH = 125;
			isHostile = true;
			attackDamage = 30;
			sightRange = 6;
			moveTimerMax = 2;

			FillInventory();
			break;
		}

		default:
			throw std::logic_error("Invalid load type for Animal constructor");
	}

	stateManager = new AnimalStateManager(this, humanAgent);
}

Animal::~Animal()
{
}

void Animal::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	Entity::Update(level);

	stateManager->Update(level);
}

void Animal::Render()
{
	// only render if animal is alive
	if (stateManager->currentState != stateManager->deadState) 
	{
		Entity::Render();
	}
}

int Animal::GetEntityType()
{
	return 1 + static_cast<int>(animalType);
}

bool Animal::TakeDamage(float amount, bool fromBow, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	//printf("Took Damage\n");
	bool killed = Entity::TakeDamage(amount);
	if (killed) 
	{
		stateManager->ChangeState(stateManager->deadState);
	}
	else if (fromBow) 
	{
		stateManager->PostEvent(AnimalEventType::TAKE_DAMAGE, level);
	}

	return killed;
}

bool Animal::IsWithinSightRange(int modifiedSightRange, Entity* entity)
{
	if (abs(entity->GetGridXPos() - GetGridXPos()) <= modifiedSightRange && abs(entity->GetGridYPos() - GetGridYPos()) <= modifiedSightRange)
		return true;
	else
		return false;
}

void Animal::FillInventory()
{
	switch (animalType) 
	{
		case AnimalType::RABBIT: 
		{
			inventory->AddItemToInventory(ItemType::MEAT, 1);
			break;
		}
		case AnimalType::DEER:
		{
			inventory->AddItemToInventory(ItemType::MEAT, 3);
			break;
		}
		case AnimalType::ELK:
		{
			inventory->AddItemToInventory(ItemType::MEAT, 6);
			break;
		}
		case AnimalType::GOOSE:
		{
			inventory->AddItemToInventory(ItemType::MEAT, 1);
			break;
		}
		case AnimalType::WOLF:
		{
			inventory->AddItemToInventory(ItemType::MEAT, 3);
			break;
		}
		case AnimalType::BEAR:
		{
			inventory->AddItemToInventory(ItemType::MEAT, 6);
			break;
		}

		default:
			throw std::logic_error("Invalid AnimalType");
	}
}
