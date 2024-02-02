#include "dbpch.h"

#include "Daybreak/Core/Time.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h" // FIXME: Temporary

namespace Daybreak
{
	float Time::GetTime() { return (float)glfwGetTime(); }
}