#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

#include "SceneLayer.h"
#include "LightingScene.h"

#include <DaybreakEditor.h>

#include "ScriptableEntityTest.h"

class Sandbox : public Daybreak::Application
{
public:
	Sandbox(Daybreak::ApplicationSpecifications spec)
		: Daybreak::Application(spec)
	{
		DB_REGISTER_SCRIPT(MoveableComponent);
		// PushLayer(new SceneLayer());
		PushLayer(new LightingScene());
		// PushLayer(new Daybreak::EditorLayer(Daybreak::AssetManager::Get()->GetAssetDir() / "scenes/SceneLayer.scene"));
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