#pragma once
#include "Config.h"
#include "SDL.h"

class LevelGenerator
{
public:
	static void GenerateLevel(class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], SDL_Renderer* ren); // Populates the given map
};



