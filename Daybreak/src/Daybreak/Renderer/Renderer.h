#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/RenderCommand.h"
#include "Daybreak/Renderer/Shader.h"
#include "Daybreak/Renderer/VertexArray.h"
#include "Daybreak/Renderer/Camera.h"
#include "Daybreak/Renderer/FrameBuffer.h"

namespace Daybreak
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void DrawFrameBuffer(const Ref<FrameBuffer>& frameBuffer, const Ref<Shader>& shader, const std::vector<int>& boundAttachmentIndices = { 0 });

		static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
		// static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
	private:
		// // Temporary?
		// struct RendererData
		// {
		// 	glm::mat4 ViewProjectionMatrix;
		// };

		// static Scope<RendererData> s_SceneData;
	};
}