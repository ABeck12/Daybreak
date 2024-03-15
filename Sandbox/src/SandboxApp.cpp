#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

#include "GameLayer.h"
#include "SceneLayer.h"

extern "C"
{
	// __declspec(dllexport) DWORD NvOptimusEnablement = 1;
	//__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

class Sandbox : public Daybreak::Application
{
public:
	Sandbox(Daybreak::ApplicationSpecifications spec)
		: Daybreak::Application(spec)
	{
		PushLayer(new SceneLayer());
	}

	~Sandbox()
	{
	}
};

Daybreak::Application* Daybreak::CreateApplication()
{
	ApplicationSpecifications spec;
	return new Sandbox(spec);
}