#pragma once

#include <array>

enum class DirectionType 
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
};

class DirectionTypeConverter 
{
public:
	static std::array<int, 2> TypeToXY(DirectionType dirType);
};