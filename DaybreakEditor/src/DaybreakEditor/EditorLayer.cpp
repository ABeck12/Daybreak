#include "DaybreakEditor/EditorLayer.h"

#include "DaybreakEditor/Pannels/ViewportPannel.h"
#include "DaybreakEditor/Pannels/InspectorPannel.h"
#include "DaybreakEditor/Pannels/HierarchyPannel.h"
#include "DaybreakEditor/Pannels/ContentBrowserPannel.h"
#include "DaybreakEditor/Pannels/LogPannel.h"
#include "DaybreakEditor/Pannels/SelectionContext.h"

#include <imgui.h>
// TEMP
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>


namespace Daybreak
{
	EditorLayer::EditorLayer(const std::filesystem::path& startupSceneFilepath)
		: Layer("EditorLayer")
	{
		m_Scene = CreateRef<Scene>("Editor Scene");
		m_SceneFilepath = startupSceneFilepath;
		Application::Get().GetWindow().SetMaximized(true);
		Application::Get().GetImGuiLayer()->SetBlockEvents(false);

		m_Pannels.AddPannel<ViewportPannel>(m_Scene);
		m_Pannels.AddPannel<InspectorPannel>();
		m_Pannels.AddPannel<HierarchyPannel>(m_Scene);
		m_Pannels.AddPannel<ContentBrowserPannel>();
		m_Pannels.AddPannel<LogPannel>();
	}

	void EditorLayer::OnAttach()
	{
		SceneSerializer serializer(m_Scene);
		serializer.Deserialize(m_SceneFilepath);


		std::filesystem::path resourceDir = AssetManager::Get()->GetAssetDir().parent_path().parent_path() / "DaybreakEditor/Resources";
		m_Pannels.GetPannelByName<ContentBrowserPannel>("ContentBrowserPannel")->SetResourceDirectory(resourceDir);

		for (Pannel* pannel : m_Pannels)
		{
			pannel->OnAttach();
		}

		if (m_IsPlaying)
		{
			m_Scene->OnRuntimeStart();
		}


		ImGuiIO& io = ImGui::GetIO();
		ImFont* pFont = io.Fonts->AddFontFromFileTTF((resourceDir / "OpenSans-Regular.ttf").string().c_str(), 18.0f);
	}

	void EditorLayer::OnDetach()
	{
		for (Pannel* pannel : m_Pannels)
		{
			pannel->OnDetach();
		}

		if (m_IsPlaying)
		{
			m_Scene->OnRuntimeStop();
		}
	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		if (m_IsPlaying)
		{
			m_Scene->OnUpdateComponents(dt);
			m_Scene->OnPhysicsUpdate(dt);
		}

		const ViewportPannel* vp = m_Pannels.GetPannelByName<ViewportPannel>("ViewportPannel");
		if (vp->IsPlayButtonActive() != m_IsPlaying)
		{
			m_IsPlaying = vp->IsPlayButtonActive();
			m_IsPlaying ? Start() : Stop();
		}

		for (Pannel* pannel : m_Pannels)
		{
			pannel->OnUpdate(dt);
		}

		if ((Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl) && Input::IsKeyPressed(Key::S)))
		{
			SaveScene();
		}
	}

	void EditorLayer::OnEvent(Event& event)
	{
		for (Pannel* pannel : m_Pannels)
		{
			pannel->OnEvent(event);
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "Ctrl+s"))
				{
					SaveScene();
				}
				if (ImGui::MenuItem("Open"))
				{
					// OpenScene();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		for (Pannel* pannel : m_Pannels)
		{
			pannel->OnImGuiRender();
		}
		// ImGui::ShowDemoWindow();
	}

	void EditorLayer::Start()
	{
		m_SceneCopy = Scene::Copy(m_Scene);
		m_Scene->SetStartTime((float)Time::GetTime());

		auto scripts = m_Scene->GetAllEntitiesWith<ScriptComponent>();
		for (auto e : scripts)
		{
			Entity entity = { e, m_Scene.get() };
			ScriptComponent& sc = entity.GetComponent<ScriptComponent>();
			sc.RuntimeBind(sc.TypeName);
		}
		m_Scene->OnRuntimeStart();
	}

	void EditorLayer::Stop()
	{
		m_Scene->OnRuntimeStop();

		UUID lastSelectedID = 0;
		bool contextWasEntity = SelectionContext::GetContext()->GetContextType() == SelectionContextType::Entity;
		if (contextWasEntity)
		{
			lastSelectedID = std::static_pointer_cast<EntityContext>(SelectionContext::GetContext())->GetEntity().GetUUID();
		}

		m_Scene = Scene::Copy(m_SceneCopy);

		ViewportPannel* vp = m_Pannels.GetPannelByName<ViewportPannel>("ViewportPannel");
		HierarchyPannel* hp = m_Pannels.GetPannelByName<HierarchyPannel>("HierarchyPannel");
		InspectorPannel* ip = m_Pannels.GetPannelByName<InspectorPannel>("InspectorPannel");
		hp->SetScene(m_Scene);
		vp->SetScene(m_Scene);

		if (lastSelectedID && contextWasEntity)
		{
			Entity newEntity = m_Scene->GetEntityByUUID(lastSelectedID);
			if (newEntity)
			{
				SelectionContext::SetContext(CreateRef<EntityContext>(newEntity));
			}
			else
			{
				SelectionContext::ResetContext();
			}
		}
	}

	void EditorLayer::SaveScene()
	{
		SceneSerializer serializer(m_Scene);
		serializer.Serialize(m_SceneFilepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		DB_CORE_LOG("Open not implemented yet");
		SceneSerializer serializer(m_Scene);
		serializer.Deserialize(path);
	}

}