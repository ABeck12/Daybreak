#include <Daybreak.h>
#include <Daybreak/Core/EntryPoint.h>

#include "SceneLayer.h"

// #include <DaybreakEditor.h>

#include "ScriptableEntityTest.h"
#include "../scripts/test.h"
#include "Daybreak/Scripting/ScriptEngine.h"
// #include "Daybreak/Assets/AssetManager/AssetManager.h"

// extern "C"
// {
// 	// __declspec(dllexport) DWORD NvOptimusEnablement = 1;
// 	//__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
// }
// __declspec(dllimport) void Test();
/*
int TestLoadDLL()
{
	typedef MyStruct(__cdecl * MYPROC)(LPCWSTR);

	HINSTANCE hinstLib;
	// MYPROC ProcAdd;
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

	// Get a handle to the DLL module.

	hinstLib = LoadLibrary(TEXT("C:\\dev\\Daybreak\\bin\\Debug-windows-x86_64\\ScriptLibrary\\ScriptLibrary.dll"));
	// hinstLib = LoadLibrary(TEXT("ScriptLibrary.dll"));

	// If the handle is valid, try to get the function address.

	if (hinstLib != NULL)
	{
		auto ProcAdd = (MYPROC)GetProcAddress(hinstLib, "Test");

		// If the function address is valid, call the function.
		if (NULL != ProcAdd)
		{
			fRunTimeLinkSuccess = TRUE;
			auto out = (ProcAdd)(L"Message sent to the DLL function\n");
			DB_LOG("{} {}", out.i, out.f);
		}
		// Free the DLL module.

		fFreeResult = FreeLibrary(hinstLib);
	}

	// If unable to call the DLL function, use an alternative.
	if (!fRunTimeLinkSuccess)
		printf("Message printed from executable\n");

	return 0;
}
*/
class Sandbox : public Daybreak::Application
{
public:
	Sandbox(Daybreak::ApplicationSpecifications spec)
		: Daybreak::Application(spec)
	{
		DB_REGISTER_SCRIPT(MoveableComponent);
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