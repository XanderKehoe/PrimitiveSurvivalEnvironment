#pragma once

#include "Tile.h"
class MapGenerator
{
public:
	static void GenerateMap(Tile* map[Config::MAP_SIZE][Config::MAP_SIZE], SDL_Renderer* ren); // Populates the given map
};



