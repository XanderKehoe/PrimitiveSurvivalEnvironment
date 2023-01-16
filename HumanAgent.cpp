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
    int numberOfNonTileObservations = 12;
    int numberOfObservationsPerTile = 1;
    // Preprocessing these observations here as its simpler to do it here
    std::vector<float> ret(numberOfNonTileObservations + inventory->GetItems().size() + ((((OBS_VIEW_LENGTH * 2) + 1) * ((OBS_VIEW_LENGTH * 2) + 1)) * numberOfObservationsPerTile));
    ret.at(0) = health / MAX_HEALTH; // technically already included in the tile float observations?
    ret.at(1) = hunger / MAX_HUNGER;
    ret.at(2) = thirst / MAX_THIRST;
    ret.at(3) = weariness / MAX_WEARINESS;
    ret.at(4) = (float) currentBowCooldown / (float) BOW_COOLDOWN_MAX;
    ret.at(5) = (float) (gridXPos) / (float) Config::LEVEL_SIZE;
    ret.at(6) = (float) (gridYPos) / (float) Config::LEVEL_SIZE;
    ret.at(7) = (float) startVisibilityCount / (float) START_VISIBILITY;
    ret.at(8) = (float) sneaking;
    ret.at(9) = (float) hearAnimal;
    ret.at(10) = (float) hearHostileAnimal;
    ret.at(11) = (float) hearPassiveAnimal;
    
    int index = numberOfNonTileObservations;

    // get inventory observations
    std::map<ItemType, Item*> items = inventory->GetItems();
    std::map<ItemType, Item*>::iterator it;

    for (it = items.begin(); it != items.end(); it++)
    {
        ret.at(index) = (float) it->second->currentAmount / (float) it->second->maxAmountSackUpgrade;
        index++;
    }

    // get tile observations
    for (int i = -(OBS_VIEW_LENGTH); i <= OBS_VIEW_LENGTH; i++)
    {
        for (int j = -(OBS_VIEW_LENGTH); j <= OBS_VIEW_LENGTH; j++)
        {
            float entityHealth;

            if (!GridPosOutOfBounds(gridXPos + i, gridYPos + j))
            {
                int tileGridXPos = gridXPos + i;
                int tileGridYPos = gridYPos + j;
                Tile* thisTile = level[tileGridXPos][tileGridYPos];

                if (thisTile->attachedEntity != nullptr) 
                {
                    entityHealth = thisTile->attachedEntity->GetCurrentHealth() / thisTile->attachedEntity->GetMaxHealth();
                }
                else
                {
                    entityHealth = 0; // no entity exists
                }
            }
            else // observation is out of bounds
            {
                entityHealth = 0;
            }

            ret.at(index) = entityHealth;
            index += numberOfObservationsPerTile;
        }
    }

    return ret;
}


std::vector<int> HumanAgent::GetObservationsInt(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE])
{
    int numberOfObservationsPerTile = 3;
    std::vector<int> ret((((OBS_VIEW_LENGTH * 2) + 1) * ((OBS_VIEW_LENGTH * 2) + 1)) * numberOfObservationsPerTile);

    bool debug = false;
    int index = 0;

    for (int i = -(OBS_VIEW_LENGTH ); i <= OBS_VIEW_LENGTH; i++)
    {
        for (int j = -(OBS_VIEW_LENGTH); j <= OBS_VIEW_LENGTH; j++)
        {
            int tileType;
            int available;
            int entityType;

            //printf("int obs ij [%d][%d]\n", i, j);
            if (!GridPosOutOfBounds(gridXPos + i, gridYPos + j))
            {
                int tileGridXPos = gridXPos + i;
                int tileGridYPos = gridYPos + j;
                Tile* thisTile = level[tileGridXPos][tileGridYPos];

                tileType = static_cast<int>(thisTile->tileType);
                available = thisTile->available;

                if (thisTile->attachedEntity == nullptr || (i == 0 && j == 0)) 
                {
                    entityType = 0;
                }
                else 
                {
                    entityType = thisTile->attachedEntity->GetEntityType();
                }

                //thisTile->tileTexture = TextureManager::LoadTexture("Textures/TempWaterTile.png", renderer); // DEBUG to visualize observation tiles
                //printf("\t tile was NOT out of bounds\n");
            }
            else // observation is out of bounds
            {
                tileType = static_cast<int>(TileType::DNE);
                available = 0;
                entityType = 0;
                //printf("\t tile was out of bounds.\n");
            }

            // update numberOfObservationsPerTile if adding anymore information per tile in the future
            ret.at(index) = tileType; //printf("\t\ttileType: %d\n", tileType);
            ret.at(index + 1) = available; //printf("\t\tvailable: %d\n", available);
            ret.at(index + 2) = entityType; //printf("\t\tentityType: %d\n", entityType);

            index += numberOfObservationsPerTile;

            /*
            printf("\n");
            for (int val : ret)
                printf("val: %d\n", val);
            printf("\n");
            */
        }
    }

    if (debug)
        printf("Count: %d size(): %d\n", index, ret.size()); // should be (OBS_VIEW_LENGTH + 1)^2

    return ret;
}

