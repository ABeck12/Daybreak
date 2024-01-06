#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

#include "GameLayer.h"

//#ifdef __cplusplus
//extern "C" {
//#endif
//
//	__declspec(dllexport) DWORD NvOptimusEnablement = 1;
//	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//
//#ifdef __cplusplus
//}
//#endif

class Sandbox : public Daybreak::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameLayer());
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