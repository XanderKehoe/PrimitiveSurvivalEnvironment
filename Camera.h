#pragma once
#include "SDL.h"
class Camera
{
public:
	static int xPos;
	static int yPos;

	static void Init(int screenWidth, int screenHeight, int tileSize, int mapSize);
	static void Move(int x, int y);

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	static int LEVEL_WIDTH;
	static int LEVEL_HEIGHT;
};

