#pragma once

#include <Daybreak.h>

#include "DaybreakEditor/Pannels/HierarchyPannel.h"

namespace Daybreak
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const std::filesystem::path& startupSceneFilepath);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		void SaveScene();
		// void OpenScene(const std::filesystem::path&);


		void Start();
		void Stop();

		void InitCameraScene();
		void UpdateCameraPosition(DeltaTime dt);

	private:
		std::filesystem::path m_SceneFilepath;
		Ref<Scene> m_Scene;
		Ref<Scene> m_SceneCopy;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_EditorCameraScene;
		Entity m_EditorCamera;
		glm::vec2 m_LastMouseClickedPos;

		HierarchyPannel m_HierarchyPannel;
		bool m_IsPlaying = false;
	};
}