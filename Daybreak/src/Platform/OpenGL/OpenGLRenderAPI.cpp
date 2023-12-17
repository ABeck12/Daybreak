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
}