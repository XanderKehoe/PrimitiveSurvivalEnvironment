#include "Entity.h"

Entity::Entity(const char* texturesheet, SDL_Renderer* ren, int initXPos, int initYPos, int tileSize) : GameObject(texturesheet, ren, initXPos, initYPos, tileSize)
{

}

Entity::~Entity()
{

}
