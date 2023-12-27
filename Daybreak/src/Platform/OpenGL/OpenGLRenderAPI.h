#pragma once

#include "Daybreak/Renderer/RenderAPI.h"

#include "glm/glm.hpp"

namespace Daybreak
{
	class OpenGLRenderAPI : public RenderAPI
	{
		virtual void Init() override;
		virtual void Clear() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
	};
}
