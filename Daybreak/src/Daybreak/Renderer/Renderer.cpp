#include "dbpch.h"

#include "Daybreak/Renderer/Renderer.h"

#include "Daybreak/Renderer/RenderCommand.h"
#include "Daybreak/Renderer/Renderer2D.h"

namespace Daybreak
{
	Scope<Renderer::RendererData> Renderer::s_SceneData = CreateScope<Renderer::RendererData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetProjection() * transform;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_Transform", transform);
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix); // Set the view projection from a reference to the current scenes camera
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}