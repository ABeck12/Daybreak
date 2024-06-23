#include "dbpch.h"

#include "Daybreak/Core/Window.h"

#ifdef DB_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Daybreak
{
	#ifdef DB_PLATFORM_WINDOWS
	Scope<Window> Window::Create(const WindowSpecifications& props)
	{
		return CreateScope<WindowsWindow>(props);
	}
	#endif
}