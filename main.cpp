#include <iostream>
#include "Game.h"
#include "SharedMemoryManager.h"
#include "UpdateResult.h"
//#include "PythonInterface.h"

Game* game = nullptr;
const bool TRAINING = true;

// TODO
	// Verify one-hot encoding in obs works, (saw some 4, 5 and 7 values)
	// 'Querying server for new policy weights' occasionally occurs when receiving action or logging return to server, seems to be time based.
		// it is our biggest bottleneck currently, find out if we can adjust this time.
	// Implement new observations (and then test!)
	// Implement traps
	// Implement water and cliff generation
	// Implement water, cliff, and wall texture loading

int main(int argc, char *argv[])
{
	// According to ChatGPT (and testing confirms), shared memory needs to be declared before static classes used I guess?
	// So we'll probably keep this segment and pass the references of the shared memory to the SharedMemoryManager
	windows_shared_memory smh(open_or_create, "PSEMem", read_write, 1000u);
	mapped_region region(smh, read_write);
	SharedMemoryManager::Init(&smh, &region);

	const int FPS = 1;
	const int frameDelay = 1000 / FPS;

	Uint32 lastUpdateTime = SDL_GetTicks();
	int timeSinceLastUpdate;

	game = new Game();

	game->Init("Primitive Survival Environment", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 1000, false);
	game->Render(); // Go ahead and render once for visual que that the game has been initialized

	ActionType selectedAction = ActionType::NOTHING;

	while (game->Running()) 
	{
		game->HandleEvents(!TRAINING);

		timeSinceLastUpdate = SDL_GetTicks() - lastUpdateTime;

		if (TRAINING) 
		{
			UpdateResult updateResult = game->Update(selectedAction);

			std::vector<int> agentNewStateInt = game->GetAgentObservationsInt();
			std::vector<float> agentNewStateFloat = game->GetAgentObservationsFloat();

			SharedMemoryManager::SendStateRewardDone(updateResult.reward, updateResult.done, agentNewStateInt, agentNewStateFloat);
			//SharedMemoryManager::SendTestData();

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

		game->Render();		
	}

	game->Clean();

	return 0;
}