#pragma once
#include <chrono>

#include "Daybreak/Core/DeltaTime.h"

#define DB_TIME_SCOPE(name) Daybreak::ScopeTimer timer = Daybreak::ScopeTimer(name);

namespace Daybreak
{
	class Time
	{
	public:
		static double GetTime();
	};

	class ScopeTimer
	{
	public:
		ScopeTimer() = default;
		ScopeTimer(const std::string& name);
		~ScopeTimer();

	private:
		std::string m_Name;
		double m_StartTime;
	};
}