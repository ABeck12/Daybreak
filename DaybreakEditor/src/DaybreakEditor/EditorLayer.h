#pragma once

#include <Daybreak.h>

#include "DaybreakEditor/Pannels/Pannel.h"

#include "DaybreakEditor/Pannels/SelectionContext.h"

namespace Daybreak
{
	class EditorLayer final : public Layer
	{
	public:
		EditorLayer(const std::filesystem::path& startupSceneFilepath);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

		void SaveScene();
		void OpenScene(const std::filesystem::path& path);

	private:
		void Start();
		void Stop();

	private:
		bool m_IsPlaying = false;

		std::filesystem::path m_SceneFilepath;
		Ref<Scene> m_Scene;
		Ref<Scene> m_SceneCopy;

		PannelOrganizer m_Pannels;
	};
}