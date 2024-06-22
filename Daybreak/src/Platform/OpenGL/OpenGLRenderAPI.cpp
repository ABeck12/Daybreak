#include "dbpch.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Daybreak
{
	void OpenGLRenderAPI::Init()
	{
		// TODO: Temporary, move to GraphicsContext->Init() inside platform window init()
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DB_CORE_INFO("OpenGL Info:");
		DB_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		DB_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		DB_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));
		//===================================================================

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// For Anti Aliasing
		glEnable(GL_MULTISAMPLE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Face culling
		glEnable(GL_CULL_FACE);
	}

	void OpenGLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::SetDepthTesting(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_BLEND);
			glDepthFunc(GL_LEQUAL);
		}
		else
		{
			glEnable(GL_BLEND);
			glDepthFunc(GL_ALWAYS);
			// glDisable(GL_BLEND);
		}
	}

	void OpenGLRenderAPI::SetBlendMode(BlendModes mode)
	{
		switch (mode)
		{
			case BlendModes::One:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case BlendModes::OneMinusSrcAlpha:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
		}
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