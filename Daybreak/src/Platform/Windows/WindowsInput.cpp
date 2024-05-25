#include "dbpch.h"

#include "Daybreak/Core/Input.h"
#include "Daybreak/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Daybreak
{
	bool Input::IsKeyPressed(const KeyCode key)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

	std::string Input::GetGamepadName(Gamepad gamepad)
	{
		return glfwGetGamepadName((int)gamepad);
	}

	bool Input::IsGamepadConnected(Gamepad gamepad)
	{
		return (bool)glfwJoystickPresent((int)gamepad);
	}

	float Input::GetGamepadAxis(Gamepad gamepad, GamepadAxis axis)
	{
		if (!IsGamepadConnected(gamepad))
		{
			return 0.0f;
		}

		int count;
		const float* axes = glfwGetJoystickAxes((int)gamepad, &count);
		switch (axis)
		{
			case GamepadAxis::LeftY:
				return -axes[(int)axis];
			case GamepadAxis::RightY:
				return -axes[(int)axis];
			case GamepadAxis::LeftX:
				return axes[(int)axis];
			case GamepadAxis::RightX:
				return axes[(int)axis];
			case GamepadAxis::RightTrigger:
				return (1 + axes[(int)axis]) / 2;
			case GamepadAxis::LeftTrigger:
				return (1 + axes[(int)axis]) / 2;
		}
		DB_CORE_ERROR("Unknown gamepad axis {}", (int)axis);
		return 0.0f;
	}

	bool Input::IsGamepadButtonPressed(Gamepad gamepad, GamepadButton button)
	{
		if (IsGamepadConnected(gamepad))
		{
			int buttonCount;
			const unsigned char* buttons = glfwGetJoystickButtons((int)gamepad, &buttonCount);
			return buttons[(int)button] == GLFW_PRESS;
		}
		return false;
	}
}