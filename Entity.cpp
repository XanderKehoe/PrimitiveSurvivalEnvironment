#include "Entity.h"

Entity::Entity(const char* texturesheet, SDL_Renderer* ren, unsigned long initXPos, unsigned long initYPos, unsigned int tileSize) : GameObject(texturesheet, ren, initXPos, initYPos, tileSize)
{

}

Entity::~Entity()
{

}
