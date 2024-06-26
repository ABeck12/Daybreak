#pragma once

#include "dbpch.h"

#include "Daybreak/Core/Window.h"
#include <GLFW/glfw3.h>

namespace Daybreak
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecifications& props);
		virtual ~WindowsWindow();
		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		void SetMaximized(bool maximized) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		virtual void Init(const WindowSpecifications& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		// Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			bool Maximized;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}