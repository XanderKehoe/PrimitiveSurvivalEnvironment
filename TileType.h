#pragma once
enum class TileType 
{ 
	DNE = 0, // Used when collecting observations about tiles that do not exist (I.E. off the level)
	PLAIN, 
	CLIFF, 
	WATER, 
	BUSH_BERRY, 
	BUSH_FIBER, 
	TREE, 
	ROCK, 
	DOOR, 
	WALL 
};