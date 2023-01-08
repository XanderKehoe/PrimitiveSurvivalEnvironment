#include "PathFinder.h"

vector<DirectionType> PathFinder::GetPath(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], int startX, int startY, int endX, int endY, bool isHuman)
{
    int** gScore = new int* [Config::LEVEL_SIZE];
    for (int i = 0; i < Config::LEVEL_SIZE; i++)
    {
        gScore[i] = new int[Config::LEVEL_SIZE];
    }
    int** fScore = new int* [Config::LEVEL_SIZE];
    for (int i = 0; i < Config::LEVEL_SIZE; i++)
    {
        fScore[i] = new int[Config::LEVEL_SIZE];
    }
    DirectionType** cameFrom = new DirectionType * [Config::LEVEL_SIZE];
    for (int i = 0; i < Config::LEVEL_SIZE; i++)
    {
        cameFrom[i] = new DirectionType[Config::LEVEL_SIZE];
    }

    // Set all gScores to max value, set all fScores to max value
    for (int i = 0; i < Config::LEVEL_SIZE; i++)
    {
        for (int j = 0; j < Config::LEVEL_SIZE; j++)
        {
            gScore[i][j] = INT_MAX;
            fScore[i][j] = INT_MAX;
            cameFrom[i][j] = DirectionType::UP; // initializing
            level[i][j]->visited = false; // reset
        }
    }

    // Set the gScore of the start position to 0
    gScore[startX][startY] = 0;

    // Set the fScore of the start position to the distance to the end position
    fScore[startX][startY] = HeuristicCostEstimate(startX, startY, endX, endY);

    // Create a min_heap to store the tiles that are being evaluated
    auto cmp = [&](Tile* t1, Tile* t2) { return fScore[t1->GetGridXPos()][t1->GetGridYPos()] > fScore[t2->GetGridXPos()][t2->GetGridYPos()]; };
    priority_queue<Tile*, vector<Tile*>, decltype(cmp)> minHeap(cmp);

    // Add the start position to the min_heap
    minHeap.push(level[startX][startY]);

    // Counter for max search limit, helps avoid searching the entire map when a path obviously should have been found (map is fairly open, not a maze)
    int limitCount = 0;

    // While the min_heap is not empty
    while (!minHeap.empty())
    {
        // Get the tile with the lowest fScore from the min_heap
        Tile* currentTile = minHeap.top();
        minHeap.pop();
        limitCount++;

        // Mark the current tile as visited
        currentTile->visited = true;

        // If the current tile is the end position, then we have found a path
        if (currentTile->GetGridXPos() == endX && currentTile->GetGridYPos() == endY)
        {
            // Cleanup
            for (int i = 0; i < Config::LEVEL_SIZE; i++)
            {
                delete[] gScore[i];
            }
            delete[] gScore;
            for (int i = 0; i < Config::LEVEL_SIZE; i++)
            {
                delete[] fScore[i];
            }
            delete[] fScore;

            // Retrace the path and return it
            return RetracePath(level, cameFrom, level[endX][endY], startX, startY);
        }

        // If we've checked more than (straight path)^2.5, we're probably never going to find a valid path.
        // Better to save computational resources.
        if (limitCount > pow((abs(endX - startX) + abs(endY - startY)), 2.5))
            break;

        // Check the tiles surrounding the current tile
        for (int i = 0; i < 4; i++) // iterative all 4 direction types (UP, DOWN, LEFT, RIGHT)
        {
            std::array<int, 2> ij = DirectionTypeConverter::TypeToXY(static_cast<DirectionType>(i));

            // Get the x and y coordinates of the surrounding tile
            int x = currentTile->GetGridXPos() + ij[0];
            int y = currentTile->GetGridYPos() + ij[1];

            // Check if the surrounding tile is out of bounds or not walkable
            if (x < 0 || x >= Config::LEVEL_SIZE || y < 0 || y >= Config::LEVEL_SIZE || !level[x][y]->IsWalkable(isHuman))
            {
                continue;
            }

            // Calculate the tentative gScore for the surrounding tile
            int tentativeGScore = gScore[currentTile->GetGridXPos()][currentTile->GetGridYPos()] 
                + Dist(currentTile->GetGridXPos(), currentTile->GetGridYPos(), level[x][y]->GetGridXPos(), level[x][y]->GetGridYPos());

            // Check if the tentative gScore is better than the current gScore for the surrounding tile
            if (tentativeGScore < gScore[x][y])
            {
                // Update the cameFrom array to store the direction we came from
                if (x < currentTile->GetGridXPos())
                {
                    cameFrom[x][y] = DirectionType::RIGHT;
                }
                else if (x > currentTile->GetGridXPos())
                {
                    cameFrom[x][y] = DirectionType::LEFT;
                }
                else if (y < currentTile->GetGridYPos())
                {
                    cameFrom[x][y] = DirectionType::DOWN;
                }
                else if (y > currentTile->GetGridYPos())
                {
                    cameFrom[x][y] = DirectionType::UP;
                }

                //printf("\tcameFrom[x][y] == %d\n", cameFrom[x][y]);

                // Update the gScore and fScore for the surrounding tile
                gScore[x][y] = tentativeGScore;
                fScore[x][y] = gScore[x][y] + HeuristicCostEstimate(x, y, endX, endY);

                // Add the surrounding tile to the min_heap
                minHeap.push(level[x][y]);
            }
        }
    }

    // If we reach here, then no path was found (can check with retVec.empty())
    return vector<DirectionType>();
}

