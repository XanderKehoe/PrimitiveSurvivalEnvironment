#include "MapGenerator.h"
#include <time.h>

void MapGenerator::GenerateMap(Tile* map[Game::MAP_SIZE][Game::MAP_SIZE], SDL_Renderer* ren, int tileSize)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < Game::MAP_SIZE; i++) 
	{
		for (int j = 0; j < Game::MAP_SIZE; j++) 
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

			map[i][j] = new Tile(randTileType, ren, initXPos, initYPos, tileSize);
			//std::cout << "new tile at [" << i << "][" << j << "] created." << std::endl;
		}
	}
}

