#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
//#include "GameObject.h"
//#include "HumanAgentBase.h" //DO NOT USE //except we kinda have to use it. // no we don't, put it in the .cpp file instead! (4 Hours of my life wasted)
//#include "Tile.h" // same here, put include in .cpp file and use forward declaration in header file (aka here)
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	const static unsigned int TILE_SIZE = 64;
	const static unsigned int MAP_SIZE = 128; // MAP_SIZE x MAP_SIZE will be the total size of the map in tiles.
	class Tile* map[MAP_SIZE][MAP_SIZE]; // forward declaration example here

	static SDL_Event event;

	void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool Running() { return isRunning; }

	SDL_Renderer* GetRenderer() { return renderer; }
private:
	int cnt = 0;
	bool isRunning = false;
	SDL_Window *window = nullptr;
	SDL_Renderer* renderer = nullptr;
};
