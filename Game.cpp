#include <random>
#include "Game.h"
#include "HumanAgent.h"
#include "Animal.h"
#include "Tile.h"
#include "LevelGenerator.h"
#include "Camera.h"


SDL_Event Game::event;

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

			TextureManager::InitializeTextures(renderer);
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

	InitEntities();
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
						agent->TakeAction(ActionType::MOVE_DOWN, level, animalList);
						break;
					}
					case SDLK_UP:
					{
						agent->TakeAction(ActionType::MOVE_UP, level, animalList);
						break;
					}
					case SDLK_LEFT:
					{
						agent->TakeAction(ActionType::MOVE_LEFT, level, animalList);
						break;
					}
					case SDLK_RIGHT:
					{
						agent->TakeAction(ActionType::MOVE_RIGHT, level, animalList);
						break;
					}
					case SDLK_t:
					{
						agent->TakeAction(ActionType::INTERACT_UP, level, animalList);
						break;
					}
					case SDLK_f:
					{
						agent->TakeAction(ActionType::INTERACT_LEFT, level, animalList);
						break;
					}
					case SDLK_g:
					{
						agent->TakeAction(ActionType::INTERACT_DOWN, level, animalList);
						break;
					}
					case SDLK_h:
					{
						agent->TakeAction(ActionType::INTERACT_RIGHT, level, animalList);
						break;
					}
					case SDLK_1:
					{
						agent->TakeAction(ActionType::CRAFT_SPEAR, level, animalList);
						break;
					}
					case SDLK_2:
					{
						agent->TakeAction(ActionType::CRAFT_AXE, level, animalList);
						break;
					}
					case SDLK_3:
					{
						agent->TakeAction(ActionType::CRAFT_HAMMER, level, animalList);
						break;
					}
					case SDLK_4:
					{
						agent->TakeAction(ActionType::CRAFT_SACK, level, animalList);
						break;
					}
					case SDLK_5:
					{
						agent->TakeAction(ActionType::CRAFT_BOW, level, animalList);
						break;
					}
					case SDLK_6:
					{
						agent->TakeAction(ActionType::CRAFT_ARROW, level, animalList);
						break;
					}
					case SDLK_F1:
					{
						agent->TakeAction(ActionType::TOGGLE_SNEAK, level, animalList);
						break;
					}
					case SDLK_F2:
					{
						agent->TakeAction(ActionType::SHOOT_BOW, level, animalList);
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

UpdateResult Game::Update(ActionType selectedAgentAction) 
{	
	for (Animal* a : animalList) 
	{
		a->Update(level);
		
		//printf("Animal | xPos: %d yPos: %d\n", a.GetGridXPos(), a.GetGridYPos());
	}

	UpdateResult updateResult = agent->Update(level, selectedAgentAction, animalList);

	for (unsigned int i = 0; i < Config::LEVEL_SIZE; ++i)
	{
		for (unsigned int j = 0; j < Config::LEVEL_SIZE; ++j)
		{
			level[i][j]->Update();
		}
	}

	return updateResult;
}

std::vector<int> Game::GetAgentObservationsInt() 
{
	return agent->GetObservationsInt(level);
}

std::vector<float> Game::GetAgentObservationsFloat()
{
	return agent->GetObservationsFloat(level);
}

void Game::InitEntities()
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, Config::LEVEL_SIZE - 1); // define the range

	//agent = new HumanAgent(TextureLoadType::ENTITY_HUMAN, renderer, distr(gen), distr(gen));

	agent = new HumanAgent(TextureLoadType::ENTITY_HUMAN, renderer, Config::LEVEL_SIZE / 2, Config::LEVEL_SIZE / 2);
	Camera::Center(agent->GetGridXPos(), agent->GetGridYPos());
	
	//Animal* testAnimal = new Animal(TextureLoadType::ENTITY_RABBIT, renderer, 21, 20);
	//animalList.push_back(testAnimal);

	for (int i = 0; i < Config::NUM_RABBITS; i++) 
	{
		Animal* newAnimal = new Animal(TextureLoadType::ENTITY_RABBIT, renderer, distr(gen), distr(gen), agent);
		animalList.push_back(newAnimal);
	}

	for (int i = 0; i < Config::NUM_DEER; i++)
	{
		Animal* newAnimal = new Animal(TextureLoadType::ENTITY_DEER, renderer, distr(gen), distr(gen), agent);
		animalList.push_back(newAnimal);
	}

	for (int i = 0; i < Config::NUM_ELK; i++)
	{
		Animal* newAnimal = new Animal(TextureLoadType::ENTITY_ELK, renderer, distr(gen), distr(gen), agent);
		animalList.push_back(newAnimal);
	}

	for (int i = 0; i < Config::NUM_GOOSE; i++)
	{
		Animal* newAnimal = new Animal(TextureLoadType::ENTITY_GOOSE, renderer, distr(gen), distr(gen), agent);
		animalList.push_back(newAnimal);
	}

	for (int i = 0; i < Config::NUM_WOLF; i++)
	{
		Animal* newAnimal = new Animal(TextureLoadType::ENTITY_WOLF, renderer, distr(gen), distr(gen), agent);
		animalList.push_back(newAnimal);
	}

	for (int i = 0; i < Config::NUM_BEAR; i++)
	{
		Animal* newAnimal = new Animal(TextureLoadType::ENTITY_BEAR, renderer, distr(gen), distr(gen), agent);
		animalList.push_back(newAnimal);
	}

	Render();
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

	for (Animal* e : animalList)
	{
		e->Render();
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