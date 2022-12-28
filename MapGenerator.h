#pragma once

#include "Tile.h"
class MapGenerator
{
public:
	static void GenerateMap(Tile* map[Game::MAP_SIZE][Game::MAP_SIZE], SDL_Renderer* ren, int tileSize);
};



