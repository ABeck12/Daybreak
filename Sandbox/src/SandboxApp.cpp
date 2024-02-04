#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

#include "GameLayer.h"
#include "SceneLayer.h"
#include "AudioLayer.h"

extern "C" {
	// __declspec(dllexport) DWORD NvOptimusEnablement = 1;
	//__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

class Sandbox : public Daybreak::Application
{
public:
	Sandbox()
	{
		//PushLayer(new GameLayer());
		// PushLayer(new SceneLayer());
		PushLayer(new AudioLayer());
	}
	~Sandbox()
	{
	}
};

Daybreak::Application* Daybreak::CreateApplication()
{
	return new Sandbox();
}