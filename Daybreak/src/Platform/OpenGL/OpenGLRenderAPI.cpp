#include "dbpch.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Daybreak
{
	void OpenGLRenderAPI::Init()
	{
		//TODO: Temporary, move to GraphicsContext->Init() inside platform window init()
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DB_CORE_INFO("OpenGL Info:");
		DB_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		DB_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		DB_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));
		//===================================================================

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
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

	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRenderAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}