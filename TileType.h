#pragma once
enum class TileType 
{ 
	DNE = 0, // Used when collecting observations about tiles that do not exist (I.E. off the level)
	PLAIN,
	BUSH_BERRY,
	BUSH_FIBER,
	TREE,
	ROCK,
	CLIFF, 
	WATER, 
	WALL,
	DOOR,
};