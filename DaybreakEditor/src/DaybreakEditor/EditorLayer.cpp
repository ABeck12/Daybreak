#include "DaybreakEditor/EditorLayer.h"

#include <imgui.h>

#include <glfw/glfw3.h>

namespace Daybreak
{
	EditorLayer::EditorLayer(const std::filesystem::path& startupSceneFilepath)
		: Layer("EditorLayer")
	{
		InitCameraScene();
		m_Scene = CreateRef<Scene>("Editor Scene");

		FrameBufferSpecifications fbspec;
		fbspec.Width = Application::Get().GetWindow().GetWidth();
		fbspec.Height = Application::Get().GetWindow().GetHeight();
		m_FrameBuffer = FrameBuffer::Create(fbspec);


		m_SceneFilepath = startupSceneFilepath;

		m_HierarchyPannel = HierarchyPannel(m_Scene);

		Application::Get().GetWindow().SetMaximized(true);
	}

	void EditorLayer::OnAttach()
	{
		SceneSerializer serializer(m_Scene);
		serializer.Deserialize(m_SceneFilepath);
		if (m_IsPlaying)
			m_Scene->OnRuntimeStart();
	}

	void EditorLayer::OnDetach()
	{
		if (m_IsPlaying)
			m_Scene->OnRuntimeEnd();
	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		m_FrameBuffer->Bind();
		RenderCommand::Clear();
		if (m_IsPlaying)
		{
			RenderCommand::SetClearColor(glm::vec4(0));
			m_Scene->OnRuntimeUpdate(dt);
		}
		else
		{
			RenderCommand::SetClearColor(glm::vec4(0.15f, 0.15, 0.15f, 1.0f));
			UpdateCameraPosition(dt);
			m_Scene->EditorRenderScene(m_EditorCamera);
		}
		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& e = (KeyPressedEvent&)event;
			if (e.GetKeyCode() == Key::Escape)
			{
				Application::Get().Close();
			}
		}
		if (event.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& e = (WindowResizeEvent&)event;
			if (e.GetWidth() != 0 && e.GetHeight() != 0) // TEMPORARY!!!
			{
				Camera camera = m_Scene->GetActiveCameraEntity().GetComponent<CameraComponent>().Camera;
				camera.SetProjection(glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.1f, 75.0f));

				Camera editorCamera = m_EditorCamera.GetComponent<CameraComponent>().Camera;
				camera.SetProjection(glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.1f, 75.0f));

				m_FrameBuffer->Resize(e.GetWidth(), e.GetHeight());
			}
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		// Render framebuffer
		ImGuiIO& io = ImGui::GetIO();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// static bool opt_fullscreen_persistant = true;
		// bool opt_fullscreen = opt_fullscreen_persistant;
		// ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		// window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		// window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		// if (opt_fullscreen)
		// {
		// 	ImGuiViewport* viewport = ImGui::GetMainViewport();
		// 	ImGui::SetNextWindowPos(viewport->Pos);
		// 	ImGui::SetNextWindowSize(viewport->Size);
		// 	ImGui::SetNextWindowViewport(viewport->ID);
		// 	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		// 	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		// 	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		// window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		// window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		// }
		// if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		// {
		// 	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		// 	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
		// }


		bool isopen = true;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					SaveScene();
				}
				if (ImGui::MenuItem("Open"))
				{
					DB_CORE_LOG("Open not implemented yet");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		m_HierarchyPannel.Render();
		if (ImGui::IsMouseClicked(0))
		{
			if (m_HierarchyPannel.HasSelectedEntity())
			{
				DB_LOG("Setting camera position");
				auto& ts = m_EditorCamera.GetComponent<TransformComponent>();
				glm::vec2 newPos = m_HierarchyPannel.GetSelectedEntityPosition();
				ts.Position.x = newPos.x;
				ts.Position.y = newPos.y;
			}
		}


		ImGui::Begin("Viewport");


		std::string text = m_IsPlaying ? "Pause" : "Play";
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
		if (ImGui::Button(text.c_str()))
		{
			m_IsPlaying = !m_IsPlaying;
			m_IsPlaying ? Start() : Stop();
		}
		ImGui::Separator();
		uint32_t textureID = m_FrameBuffer->GetAttachmentRendererID(0);
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		float aspectRatio = 1920.0f / 1080.0f;
		ImVec2 imageSize = { viewportSize.x, viewportSize.x / aspectRatio };
		ImGui::SetCursorPos({ 0.5f * (viewportSize.x - imageSize.x), 0.5f * (viewportSize.y - imageSize.y) + 43 }); // FIXME: 43 is just a temporary guess on to get the viewport to line up vertically correctly
		ImGui::Image((void*)textureID, imageSize, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::Begin("Stats");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();


		// ImGui::ShowDemoWindow();
	}


	void EditorLayer::SaveScene()
	{
		SceneSerializer serializer(m_Scene);
		serializer.Serialize(m_SceneFilepath);
	}

	void EditorLayer::Start()
	{
		m_SceneCopy = Scene::Copy(m_Scene);
		m_Scene->SetStartTime(Time::GetTime());
		m_Scene->OnRuntimeStart();
	}

	void EditorLayer::Stop()
	{
		m_Scene->OnRuntimeEnd();
		m_Scene = Scene::Copy(m_SceneCopy);
		m_HierarchyPannel.SetScene(m_Scene);
	}

	void EditorLayer::InitCameraScene()
	{
		m_EditorCameraScene = CreateRef<Scene>("Editor Camera Scene");
		m_EditorCamera = m_EditorCameraScene->CreateEntity("Editor Camera");
		auto& ts = m_EditorCamera.GetComponent<TransformComponent>();
		ts.Position = { 0, 0, -10. };
		auto& cc = m_EditorCamera.AddComponent<CameraComponent>();
		cc.Camera.SetProjection(glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 75.0f));
	}

	void EditorLayer::UpdateCameraPosition(DeltaTime dt)
	{
		auto& ts = m_EditorCamera.GetComponent<TransformComponent>();
		if (Input::IsKeyPressed(Key::Left))
		{
			ts.Position.x -= dt * 10;
		}
		if (Input::IsKeyPressed(Key::Right))
		{
			ts.Position.x += dt * 10;
		}
		if (Input::IsKeyPressed(Key::Up))
		{
			ts.Position.y += dt * 10;
		}
		if (Input::IsKeyPressed(Key::Down))
		{
			ts.Position.y -= dt * 10;
		}
	}
}