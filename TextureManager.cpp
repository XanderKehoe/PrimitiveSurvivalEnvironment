#include "TextureManager.h"
#include "Game.h"

// forward declaration (resolves linker error)
SDL_Texture* TextureManager::tilePlain;
SDL_Texture* TextureManager::tileBushBerry;
SDL_Texture* TextureManager::tileBushFiber;
SDL_Texture* TextureManager::tileRock;
SDL_Texture* TextureManager::tileRockDemolished;
SDL_Texture* TextureManager::tileTree;
SDL_Texture* TextureManager::tileTreeStump;
SDL_Texture* TextureManager::entityHuman;
SDL_Texture* TextureManager::entityHumanCrouch;
SDL_Texture* TextureManager::entityHumanWithSpear;
SDL_Texture* TextureManager::entityHumanCrouchWithSpear;
SDL_Texture* TextureManager::entityRabbit;
SDL_Texture* TextureManager::entityDeer;
SDL_Texture* TextureManager::entityElk;
SDL_Texture* TextureManager::entityGoose;
SDL_Texture* TextureManager::entityWolf;
SDL_Texture* TextureManager::entityBear;
SDL_Texture* TextureManager::debugTexture;
SDL_Texture* TextureManager::debugUp;
SDL_Texture* TextureManager::debugDown;
SDL_Texture* TextureManager::debugLeft;
SDL_Texture* TextureManager::debugRight;

void TextureManager::InitializeTextures(SDL_Renderer* ren)
{
	tilePlain = LoadTexture("Textures/GrassTilePlain.png", ren);
	tileBushBerry = LoadTexture("Textures/GrassTileWithBushBerry.png", ren);
	tileBushFiber = LoadTexture("Textures/GrassTileWithBushFiber.png", ren);
	tileRock = LoadTexture("Textures/GrassTileWithRock.png", ren);
	tileRockDemolished = LoadTexture("Textures/GrassTileWithRockDemolished.png", ren);
	tileTree = LoadTexture("Textures/GrassTileWithTree.png", ren);
	tileTreeStump = LoadTexture("Textures/GrassTileWithTreeStump.png", ren);
	entityHuman = LoadTexture("Textures/HumanAgentNoSpear.png", ren);
	entityHumanCrouch = LoadTexture("Textures/HumanAgentNoSpearCrouch.png", ren);
	entityHumanWithSpear = LoadTexture("Textures/HumanAgentStanding.png", ren);
	entityHumanCrouchWithSpear = LoadTexture("Textures/HumanAgentCrouching.png", ren);
	entityRabbit = LoadTexture("Textures/Rabbit.png", ren);
	entityDeer = LoadTexture("Textures/Deer.png", ren);
	entityElk = LoadTexture("Textures/Elk.png", ren);
	entityGoose = LoadTexture("Textures/Goose.png", ren);
	entityWolf = LoadTexture("Textures/Wolf.png", ren);
	entityBear = LoadTexture("Textures/Bear.png", ren);
	debugTexture = LoadTexture("Textures/Debug.png", ren);
	debugUp = LoadTexture("Textures/DebugUp.png", ren);
	debugDown = LoadTexture("Textures/DebugDown.png", ren);
	debugLeft = LoadTexture("Textures/DebugLeft.png", ren);
	debugRight = LoadTexture("Textures/DebugRight.png", ren);
}

SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer* ren)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
	SDL_FreeSurface(tempSurface);

	if (tex == NULL)
		throw std::logic_error("Texture did not successfully import.");

	return tex;
}

SDL_Texture* TextureManager::LoadTextureByType(TextureLoadType textureLoadType)
{
	switch (textureLoadType) 
	{
		case TextureLoadType::TILE_PLAIN:
		{
			return tilePlain;
		}
		case TextureLoadType::TILE_BUSH_BERRY:
		{
			return tileBushBerry;
		}
		case TextureLoadType::TILE_BUSH_FIBER:
		{
			return tileBushFiber;
		}
		case TextureLoadType::TILE_ROCK:
		{
			return tileRock;
		}
		case TextureLoadType::TILE_ROCK_DEMOLISHED:
		{
			return tileRockDemolished;
		}
		case TextureLoadType::TILE_TREE:
		{
			return tileTree;
		}
		case TextureLoadType::TILE_TREE_STUMP:
		{
			return tileTreeStump;
		}
		case TextureLoadType::ENTITY_HUMAN: 
		{
			return entityHuman;
		}
		case TextureLoadType::ENTITY_HUMAN_CROUCH: 
		{
			return entityHumanCrouch;
		}
		case TextureLoadType::ENTITY_HUMAN_WITH_SPEAR:
		{
			return entityHumanWithSpear;
		}
		case TextureLoadType::ENTITY_HUMAN_WITH_SPEAR_CROUCH:
		{
			return entityHumanCrouchWithSpear;
		}
		case TextureLoadType::ENTITY_RABBIT:
		{
			return entityRabbit;
		}
		case TextureLoadType::ENTITY_DEER:
		{
			return entityDeer;
		}
		case TextureLoadType::ENTITY_ELK:
		{
			return entityElk;
		}
		case TextureLoadType::ENTITY_GOOSE:
		{
			return entityGoose;
		}
		case TextureLoadType::ENTITY_WOLF:
		{
			return entityWolf;
		}
		case TextureLoadType::ENTITY_BEAR:
		{
			return entityBear;
		}
		case TextureLoadType::DEBUG: 
		{
			return debugTexture;
		}
		case TextureLoadType::DEBUG_UP:
		{
			return debugUp;
		}
		case TextureLoadType::DEBUG_DOWN:
		{
			return debugDown;
		}
		case TextureLoadType::DEBUG_LEFT:
		{
			return debugLeft;
		}
		case TextureLoadType::DEBUG_RIGHT:
		{
			return debugRight;
		}

		default:
			throw std::logic_error("Not Implemented");
	}
}