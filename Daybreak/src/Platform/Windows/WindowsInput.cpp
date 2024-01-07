#include "dbpch.h"

//#include "Platform/Windows/WindowsInput.h"
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

	//Input* Input::s_Instance = new WindowsInput();

	//bool WindowsInput::IsKeyPressedImpl(int keycode)
	//{
	//	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	//	auto state = glfwGetKey(window, keycode);
	//	return state == GLFW_PRESS || state == GLFW_REPEAT;
	//}

	//bool WindowsInput::IsMouseButtonPressedImpl(int button)
	//{
	//	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	//	auto state = glfwGetMouseButton(window, button);
	//	return state == GLFW_PRESS;
	//}

	//std::pair<float, float> WindowsInput::GetMousePositionImpl()
	//{
	//	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	//	double xpos, ypos;
	//	glfwGetCursorPos(window, &xpos, &ypos);

	//	return { (float)xpos, (float)ypos };
	//}

	//float WindowsInput::GetMouseXImpl()
	//{
	//	auto [x, y] = GetMousePositionImpl();
	//	return x;
	//}

	//float WindowsInput::GetMouseYImpl()
	//{
	//	auto [x, y] = GetMousePositionImpl();
	//	return y;
	//}

}