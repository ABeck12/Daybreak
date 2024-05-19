#pragma once

#include "dbpch.h"

#include "Daybreak/Core/MouseCodes.h"
#include "Daybreak/Core/KeyCodes.h"
#include "Daybreak/Core/GamepadCodes.h"

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

		static std::string GetGamepadName(Gamepad gamepad);
		static bool IsGamepadConnected(Gamepad gamepad);
		static float GetGamepadAxis(Gamepad gamepad, GamepadAxis axis);
		static bool IsGamepadButtonPressed(Gamepad gamepad, GamepadButton button);
	};

}