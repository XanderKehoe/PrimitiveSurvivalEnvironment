#pragma once
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
	static int* TypeToXY(DirectionType dirType);
};