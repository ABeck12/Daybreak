#pragma once

#include "dbpch.h"

namespace Daybreak
{
	class Renderer
	{
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		static void Init();
		static void Shutdown();
	};
}