#include "Camera.h"
#include "Config.h"
#include <iostream>

// static variables are declared here to get around lnk2001 (unresolved external symbol) error.
// all values set here are arbitrary and should be updated with the Init function if they should change.
long int Camera::xPos = 0;
long int Camera::yPos = 0;

unsigned short Camera::zoom = 1;

int Camera::SCREEN_WIDTH = 800;
int Camera::SCREEN_HEIGHT = 640;

int Camera::LEVEL_WIDTH = 800;
int Camera::LEVEL_HEIGHT = 640;

void Camera::Init(int screenWidth, int screenHeight)
{
	SCREEN_WIDTH = screenWidth;
	SCREEN_HEIGHT = screenHeight;

	LEVEL_WIDTH = Config::TILE_SIZE * Config::LEVEL_SIZE;
	LEVEL_HEIGHT = Config::TILE_SIZE * Config::LEVEL_SIZE;
}

void Camera::Move(int x, int y)
{
	xPos += x; // - Game::SCREEN_WIDTH / 2;
	yPos += y; // - Game::SCREEN_HEIGHT / 2;

	CheckAndAdjustOutOfBounds();

	//std::cout << "Camera Pos Updated, new pos is: [" << xPos << "][" << yPos << "] max X/Y: [" << (LEVEL_WIDTH - SCREEN_WIDTH) << "][ " << (LEVEL_HEIGHT - SCREEN_HEIGHT) << "]" << std::endl;
}

void Camera::AdjustZoom(bool increase)
{
	if (increase) 
	{
		// weird graphic glitches when zooming out past 4, hard capping here to prevent this.
		if ((SCREEN_WIDTH * (zoom + 1)) < LEVEL_WIDTH && (SCREEN_HEIGHT * (zoom + 1)) < LEVEL_HEIGHT && zoom < 4) 
		{
			// center the zoom out so that it goes to the center rather than the top-left on the screen
			xPos -= ((SCREEN_WIDTH * zoom) / 4) + (xPos / 2);
			yPos -= ((SCREEN_HEIGHT * zoom) / 4) + (yPos / 2);

			zoom *= 2;

			CheckAndAdjustOutOfBounds();
		}
	}
	else 
	{
		if (zoom >= 2)
		{
			zoom /= 2;

			// center the zoom in so that it goes to the center rather than the top-left on the screen
			xPos += ((SCREEN_WIDTH * zoom) / 4) + xPos;
			yPos += ((SCREEN_HEIGHT * zoom) / 4) + yPos;

			CheckAndAdjustOutOfBounds();
		}
	}
}

void Camera::CheckAndAdjustOutOfBounds() 
{
	if (xPos < 0) {
		xPos = 0;
	}

	if (yPos < 0) {
		yPos = 0;
	}

	if (xPos > (LEVEL_WIDTH / zoom) - SCREEN_WIDTH)
	{
		std::cout << "Camera X Gaurd Rail hit: " << xPos << " > (" << LEVEL_WIDTH << " / " << zoom << ") - " << SCREEN_WIDTH << std::endl;
		xPos = (LEVEL_WIDTH / zoom) - SCREEN_WIDTH;
	}

	if (yPos > (LEVEL_HEIGHT / zoom) - SCREEN_HEIGHT)
	{
		std::cout << "Camera Y Gaurd Rail hit: " << yPos << " > (" << LEVEL_HEIGHT << " / " << zoom << ") - " << SCREEN_HEIGHT << std::endl;
		yPos = (LEVEL_HEIGHT / zoom) - SCREEN_HEIGHT;
	}
}


