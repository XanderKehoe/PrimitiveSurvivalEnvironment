#include "Entity.h"

Entity::Entity(const char* texturesheet, SDL_Renderer* ren, unsigned long initXPos, unsigned long initYPos) : GameObject(texturesheet, ren, initXPos, initYPos)
{

}

Entity::~Entity()
{

}
