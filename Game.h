#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "Config.h"
#include "ActionType.h"
#include "UpdateResult.h"
//#include "GameObject.h"
//#include "HumanAgentBase.h" //DO NOT USE //except we kinda have to use it. // no we don't, put it in the .cpp file instead! (4 Hours of my life wasted)
//#include "Tile.h" // same here, put include in .cpp file and use forward declaration in header file (aka here)
#include <iostream>
#include <vector>
#include <list>

class Game
{
public:
	Game();
	~Game();

	class Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE]; // the 2d array containing all the tiles, creating the level. Also an example of forward declaration here.

	static SDL_Event event;

	void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	void HandleEvents(bool allowPlayerControl);
	UpdateResult Update(ActionType selectedAgentAction); // returns the reward from the agent.
	void Render();
	void Clean();

	bool Running() { return isRunning; }

	SDL_Renderer* GetRenderer() { return renderer; }
	std::vector<int> GetAgentObservationsInt();
	std::vector<float> GetAgentObservationsFloat();
private:
	unsigned short cameraSpeed = 4;
	bool isRunning = false;
	SDL_Window *window = nullptr;
	SDL_Renderer* renderer = nullptr;

	class HumanAgent* agent;
	std::list<class Animal*> animalList;

	void InitEntities();
};
