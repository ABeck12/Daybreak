#pragma once

#include "Daybreak/Renderer/RenderAPI.h"

namespace Daybreak
{
	class RenderCommand
	{
	public:
		static void Init() { s_RenderAPI->Init(); }

	private:
		static Scope<RenderAPI> s_RenderAPI;
	};
}