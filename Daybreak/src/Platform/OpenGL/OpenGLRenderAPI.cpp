#include "dbpch.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Daybreak
{
	void OpenGLRenderAPI::Init()
	{
		//TODO: Temporary, move to GraphicsContext->Init() inside platform window init()
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
}