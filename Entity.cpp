#include "Entity.h"
#include "TileType.h"
#include "Tile.h"
#include <array>
#include "Camera.h"

Tile* attachedTile;

Entity::Entity(TextureLoadType textureLoadType, SDL_Renderer* ren, 
	unsigned long initXPos, unsigned long initYPos) 
	: GameObject(textureLoadType, ren, initXPos, initYPos)
{
	inventory = new Inventory(false);
}

Entity::~Entity()
{

}

void Entity::Update(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	if (health < MAX_HEALTH)
		health += 0.25F; // passive health regeneration

	movesLeft = MOVES_PER_UPDATE;
}

bool Entity::Move(DirectionType directionType, bool isHuman, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	if (!CanMove()) 
	{
		printf("WARNING: return false on Move(), should check CanMove() before using Move()");
		return false;
	}

	std::array<int, 2> xy = DirectionTypeConverter::TypeToXY(directionType);
	int x = xy[0];
	int y = xy[1];

	movesLeft--;

	// check if movement leads GameObject off level
	if (!GridPosOutOfBounds(gridXPos + x, gridYPos + y))
	{
		TileType tileType = level[gridXPos + x][gridYPos + y]->tileType;
		//printf("tileType: %d\n", tileType);
		// check if tile is available to move in
		if (level[gridXPos + x][gridYPos + y]->IsWalkable(isHuman))
		{
			// check if any other entity is occupying the same space TODO
			if (level[gridXPos + x][gridYPos + y]->attachedEntity == NULL)
			{
				// no entity already occupying this tile, can move into this tile.
				level[gridXPos + x][gridYPos + y]->attachedEntity = this; // set tile that we are moving into as 'occupied'
				level[gridXPos][gridYPos]->attachedEntity = NULL; // set tile that we are moving out of as 'free'
				attachedTile = level[gridXPos + x][gridYPos + y];

				gridXPos += x;
				gridYPos += y;

				if (isHuman && Camera::IsCoordOutsideCamView(gridXPos * Config::TILE_SIZE, gridYPos * Config::TILE_SIZE, (Config::TILE_SIZE * 3))) // if agent is within 3 tiles of camera's view border, recenter the camera
					Camera::Center(gridXPos, gridYPos);

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

bool Entity::TakeDamage(float amount, bool fromBow, Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
	health -= amount;
	if (health < 0) 
	{
		isDead = true;
		attachedTile->attachedEntity = nullptr;
		return true;
	}
	else
		return false;
}

void Entity::Respawn() 
{
	health = MAX_HEALTH;
	isDead = false;
}

void Entity::Render()
{
	GameObject::Render();
}

bool Entity::GridPosOutOfBounds(int gridX, int gridY) 
{
	if (gridX >= 0 && gridY >= 0 && gridX < Config::LEVEL_SIZE && gridY < Config::LEVEL_SIZE)
		return false;
	else
		return true;
}
