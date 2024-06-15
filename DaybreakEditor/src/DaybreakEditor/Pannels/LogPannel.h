#pragma once

#include <Daybreak.h>

#include "DaybreakEditor/Pannels/Pannel.h"

namespace Daybreak
{
	class LogPannel final : public Pannel
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime dt) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;
		virtual const std::string& GetName() const override { return m_PannelName; }

	private:
		std::string m_PannelName = "LogPannel";
	};
}