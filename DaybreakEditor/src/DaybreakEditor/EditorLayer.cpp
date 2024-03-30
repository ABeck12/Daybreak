#include "DaybreakEditor/EditorLayer.h"

#include <imgui.h>

#include <glfw/glfw3.h>

namespace Daybreak
{
	EditorLayer::EditorLayer(const std::string& startupSceneFilepath)
		: Layer("EditorLayer")
	{
		m_Scene = Daybreak::CreateRef<Daybreak::Scene>("Editor Scene");

		Daybreak::FrameBufferSpecifications fbspec;
		fbspec.Width = Daybreak::Application::Get().GetWindow().GetWidth();
		fbspec.Height = Daybreak::Application::Get().GetWindow().GetHeight();
		m_FrameBuffer = Daybreak::FrameBuffer::Create(fbspec);

		m_SceneFilepath = startupSceneFilepath;

		m_HierarchyPannel = HierarchyPannel(m_Scene);

		Application::Get().GetWindow().SetMaximized(true);
	}

	void EditorLayer::OnAttach()
	{
		Daybreak::SceneSerializer serializer(m_Scene);
		serializer.Deserialize(m_SceneFilepath);
		m_Scene->OnRuntimeStart();
	}

	void EditorLayer::OnDetach()
	{
		m_Scene->OnRuntimeEnd();
	}

	void EditorLayer::OnUpdate(Daybreak::DeltaTime dt)
	{
		m_FrameBuffer->Bind();
		Daybreak::RenderCommand::Clear();
		Daybreak::RenderCommand::SetClearColor(glm::vec4(0));

		m_Scene->OnRuntimeUpdate(dt);
		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnEvent(Daybreak::Event& event)
	{
		if (event.GetEventType() == Daybreak::EventType::KeyPressed)
		{
			Daybreak::KeyPressedEvent& e = (Daybreak::KeyPressedEvent&)event;
			if (e.GetKeyCode() == Daybreak::Key::Escape)
			{
				Daybreak::Application::Get().Close();
			}
		}
		if (event.GetEventType() == Daybreak::EventType::WindowResize)
		{
			Daybreak::WindowResizeEvent& e = (Daybreak::WindowResizeEvent&)event;
			if (e.GetWidth() != 0 && e.GetHeight() != 0) // TEMPORARY!!!
			{
				Daybreak::Camera camera = m_Scene->GetActiveCameraEntity().GetComponent<Daybreak::CameraComponent>().Camera;
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

		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
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

		ImGui::Begin("Viewport");
		uint32_t textureID = m_FrameBuffer->GetAttachmentRendererID(0);
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		float aspectRatio = 1920.0f / 1080.0f;
		ImVec2 imageSize = { viewportSize.x, viewportSize.x / aspectRatio };
		ImGui::SetCursorPos({ 0.5f * (viewportSize.x - imageSize.x), 0.5f * (viewportSize.y - imageSize.y) });
		ImGui::Image((void*)textureID, imageSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		m_HierarchyPannel.Render();

		// ImGui::Begin("Debug Stats");
		// ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		// ImGui::End();
	}
}