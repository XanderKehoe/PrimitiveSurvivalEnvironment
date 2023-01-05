#pragma once

#include <array>

enum class DirectionType 
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

class DirectionTypeConverter 
{
public:
	static std::array<int, 2> TypeToXY(DirectionType dirType);
};