vector<DirectionType> PathFinder::RetracePath(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], DirectionType** cameFrom, Tile* endTile, int startX, int startY)
{
    // Create a vector to store the path
    vector<DirectionType> path;

    // Set the current tile to the end tile
    Tile* currentTile = endTile;

    // While the current tile is not the start tile
    while (currentTile->GetGridXPos() != startX || currentTile->GetGridYPos() != startY)
    {
        //printf("xy: %d %d\n", currentTile->GetGridXPos(), currentTile->GetGridYPos());
        // Insert the direction we came from at the beginning of the path
        DirectionType revDir = ReverseDirection(cameFrom[currentTile->GetGridXPos()][currentTile->GetGridYPos()]);
        currentTile->cameFrom = revDir;
        path.push_back(revDir);

        // Set the current tile to the tile we came from
        switch (cameFrom[currentTile->GetGridXPos()][currentTile->GetGridYPos()])
        {
            case DirectionType::UP:
                currentTile = level[currentTile->GetGridXPos()][currentTile->GetGridYPos() - 1];
                break;
            case DirectionType::DOWN:
                currentTile = level[currentTile->GetGridXPos()][currentTile->GetGridYPos() + 1];
                break;
            case DirectionType::LEFT:
                currentTile = level[currentTile->GetGridXPos() - 1][currentTile->GetGridYPos()];
                break;
            case DirectionType::RIGHT:
                currentTile = level[currentTile->GetGridXPos() + 1][currentTile->GetGridYPos()];
                break;
        }
        //printf("\tcameFrom: %d\n", cameFrom[currentTile->GetGridXPos()][currentTile->GetGridYPos()]);
    }

    for (int i = 0; i < Config::LEVEL_SIZE; i++)
    {
        delete[] cameFrom[i];
    }
    delete[] cameFrom;

    // Return the path
    return path;
}

int PathFinder::Dist(int x1, int y1, int x2, int y2)
{
    // Calculate the distance between the tiles using the Pythagorean theorem
    int dx = x2 - x1;
    int dy = y2 - y1;
    return (int) sqrt(dx * dx + dy * dy);
}

DirectionType PathFinder::GetDirection(Tile* t1, Tile* t2)
{
    // Calculate the x and y differences between the tiles
    int xDiff = t2->GetGridXPos() - t1->GetGridXPos();
    int yDiff = t2->GetGridYPos() - t1->GetGridYPos();

    // Return the direction based on the x and y differences
    if (xDiff == 1)
    {
        return DirectionType::RIGHT;
    }
    else if (xDiff == -1)
    {
        return DirectionType::LEFT;
    }
    else if (yDiff == 1)
    {
        return DirectionType::DOWN;
    }
    else if (yDiff == -1)
    {
        return DirectionType::UP;
    }

    throw std::logic_error("Unknown direction type");
}

DirectionType PathFinder::ReverseDirection(DirectionType dirType) 
{
    switch (dirType) 
    {
        case DirectionType::UP:
            return DirectionType::DOWN;
        case DirectionType::DOWN:
            return DirectionType::UP;
        case DirectionType::LEFT:
            return DirectionType::RIGHT;
        case DirectionType::RIGHT:
            return DirectionType::LEFT;
    }

    throw std::logic_error("Unknown direction type");
}
