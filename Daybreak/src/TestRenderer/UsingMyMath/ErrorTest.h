#pragma once
#include <glad/glad.h>
#include <iostream>

#define LOG(x) std::cout << x << std::endl
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogcall())

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogcall()
{
	while (GLenum error = glGetError())
	{
		LOG(error);
		return false;
	}
	return true;
}



