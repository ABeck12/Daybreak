#include "dbpch.h"

#include "Daybreak/Renderer/Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Daybreak
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath)
	{
		return CreateRef<OpenGLShader>(name, filepath);
	}
}