//#pragma once
//
//#include "Daybreak/Core/Layer.h"
//#include "Daybreak/Events/ApplicationEvent.h"
//#include "Daybreak/Events/KeyEvent.h"
//#include "Daybreak/Events/MouseEvent.h"
//
//namespace Daybreak
//{
//	class ImGuiLayer : public Layer
//	{
//	public:
//		ImGuiLayer();
//		~ImGuiLayer();
//
//		virtual void OnAttach() override;
//		virtual void OnDetach() override;
//		virtual void OnUpdate() override;
//		virtual void OnEvent(Event& event) override;
//	private:
//		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
//		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
//		bool OnMouseMovedEvent(MouseMovedEvent& e);
//		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
//		bool OnKeyPressedEvent(KeyPressedEvent& e);
//		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
//		bool OnKeyTypedEvent(KeyTypedEvent& e);
//		bool OnWindowResizeEvent(WindowResizeEvent& e);
//	private:
//		float m_Time = 0.0f;
//	};
//}