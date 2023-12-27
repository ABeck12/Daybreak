#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/RenderCommand.h"
#include "Daybreak/Renderer/Shader.h"
#include "Daybreak/Renderer/VertexArray.h"

namespace Daybreak
{
	class Renderer
	{

	public:
		static void Init();
		static void Shutdown();

		static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		
	};
}