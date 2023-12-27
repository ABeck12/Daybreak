#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

#include "TestLayer.h"

class Sandbox : public Daybreak::Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
		PushOverlay(new Daybreak::ImGuiLayer());
	}
	~Sandbox()
	{
	}
};

Daybreak::Application* Daybreak::CreateApplication()
{
	return new Sandbox();
}