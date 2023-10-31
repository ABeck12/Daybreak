#include "dbpch.h"

#include "Daybreak/Math/Functions/CommonMathFunc.h"

namespace Daybreak::Math
{
	float Min(const float value1, const float value2)
	{
		return (value1 <= value2) ? value1 : value2;
	}

	float Max(const float value1, const float value2)
	{
		return (value1 >= value2) ? value1 : value2;
	}

	float Sign(const float value)
	{
		if (value == 0.0)
			return 0.0f;
		return (value > 0.0f) ? 1.0f : -1.0f;
	}

	float Abs(const float value)
	{
		return (value > 0.0f) ? value : -value;
	}


	

}