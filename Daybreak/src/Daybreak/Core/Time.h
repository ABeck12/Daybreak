#pragma once
#include <chrono>

#include "Daybreak/Core/DeltaTime.h"

namespace Daybreak
{
	class Time
	{
	public:
		// const DeltaTime FixedDeltaTime = 0.016;
		static float GetTime();
	};
}