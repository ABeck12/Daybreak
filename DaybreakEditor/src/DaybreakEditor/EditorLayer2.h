#pragma once

#include <Daybreak.h>

#include "DaybreakEditor/Pannels/Pannel.h"

namespace Daybreak
{
	class EditorLayer2 final : public Layer
	{
	public:
		EditorLayer2(const std::filesystem::path& startupSceneFilepath);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		std::filesystem::path m_SceneFilepath;
		Ref<Scene> m_Scene;
		Ref<Scene> m_SceneCopy;

		std::vector<Scope<Pannel>> m_Pannels;
	};
}