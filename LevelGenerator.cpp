#include <time.h>
#include "LevelGenerator.h"
#include "Tile.h"

void LevelGenerator::GenerateLevel(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], SDL_Renderer* ren)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < Config::LEVEL_SIZE; i++)
	{
		for (int j = 0; j < Config::LEVEL_SIZE; j++)
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

			level[i][j] = new Tile(randTileType, ren, initXPos, initYPos);
			//std::cout << "new tile at [" << i << "][" << j << "] created." << std::endl;
		}
	}

	printf("Level has been generated!\n");
}

