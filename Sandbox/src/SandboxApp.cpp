#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

#include "SceneLayer.h"

#include <DaybreakEditor.h>

#include "ScriptableEntityTest.h"

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
		DB_REGISTER_SCRIPTABLE_ENTITY(MoveableComponent);
		// PushLayer(new SceneLayer());
		PushLayer(new Daybreak::EditorLayer("../Sandbox/assets/scenes/SceneLayer.scene"));
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