#pragma once
#include <vector>
#include <queue>
#include "DirectionType.h"
#include "Config.h"
#include "Tile.h"

using namespace std;


class PathFinder
{
public:
	static vector<DirectionType> GetPath(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], int startX, int startY, int endX, int endY, bool isHuman = false);
	static int Dist(int x1, int y1, int x2, int y2);
private:
	static int HeuristicCostEstimate(int startX, int startY, int endX, int endY) { return abs(startX - endX) + abs(startY - endY);}
	static vector<DirectionType> RetracePath(Tile* level[Config::LEVEL_SIZE][Config::LEVEL_SIZE], DirectionType** cameFrom, Tile* currentTile, int startX, int startY);
	static DirectionType GetDirection(Tile* t1, Tile* t2);
	static DirectionType ReverseDirection(DirectionType dirType);
};

