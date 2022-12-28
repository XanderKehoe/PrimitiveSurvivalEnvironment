#include "MapGenerator.h"

void MapGenerator::GenerateMap(Tile* map[Game::MAP_SIZE][Game::MAP_SIZE], SDL_Renderer* ren, int tileSize)
{
	for (int i = 0; i < Game::MAP_SIZE; i++) 
	{
		for (int j = 0; j < Game::MAP_SIZE; j++) 
		{
			map[i][j] = new Tile(TileType::PLAIN, ren, i * tileSize, j * tileSize, tileSize);
		}
	}
}

