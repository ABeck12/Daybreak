#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

// #include "SceneLayer.h"
#include "LightingScene.h"
#include "FramebufferTesting.h"
#include "StressTesting.h"

#include <DaybreakEditor.h>
#include "ScriptInclude.auto.h"

// #include "ScriptableEntityTest.h"

extern "C"
{
	// __declspec(dllexport) DWORD NvOptimusEnablement = 1;
	// __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

class Sandbox : public Daybreak::Application
{
public:
	Sandbox(Daybreak::ApplicationSpecifications spec)
		: Daybreak::Application(spec)
	{
		Daybreak::Auto::AutoScriptInclude();
		// PushLayer(new SceneLayer());
		// PushLayer(new LightingScene());
		// PushLayer(new FBTesting());
		// PushLayer(new StressTestLayer());
		PushLayer(new Daybreak::EditorLayer(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene"));
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