#include "HumanAgentBase.h"
#include "TileType.h"
#include "Tile.h"

HumanAgentBase::HumanAgentBase(TextureLoadType textureLoadType, SDL_Renderer* ren, 
	unsigned long initXPos, unsigned long initYPos)
	: Entity(textureLoadType, ren, initXPos, initYPos)
{
	inventory = new Inventory();
}

HumanAgentBase::~HumanAgentBase()
{

}

void HumanAgentBase::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	Entity::Update(level);

	if (!printedObservations) {
		// DELETE ALL OF THIS
		printedObservations = true;
		std::vector<int> observations = GetObservations(level); // need to delete this dynamically allocated memory here at some point
	}

	// Move(0, 1, true, level);
	// not implemented 
}

bool HumanAgentBase::TakeAction(ActionType action, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	// TODO
	switch (action) 
	{
		case ActionType::MOVE_DOWN: 
		{
			return Move(DirectionType::DOWN, true, level);
		}
		case ActionType::MOVE_LEFT:
		{
			return Move(DirectionType::LEFT, true, level);
		}
		case ActionType::MOVE_UP:
		{
			return Move(DirectionType::UP, true, level);
		}
		case ActionType::MOVE_RIGHT:
		{
			return Move(DirectionType::RIGHT, true, level);
		}
		case ActionType::INTERACT_DOWN:
		{
			return Interact(DirectionType::DOWN, level);
		}
		case ActionType::INTERACT_LEFT:
		{
			return Interact(DirectionType::LEFT, level);
		}
		case ActionType::INTERACT_UP:
		{
			return Interact(DirectionType::UP, level);
		}
		case ActionType::INTERACT_RIGHT:
		{
			return Interact(DirectionType::RIGHT, level);
		}

		default:
			throw std::logic_error("Not Implemeneted");

	}
}

bool HumanAgentBase::Interact(DirectionType directionType, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]) 
{
	int* xy = DirectionTypeConverter::TypeToXY(directionType);
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
			thisTile->HumanInteract(this);
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



