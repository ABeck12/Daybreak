#include "DaybreakEditor/Pannels/LogPannel.h"

#include <imgui.h>

namespace Daybreak
{
	void LogPannel::OnAttach()
	{
	}

	void LogPannel::OnDetach()
	{
	}

	void LogPannel::OnUpdate(DeltaTime dt)
	{
	}

	void LogPannel::OnImGuiRender()
	{
		ImGui::Begin("Log");
		const ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	void LogPannel::OnEvent(Event& event)
	{
	}

}