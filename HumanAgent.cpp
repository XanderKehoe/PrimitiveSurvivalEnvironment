#include "HumanAgent.h"
#include "TileType.h"
#include "Tile.h"

#pragma warning(disable : 26451)
#pragma warning(disable : 6294)

HumanAgent::HumanAgent(TextureLoadType textureLoadType, SDL_Renderer* ren, unsigned long int initXPos, unsigned long int initYPos)
    : HumanAgentBase(textureLoadType, ren, initXPos, initYPos)
{
    //moveTimerMax = 3;
}

HumanAgent::~HumanAgent()
{
}

std::vector<float> HumanAgent::GetObservationsFloat(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
    // Preprocessing these observations here as its simpler to do it here
    std::vector<float> ret(5 + inventory->GetItems().size());
    ret.at(0) = health / MAX_HEALTH;
    ret.at(1) = hunger / MAX_HUNGER;
    ret.at(2) = thirst / MAX_THIRST;
    ret.at(3) = weariness / MAX_WEARINESS;
    ret.at(4) = (float) moveCurrentCooldown / (float) moveTimerMax;
    
    int index = 5;

    std::map<ItemType, Item*>::iterator it;


    for (it = inventory->GetItems().begin(); it != inventory->GetItems().end(); it++)
    {
        ret.at(index) = (float) it->second->currentAmount / (float) it->second->maxAmountSackUpgrade;
        index++;
    }

    return ret;
}


std::vector<int> HumanAgent::GetObservationsInt(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
    int numberOfObservationsPerTile = 2;
    std::vector<int> ret(((OBS_VIEW_LENGTH + 1) * (OBS_VIEW_LENGTH + 1)) * numberOfObservationsPerTile);

    bool debug = false;
    int index = 0;

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
            ret.at(index) = tileType;
            ret.at(index + 1) = available;

            index += 2;
        }
    }

    if (debug)
        printf("Count: %d size(): %d\n", index, ret.size()); // should be (OBS_VIEW_LENGTH + 1)^2

    return ret;
}

