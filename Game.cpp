#include "Game.h"
#include "HumanAgent.h"
#include "Tile.h"
#include "LevelGenerator.h"
#include "Camera.h"

SDL_Event Game::event;

HumanAgent* agent;

Game::Game()
{

}

Game::~Game() 
{
	// TODO
}

void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) 
{
	int flags = 0;
	if (fullscreen) 
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialied!..." << std::endl;

		Camera::Init(width, height);

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
			std::cout << "Window created!" << std::endl;
		else
			std::cout << "FAILED to create window" << std::endl;

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			std::cout << "Renderer created!" << std::endl;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			LevelGenerator::GenerateLevel(level, renderer);
		}
		else
			std::cout << "FAILED to create Renderer" << std::endl;

		isRunning = true;
	}
	else 
	{
		isRunning = false;
	}

	agent = new HumanAgent(TextureLoadType::ENTITY_HUMAN, renderer, 7, 7);
}

void Game::HandleEvents(bool allowPlayerControl)
{
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
		{
			isRunning = false;
			break;
		}

		case SDL_KEYDOWN: 
		{
			switch (Game::event.key.keysym.sym) 
			{
				case SDLK_w: 
				{
					//std::cout << "W" << std::endl;
					Camera::Move(0, -1 * cameraSpeed);
					break;
				}
				case SDLK_a:
				{
					//std::cout << "A" << std::endl;
					Camera::Move(-1 * cameraSpeed, 0);
					break;
				}
				case SDLK_s:
				{
					//std::cout << "S" << std::endl;
					Camera::Move(0, 1 * cameraSpeed);
					break;
				}
				case SDLK_d:
				{
					//std::cout << "D" << std::endl;
					Camera::Move(1 * cameraSpeed, 0);
					break;
				}
				case SDLK_j:
				{
					Camera::AdjustZoom(true);

					std::cout << "Zoom: " << Camera::zoom << std::endl;

					break;
				}
				case SDLK_k: 
				{
					Camera::AdjustZoom(false);

					std::cout << "Zoom: " << Camera::zoom << std::endl;

					break;
				}
			}

			if (allowPlayerControl)
			{
				switch (Game::event.key.keysym.sym)
				{
					case SDLK_DOWN:
					{
						agent->TakeAction(ActionType::MOVE_DOWN, level);
						break;
					}
					case SDLK_UP:
					{
						agent->TakeAction(ActionType::MOVE_UP, level);
						break;
					}
					case SDLK_LEFT:
					{
						agent->TakeAction(ActionType::MOVE_LEFT, level);
						break;
					}
					case SDLK_RIGHT:
					{
						agent->TakeAction(ActionType::MOVE_RIGHT, level);
						break;
					}
					case SDLK_i:
					{
						agent->TakeAction(ActionType::INTERACT_UP, level);
						break;
					}
					case SDLK_j:
					{
						agent->TakeAction(ActionType::INTERACT_LEFT, level);
						break;
					}
					case SDLK_k:
					{
						agent->TakeAction(ActionType::INTERACT_DOWN, level);
						break;
					}
					case SDLK_l:
					{
						agent->TakeAction(ActionType::INTERACT_RIGHT, level);
						break;
					}
				}
			}
			break;
		}

		case SDL_MOUSEWHEEL:
		{
			// control camera speed
			if (Game::event.wheel.y < 0) // scrolled down
			{
				cameraSpeed /= 2;
			}
			else // scrolled up
			{ 
				if (cameraSpeed < 128) // prevent overflow
					cameraSpeed *= 2;
			}

			if (cameraSpeed < 1)
				cameraSpeed = 1;

			break;
		}

		default:
			break;
	}
}

float Game::Update(ActionType selectedAgentAction) 
{
	// In future, update all other entities first before main agent (so hostile entites have a chance to damage agent)

	float reward = agent->Update(level, selectedAgentAction);

	for (unsigned int i = 0; i < Config::LEVEL_SIZE; ++i)
	{
		for (unsigned int j = 0; j < Config::LEVEL_SIZE; ++j)
		{
			level[i][j]->Update();
		}
	}

	return reward;
}

std::vector<int> Game::GetAgentObservations() 
{
	return agent->GetObservations(level);
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	// add stuff to render here

	for (unsigned int i = 0; i < Config::LEVEL_SIZE; ++i)
	{
		for (unsigned int j = 0; j < Config::LEVEL_SIZE; ++j)
		{
			level[i][j]->Render();
		}
	}

	agent->Render();

	SDL_RenderPresent(renderer);
}

void Game::Clean() 
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}