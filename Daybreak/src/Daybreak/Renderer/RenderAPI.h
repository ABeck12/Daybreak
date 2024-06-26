#pragma once

#include "Daybreak/Renderer/VertexArray.h"

#include "glm/glm.hpp"

namespace Daybreak
{
	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

		enum class BlendModes
		{
			OneMinusSrcAlpha,
			One,
		};


	public:
		virtual ~RenderAPI() = default;

		virtual void Init() = 0;
		virtual void Clear() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetDepthTesting(bool enabled) = 0;
		virtual void SetBlendMode(BlendModes mode) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		static API GetAPI() { return s_API; }
		static Scope<RenderAPI> Create();

	private:
		static API s_API;
	};
}
