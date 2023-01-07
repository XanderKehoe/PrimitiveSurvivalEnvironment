#include "Animal.h"
#include "AnimalStateDead.h"

Animal::Animal(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos) : Entity( textureLoadType, ren, initXPos, initYPos)
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

			health = 50;
			MAX_HEALTH = 50;
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

			health = 75;
			MAX_HEALTH = 75;
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

	stateManager = new AnimalStateManager(this);
}

Animal::~Animal()
{
}

void Animal::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], HumanAgentBase* humanAgent)
{
	Entity::Update(level);

	stateManager->Update(level, humanAgent);
}

void Animal::Render()
{
	// only render if animal is alive
	if (stateManager->currentState != stateManager->deadState) 
	{
		Entity::Render();
	}
}

bool Animal::TakeDamage(float amount)
{
	bool killed = Entity::TakeDamage(amount);
	if (killed) 
	{
		stateManager->ChangeState(stateManager->deadState);
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
