#pragma once
#include "SDL.h"
class Camera
{
public:
	static long int xPos;
	static long int yPos;

	static unsigned short zoom;

	static void Init(int screenWidth, int screenHeight, int tileSize, int mapSize);
	static void Move(int x, int y);

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	static int LEVEL_WIDTH;
	static int LEVEL_HEIGHT;

	static void AdjustZoom(bool increase);
private:
	static int TILE_SIZE;
	static void CheckAndAdjustOutOfBounds();
};

