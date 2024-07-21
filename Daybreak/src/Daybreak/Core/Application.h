#pragma once

#include "Daybreak/Core/Base.h"

#include "Daybreak/Core/Window.h"
#include "Daybreak/Core/LayerStack.h"

#include "Daybreak/Events/Event.h"
#include "Daybreak/Events/ApplicationEvent.h"
#include "Daybreak/Events/MouseEvent.h"
#include "Daybreak/Events/KeyEvent.h"
#include "Daybreak/ImGui/ImGuiLayer.h"


namespace Daybreak
{
	struct ApplicationSpecifications
	{
		WindowSpecifications WindowSpec;
		// bool UseGPUHighPerformance = false;
	};


	class Application
	{
	public:
		Application(ApplicationSpecifications spec = ApplicationSpecifications());
		virtual ~Application();

		void Run();

		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		ApplicationSpecifications m_AppSpec;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}