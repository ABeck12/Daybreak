#pragma once

#include "Daybreak/Renderer/RenderAPI.h"
#include "Daybreak/Renderer/VertexArray.h"
#include "glm/glm.hpp"

namespace Daybreak
{
	class RenderCommand
	{
	public:
		static void Init() { s_RenderAPI->Init(); }
		static void Clear() { s_RenderAPI->Clear(); }
		static void SetClearColor(const glm::vec4& color) { s_RenderAPI->SetClearColor(color); }
		static void DrawIndexed(const Ref<VertexArray>& vertexArray) { s_RenderAPI->DrawIndexed(vertexArray); }
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RenderAPI->SetViewport(x, y, width, height);
		}

	private:
		static Scope<RenderAPI> s_RenderAPI;
	};
}