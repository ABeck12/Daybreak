#pragma once

#include "dbpch.h"

#include "Daybreak/Core/Window.h"
#include "Daybreak/Core/LayerStack.h"

#include "Daybreak/Events/Event.h"
#include "Daybreak/Events/ApplicationEvent.h"
#include "Daybreak/Events/MouseEvent.h"
#include "Daybreak/Events/KeyEvent.h"

namespace Daybreak
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }


	private:
		bool OnWindowClose(WindowCloseEvent& e);

		Scope<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}