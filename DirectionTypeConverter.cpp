#include "DirectionType.h"
#include <stdexcept>

int* DirectionTypeConverter::TypeToXY(DirectionType dirType)
{
	int ret[2];
	switch (dirType)
	{
		case DirectionType::UP:
		{
			ret[0] = 0;
			ret[1] = -1;
			break;
		}

		case DirectionType::DOWN:
		{
			ret[0] = 0;
			ret[1] = 1;
			break;
		}

		case DirectionType::LEFT:
		{
			ret[0] = -1;
			ret[1] = 0;
			break;
		}

		case DirectionType::RIGHT:
		{
			ret[0] = 1;
			ret[1] = 0;
			break;
		}

		default:
			throw std::logic_error("Undefined DirectionType");
	}

	return ret;
}