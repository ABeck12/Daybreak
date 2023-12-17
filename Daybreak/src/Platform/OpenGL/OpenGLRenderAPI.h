#pragma once

#include "Daybreak/Renderer/RenderAPI.h"

namespace Daybreak
{
	class OpenGLRenderAPI : public RenderAPI
	{
		virtual void Init() override;
		virtual void Clear() override;
	};
}
