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

		static void OnWindowResize(uint32_t width, uint32_t height);

		static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		
		// This is where something like Renderer::DrawMesh would go for a 3D object?
	};
}