#pragma once

#include "DaybreakEditor/Pannels/Pannel.h"

#include "DaybreakEditor/Pannels/SelectionContext.h"

namespace Daybreak
{
	class HierarchyPannel final : public Pannel
	{
	public:
		HierarchyPannel(Ref<Scene> scene);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;
		virtual const std::string& GetName() const override { return m_PannelName; }

		// Entity GetSelectedEntity() { return m_SelectedEntity; }
		// void ResetSelected() { m_SelectedEntity = m_Scene->GetEntityByUUID(m_SelectedEntityID); }
		void SetScene(Ref<Scene> scene) { m_Scene = scene; }

	private:
		void DrawEntityRow(Entity entity, bool drawRowAsChild = false);

		void RemoveEntityAsChild(Entity parent, Entity child);
		void AddEntityAsChild(Entity parent, Entity child);

	private:
		std::string m_PannelName = "HierarchyPannel";
		Ref<Scene> m_Scene;
		// Entity m_SelectedEntity;
		// UUID m_SelectedEntityID;
	};
}