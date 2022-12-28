#include "Game.h"
#include "HumanAgentBase.h"
#include "Tile.h"

GameObject* player1;

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

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
			std::cout << "Window created!" << std::endl;
		else
			std::cout << "FAILED to create window" << std::endl;

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			std::cout << "Renderer created!" << std::endl;
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			LoadTextures();
		}
		else
			std::cout << "FAILED to create Renderer" << std::endl;

		isRunning = true;
	}
	else 
	{
		isRunning = false;
	}

	player1 = new HumanAgentBase("Textures/TempHuman.png", renderer, 5, 5);
}

void Game::HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;

		default:
			break;
	}
}

void Game::Update() 
{
	player1->Update();
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	// add stuff to render here
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

void Game::LoadTextures() 
{
	/*
	Tile::textureSheetBushBerry = TextureManager::LoadTexture("Textures/TempBushBerry.png", renderer);
	Tile::textureSheetBushFiber = TextureManager::LoadTexture("Textures/TempBushFiber.png", renderer);
	Tile::textureSheetRock = TextureManager::LoadTexture("Textures/TempGrassTileWithBoulder.png", renderer);
	Tile::textureSheetTree = TextureManager::LoadTexture("Textures/TempGrassTileWithTree.png", renderer);
	*/
}