#include "DaybreakEditor/EditorLayer2.h"

#include "DaybreakEditor/Pannels/ViewportPannel.h"

#include <imgui.h>

namespace Daybreak
{
	EditorLayer2::EditorLayer2(const std::filesystem::path& startupSceneFilepath)
		: Layer("EditorLayer")
	{
		m_Scene = CreateRef<Scene>("Editor Scene");
		m_SceneFilepath = startupSceneFilepath;
		Application::Get().GetWindow().SetMaximized(true);

		m_Pannels.emplace_back(CreateScope<ViewportPannel>());
	}

	void EditorLayer2::OnAttach()
	{
		SceneSerializer serializer(m_Scene);
		serializer.Deserialize(m_SceneFilepath);
	}

	void EditorLayer2::OnDetach()
	{
	}

	void EditorLayer2::OnUpdate(DeltaTime dt)
	{
		// Ref<ViewportPannel> viewport = m_Pannels[0];
	}

	void EditorLayer2::OnEvent(Event& event)
	{
	}

	void EditorLayer2::OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		for (Scope<Pannel>& pannel : m_Pannels)
		{
			pannel->OnImGuiRender();
		}
	}


}