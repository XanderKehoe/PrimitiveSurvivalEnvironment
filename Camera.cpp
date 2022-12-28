#include "Camera.h"
#include <iostream>

// static variables are declared here to get around lnk2001 (unresolved external symbol) error.
// all values set here are arbitrary and should be updated with the Init function if they should change.
int Camera::xPos = 0;
int Camera::yPos = 0;

int Camera::SCREEN_WIDTH = 800;
int Camera::SCREEN_HEIGHT = 640;

int Camera::LEVEL_WIDTH = 800;
int Camera::LEVEL_HEIGHT = 640;

void Camera::Init(int screenWidth, int screenHeight, int tileSize, int mapSize)
{
	SCREEN_WIDTH = screenWidth;
	SCREEN_HEIGHT = screenHeight;

	LEVEL_WIDTH = tileSize * mapSize;
	LEVEL_HEIGHT = tileSize * mapSize;

	//std::cout << "tileSize: " << tileSize << " * mapSize: " << mapSize << " = " << (tileSize * mapSize) << std::endl;
}

void Camera::Move(int x, int y)
{
	xPos += x; // - Game::SCREEN_WIDTH / 2;
	yPos += y; // - Game::SCREEN_HEIGHT / 2;

	if (xPos < 0) {
		xPos = 0;
	}

	if (yPos < 0) {
		yPos = 0;
	}

	if (xPos > LEVEL_WIDTH - SCREEN_WIDTH)
	{
		xPos = LEVEL_WIDTH - SCREEN_WIDTH;
	}

	if (yPos > LEVEL_HEIGHT - SCREEN_HEIGHT)
	{
		yPos = LEVEL_HEIGHT - SCREEN_HEIGHT;
	}


	//std::cout << "Camera Pos Updated, new pos is: [" << xPos << "][" << yPos << "] max X/Y: [" << (LEVEL_WIDTH - SCREEN_WIDTH) << "][ " << (LEVEL_HEIGHT - SCREEN_HEIGHT) << "]" << std::endl;
}


