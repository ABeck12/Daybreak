#pragma once

#include "DaybreakEditor/Pannels/Pannel.h"

#include "DaybreakEditor/Pannels/SelectionContext.h"

namespace Daybreak
{
	class InspectorPannel final : public Pannel
	{
	public:
		InspectorPannel();
		virtual ~InspectorPannel() override = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;
		virtual const std::string& GetName() const override { return m_PannelName; }


	private:
		void DrawEntityInspector(Entity& entity);
		void DrawFileInspector(const std::filesystem::path& path);

	private:
		std::string m_PannelName = "InspectorPannel";
		std::string m_LastEntityName;
		// Entity m_SelectedEntity; // TODO: Change to selected context once we can insptect things other than entities
	};
}