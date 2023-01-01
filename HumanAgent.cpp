#include "HumanAgent.h"
#include "TileType.h"
#include "Tile.h"

#pragma warning(disable : 26451)
#pragma warning(disable : 6294)

HumanAgent::HumanAgent(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos)
    : HumanAgentBase(textureLoadType, ren, initXPos, initYPos)
{
}

HumanAgent::~HumanAgent()
{
}


std::vector<int> HumanAgent::GetObservations(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
    int numberOfObservationsPerTile = 2;
    std::vector<int>* ret = new std::vector<int>((OBS_VIEW_LENGTH * OBS_VIEW_LENGTH + 1) * numberOfObservationsPerTile);

    for (int i = -(OBS_VIEW_LENGTH / 2); i <= OBS_VIEW_LENGTH / 2; i++)
    {
        for (int j = -(OBS_VIEW_LENGTH / 2); j <= OBS_VIEW_LENGTH / 2; j++)
        {
            int tileType;
            int available;

            if (!GridPosOutOfBounds(gridXPos + i, gridYPos + j))
            {
                int tileGridXPos = gridXPos + i;
                int tileGridYPos = gridYPos + j;
                Tile* thisTile = level[tileGridXPos][tileGridYPos];

                tileType = static_cast<int>(thisTile->tileType);
                available = thisTile->available;

                //thisTile->tileTexture = TextureManager::LoadTexture("Textures/TempWaterTile.png", renderer); // DEBUG to visualize observation tiles
            }
            else // observation is out of bounds
            {
                tileType = static_cast<int>(TileType::DNE);
                available = 0;
            }

            // update numberOfObservationsPerTile if adding anymore information per tile in the future
            ret->push_back(tileType);
            ret->push_back(available);
        }
    }

    return *ret;
}

