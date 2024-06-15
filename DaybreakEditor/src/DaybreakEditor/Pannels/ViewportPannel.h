#pragma once

#include <Daybreak.h>

#include "DaybreakEditor/Pannels/Pannel.h"

namespace Daybreak
{
	class ViewportPannel final : public Pannel
	{
	public:
		ViewportPannel(Ref<Scene> scene);
		// virtual ~ViewportPannel() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;
		virtual const std::string& GetName() const override { return m_PannelName; }

		void SetScene(Ref<Scene> scene) { m_Scene = scene; }

		bool IsPlayButtonActive() const { return m_IsPlaying; }

	private:
		void DrawTransformGuizmo(Entity& entity) const;
		void UpdateEditorCameraPosition();
		void DrawGrid() const;


	private:
		std::string m_PannelName = "ViewportPannel";

		const glm::vec4 m_RuntimeClearColor = { 0, 0, 0, 1 };
		const glm::vec4 m_EditorClearColor = { 0.25f, 0.25f, 0.25f, 1.0f };

		Ref<Scene> m_CameraScene;
		Entity m_EditorCameraEntity;
		Ref<Scene> m_Scene;
		const float m_AspectRatio = 1920.0f / 1080.0f;

		glm::vec2 m_ViewportBounds;
		glm::vec2 m_ImageSize;

		float m_GuizmoOffset = 0;

		bool m_DrawGrid = true;
		const bool m_Guizmos2D = true;
		bool m_IsPlaying = false;
	};
}