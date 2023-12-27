#include "dbpch.h"

#include "Daybreak/Renderer/Renderer.h"

#include "Daybreak/Renderer/RenderCommand.h"

namespace Daybreak
{

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		vertexArray->Bind();
	}
}