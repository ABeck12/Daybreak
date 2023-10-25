#include <Daybreak.h>

class ExampleLayer : public Daybreak::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//DB_INFO("ExampleLayer::Update");
		//if (Daybreak::Input::IsKeyPressed(DB_KEY_TAB))
		//	DB_LOG("Tab key is pressed (poll)!");
	}

	void OnEvent(Daybreak::Event& event) override
	{
		//DB_LOG("{0}", event);
		//if (event.GetEventType() == Daybreak::EventType::KeyPressed)
		//{
		//	Daybreak::KeyPressedEvent& e = (Daybreak::KeyPressedEvent&)event;
		//	if (e.GetKeyCode() == DB_KEY_TAB)
		//		DB_LOG("Tab key is pressed (event)!");
		//	DB_LOG("{0}", (char)e.GetKeyCode());
		//}
	}

};
class Sandbox : public Daybreak::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{
	}
};

Daybreak::Application* Daybreak::CreateApplication()
{
	return new Sandbox();
}