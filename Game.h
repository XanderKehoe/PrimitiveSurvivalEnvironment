#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "Config.h"
//#include "GameObject.h"
//#include "HumanAgentBase.h" //DO NOT USE //except we kinda have to use it. // no we don't, put it in the .cpp file instead! (4 Hours of my life wasted)
//#include "Tile.h" // same here, put include in .cpp file and use forward declaration in header file (aka here)
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	class Tile* map[Config::MAP_SIZE][Config::MAP_SIZE]; // the 2d array containing all the tiles, creating the map. Also an example of forward declaration here.

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
