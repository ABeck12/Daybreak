#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

// #include "SceneLayer.h"
#include "LightingScene.h"

#include <DaybreakEditor.h>
// #include "../scripts/MoveableComponent.h"
#include "scripts/MoveableComponent.h"

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
		DB_REGISTER_SCRIPT(MoveableComponent);
		// PushLayer(new SceneLayer());
		// PushLayer(new LightingScene());
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