#include "Game.h"
#include "HumanAgent.h"
#include "Tile.h"
#include "LevelGenerator.h"
#include "Camera.h"

SDL_Event Game::event;

HumanAgent* player1;


const bool ALLOW_PLAYER_CONTROL = true;

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

	player1 = new HumanAgent(TextureLoadType::ENTITY_HUMAN, renderer, 7, 7);
}

void Game::HandleEvents()
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

			if (ALLOW_PLAYER_CONTROL) 
			{
				switch (Game::event.key.keysym.sym)
				{
					case SDLK_DOWN:
					{
						player1->Move(DirectionType::DOWN, true, level);
						break;
					}
					case SDLK_UP:
					{
						player1->Move(DirectionType::UP, true, level);
						break;
					}
					case SDLK_LEFT:
					{
						player1->Move(DirectionType::LEFT, true, level);
						break;
					}
					case SDLK_RIGHT:
					{
						player1->Move(DirectionType::RIGHT, true, level);
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
			else { // scrolled up
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

void Game::Update() 
{
	player1->Update(level);

	for (unsigned int i = 0; i < Config::LEVEL_SIZE; ++i)
	{
		for (unsigned int j = 0; j < Config::LEVEL_SIZE; ++j)
		{
			level[i][j]->Update();
		}
	}
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

	player1->Render();

	SDL_RenderPresent(renderer);
}

void Game::Clean() 
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}