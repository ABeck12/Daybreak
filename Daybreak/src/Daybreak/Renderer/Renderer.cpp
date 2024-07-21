#include "dbpch.h"

#include "Daybreak/Renderer/Renderer.h"

#include "Daybreak/Renderer/RenderCommand.h"
#include "Daybreak/Renderer/Renderer2D.h"
#include <glad/glad.h>

namespace Daybreak
{
	struct RendererData
	{
		static constexpr float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

		static constexpr uint32_t indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		Ref<VertexArray> frameBufferVA;
		Ref<VertexBuffer> frameBufferVB;
		Ref<IndexBuffer> frameBufferIB;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();

		s_Data.frameBufferVA = VertexArray::Create();
		s_Data.frameBufferVB = VertexBuffer::Create(s_Data.vertices, 20 * sizeof(float));
		s_Data.frameBufferIB = IndexBuffer::Create(s_Data.indices, 6);

		s_Data.frameBufferVB->SetLayout({
			{ RenderDataTypes::Float3, "a_ScreenCoord" },
			{ RenderDataTypes::Float2, "a_TexCoord" },
		});
		s_Data.frameBufferVA->AddVertexBuffer(s_Data.frameBufferVB);
		s_Data.frameBufferVA->SetIndexBuffer(s_Data.frameBufferIB);
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::DrawFrameBuffer(const Ref<FrameBuffer>& frameBuffer, const Ref<Shader>& shader, const std::vector<int>& boundAttachmentIndices)
	{
		shader->Bind();

		// FIXME: Temporary remove? need to be reworked for renderpasses?
		// for (const int i : boundAttachmentIndices)
		// {
		// 	frameBuffer->BindAttachmentAsTexture(i, i);
		// }
		shader->SetIntArray("u_Textures", &boundAttachmentIndices[0], (uint32_t)boundAttachmentIndices.size());

		RenderCommand::DrawIndexed(s_Data.frameBufferVA);
		shader->Unbind();
	}

	// void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	// {
	// 	s_SceneData->ViewProjectionMatrix = camera.GetProjection() * transform;
	// }

	// void Renderer::EndScene()
	// {

	// }

	// void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	// {
	// 	shader->Bind();
	// 	shader->SetMat4("u_Transform", transform);
	// 	shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix); // Set the view projection from a reference to the current scenes camera
	// 	vertexArray->Bind();
	// 	RenderCommand::DrawIndexed(vertexArray);
	// }

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}