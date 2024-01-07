#pragma once

#include "dbpch.h"

#include "Daybreak/Core/MouseCodes.h"
#include "Daybreak/Core/KeyCodes.h"

#include <glm/glm.hpp>

namespace Daybreak
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}