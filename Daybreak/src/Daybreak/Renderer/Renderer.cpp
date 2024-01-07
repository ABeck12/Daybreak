#include "dbpch.h"

#include "Daybreak/Renderer/Renderer.h"

#include "Daybreak/Renderer/RenderCommand.h"

namespace Daybreak
{

	void Renderer::Init()
	{
		RenderCommand::Init();
		//Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}