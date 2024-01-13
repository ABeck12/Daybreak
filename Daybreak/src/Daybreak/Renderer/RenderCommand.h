#pragma once

#include "Daybreak/Renderer/RenderAPI.h"
#include "Daybreak/Renderer/VertexArray.h"
#include "glm/glm.hpp"

namespace Daybreak
{
	class RenderCommand
	{
	public:
		static inline void Init() { s_RenderAPI->Init(); }
		static inline void Clear() { s_RenderAPI->Clear(); }
		static inline void SetClearColor(const glm::vec4& color) { s_RenderAPI->SetClearColor(color); }
		static inline void DrawIndexed(const Ref<VertexArray>& vertexArray, const uint32_t& indexCount = 0) { s_RenderAPI->DrawIndexed(vertexArray, indexCount); }
		static inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RenderAPI->SetViewport(x, y, width, height); }

	private:
		static Scope<RenderAPI> s_RenderAPI;
	};
}