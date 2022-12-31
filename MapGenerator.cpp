#include "MapGenerator.h"
#include <time.h>

void MapGenerator::GenerateMap(Tile* map[Config::MAP_SIZE][Config::MAP_SIZE], SDL_Renderer* ren)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < Config::MAP_SIZE; i++)
	{
		for (int j = 0; j < Config::MAP_SIZE; j++)
		{
			int initXPos = i;
			int initYPos = j;

			TileType randTileType;

			float random = (float) rand() / RAND_MAX;

			if (random < .05)
			{
				randTileType = TileType::BUSH_BERRY;
			}
			else if (random < .10)
			{
				randTileType = TileType::BUSH_FIBER;
			}
			else if (random < .125)
			{
				randTileType = TileType::ROCK;
			}
			else if (random < .175)
			{
				randTileType = TileType::TREE;
			}
			else {
				randTileType = TileType::PLAIN;
			}

			map[i][j] = new Tile(randTileType, ren, initXPos, initYPos);
			//std::cout << "new tile at [" << i << "][" << j << "] created." << std::endl;
		}
	}
}

