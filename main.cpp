#include <iostream>
#include "Game.h"
#include "SharedMemoryManager.h"
//#include "PythonInterface.h"

//TODO
	
	// Water and Cliff Generation
	// Water, Cliff, Wall, and Door tile display 
	// Crafting
	// Animals
	// Health, hunger, thirst, weariness
	// Multiple policy client and environments at the same time

Game* game = nullptr;
const bool TRAINING = false;

int main(int argc, char *argv[])
{
	// According to ChatGPT (and testing confirms), shared memory needs to be declared before static classes used I guess?
	// So we'll probably keep this segment and pass the references of the shared memory to the SharedMemoryManager
	windows_shared_memory smh(open_or_create, "PSEMem", read_write, 1000u);
	mapped_region region(smh, read_write);
	SharedMemoryManager::Init(&smh, &region);

	/*// Test values for shared memory, should be able to see these values in python.
	for (float i = 0; i < (region.get_size() / 4); i++)
		SharedMemoryManager::AddFloatToBuffer(i / 3);

	SharedMemoryManager::SetAvailability(7);*/

	const int FPS = 1;
	const int frameDelay = 1000 / FPS;
	bool training = false;

	Uint32 lastUpdateTime = SDL_GetTicks();
	int timeSinceLastUpdate;

	game = new Game();

	game->Init("Primitive Survival Environment", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, false);
	game->Render(); // Go ahead and render once for visual que that the game has been initialized

	ActionType selectedAction = ActionType::NOTHING;

	while (game->Running()) 
	{
		game->HandleEvents(!TRAINING);

		timeSinceLastUpdate = SDL_GetTicks() - lastUpdateTime;

		if (TRAINING) 
		{
			UpdateResult updateResult = game->Update(selectedAction);

			std::vector<int> newAgentState = game->GetAgentObservations();

			SharedMemoryManager::SendStateRewardDone(updateResult.reward, updateResult.done, newAgentState);

			SharedMemoryManager::WaitForAvailability();

			selectedAction = SharedMemoryManager::ReadSelectedAction();
			//printf("selectedAction received: %d\n", selectedAction);
			SharedMemoryManager::ClearBuffer();
		}
		else if (timeSinceLastUpdate > FPS * 1000) 
		{
			game->Update(selectedAction);

			lastUpdateTime = SDL_GetTicks();
		}

		/*}
		else 
		{
			game->Update(SharedMemoryManager::ReadSelectedAction());
		}*/

		game->Render();
	}

	game->Clean();

	return 0;
}