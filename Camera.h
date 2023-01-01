#pragma once
#include "SDL.h"
class Camera
{
public:
	static long int xPos; // current x coordinate position of the camera
	static long int yPos; // current x coordinate position of the camera

	static unsigned short zoom; // Determines how far out the camera is zoomed, higher values means camera shows more tiles.

	static void Init(int screenWidth, int screenHeight); // Initializes the camera, must be called before any other use of the camera.
	static void Move(int x, int y); // Moves the camera by the given input amount

	static int SCREEN_WIDTH; // The width of the window the environment is displayed on
	static int SCREEN_HEIGHT; // The height of the window the environment is displayed on

	static int LEVEL_WIDTH; // the width of the entire level (tile size * level size)
	static int LEVEL_HEIGHT; // the height of the entire level (tile size * level size)

	/*
	Adjusts the camera's zoom and sets boundaries to avoid buggy behavior.
	If increase == true, zoom *= 2
	If increase == false, zoom /= 2
	*/
	static void AdjustZoom(bool increase);
private:
	static void CheckAndAdjustOutOfBounds(); // Checks if camera is out of bounds by looking at its current coordinates and zoom, and readjust if necessary.
};

