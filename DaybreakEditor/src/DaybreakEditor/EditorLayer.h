#pragma once

#include <Daybreak.h>

#include "DaybreakEditor/Pannels/HierarchyPannel.h"

namespace Daybreak
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const std::string& startupSceneFilepath);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		void SaveScene();

	private:
		std::string m_SceneFilepath;
		Ref<Scene> m_Scene;
		Ref<Scene> m_RuntimeScene;
		Ref<FrameBuffer> m_FrameBuffer;

		HierarchyPannel m_HierarchyPannel;
		bool m_IsPlaying = false;
	};
}