#pragma once

#include <Daybreak.h>

#include "DaybreakEditor/Pannels/Pannel.h"

namespace Daybreak
{
	class ViewportPannel final : public Pannel
	{
	public:
		ViewportPannel();
		// ~ViewportPannel();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		// virtual void OnUpdate(DeltaTime dt) override;
		// virtual void OnEvent(Event& event) override;
		virtual void OnImGuiRender() override;

	private:
		Ref<FrameBuffer> m_Framebuffer;
	};
}