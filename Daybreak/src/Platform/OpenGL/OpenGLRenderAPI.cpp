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