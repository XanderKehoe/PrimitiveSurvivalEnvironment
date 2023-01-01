#include <iostream>
#include "Game.h"
//#include "PythonInterface.h"

Game* game = nullptr;

int main(int argc, char *argv[])
{
	//PythonInterface::Initialize();
	
	//PythonInterface::CallPython("PythonFile", "InitSomething", NULL);
	//PythonInterface::CallPython("PythonFile", "PrintSomething", PythonInterface::GetTestArgs());

	const int FPS = 1;
	const int frameDelay = 1000 / FPS;
	bool training = false;

	Uint32 lastUpdateTime = SDL_GetTicks();
	int timeSinceLastUpdate;

	game = new Game();

	game->Init("Primitive Survival Environment", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, false);

	while (game->Running()) 
	{
		game->HandleEvents();
		
		if (!training)
		{
			timeSinceLastUpdate = SDL_GetTicks() - lastUpdateTime;

			if (timeSinceLastUpdate > FPS * 1000) 
			{
				game->Update();

				lastUpdateTime = SDL_GetTicks();
			}

		}
		else 
		{
			game->Update();
		}

		game->Render();
	}

	game->Clean();

	return 0;
}