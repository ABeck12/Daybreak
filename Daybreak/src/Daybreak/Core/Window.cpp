#include "dbpch.h"

#include "Daybreak/Core/Window.h"

#ifdef DB_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Daybreak
{
	#ifdef DB_PLATFORM_WINDOWS
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
	#endif
}