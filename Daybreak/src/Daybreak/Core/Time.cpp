#include "dbpch.h"

#include "Daybreak/Core/Time.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h" // FIXME: Temporary

namespace Daybreak
{
	double Time::GetTime()
	{
		return glfwGetTime();
	}

	ScopeTimer::ScopeTimer(const std::string& name)
		: m_Name(name)
	{
		m_StartTime = Time::GetTime();
	}

	ScopeTimer::~ScopeTimer()
	{
		DB_LOG("{} duration = {:.3f}ms", m_Name, (Time::GetTime() - m_StartTime) * 1000);
	}
}