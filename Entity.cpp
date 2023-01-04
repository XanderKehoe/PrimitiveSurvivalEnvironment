#include "Entity.h"
#include "TileType.h"
#include "Tile.h"
#include <array>

Entity::Entity(TextureLoadType textureLoadType, SDL_Renderer* ren, 
	unsigned long initXPos, unsigned long initYPos) 
	: GameObject(textureLoadType, ren, initXPos, initYPos)
{

}

Entity::~Entity()
{

}

void Entity::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	if (moveCurrentCooldown > 0)
		moveCurrentCooldown--;
}

bool Entity::Move(DirectionType directionType, bool isHuman, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	if (!CanMove())
		return false;

	std::array<int, 2> xy = DirectionTypeConverter::TypeToXY(directionType);
	int x = xy[0];
	int y = xy[1];
	

	// check if movement leads GameObject off level
	if (!GridPosOutOfBounds(gridXPos + x, gridYPos + y))
	{
		TileType tileType = level[gridXPos + x][gridYPos + y]->tileType;
		//printf("tileType: %d\n", tileType);
		// check if tile is available to move in
		if (tileType == TileType::BUSH_BERRY ||
			tileType == TileType::BUSH_FIBER ||
			tileType == TileType::PLAIN ||
			(tileType == TileType::DOOR && isHuman))
		{
			// check if any other entity is occupying the same space TODO
			if (level[gridXPos + x][gridYPos + y]->attachedEntity == NULL)
			{
				// no entity already occupying this tile, can move into this tile.
				level[gridXPos + x][gridYPos + y]->attachedEntity = this; // set tile that we are moving into as 'occupied'
				level[gridXPos][gridYPos]->attachedEntity = NULL; // set tile that we are moving out of as 'free'

				gridXPos += x;
				gridYPos += y;

				if (!OVERRIDE_MOVE_COOLDOWN)
					moveCurrentCooldown = moveTimerMax;

				return true;
			}
			else // an entity is already occupying this tile.
				return false;
		}
		else // cannot move into this tile type.
			return false;
	} 
	else // movement would lead off the level.
		return false;
}

bool Entity::GridPosOutOfBounds(int gridX, int gridY) 
{
	if (gridX >= 0 && gridY >= 0 && gridX < Config::LEVEL_SIZE && gridY < Config::LEVEL_SIZE)
		return false;
	else
		return true;
}
