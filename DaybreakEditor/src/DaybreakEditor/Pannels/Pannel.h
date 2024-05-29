#pragma once

namespace Daybreak
{
	class Pannel
	{
	public:
		// virtual ~Pannel() = 0;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		// virtual void OnUpdate(DeltaTime dt) {}
		// virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() = 0;
	};
}