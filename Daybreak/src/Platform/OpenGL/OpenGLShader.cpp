#include "dbpch.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "glad/glad.h"

namespace Daybreak
{
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name)
	{
		m_RendererID = glCreateProgram();

		uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

		glAttachShader(m_RendererID, vs);
		glAttachShader(m_RendererID, fs);
		glLinkProgram(m_RendererID);
		glValidateProgram(m_RendererID);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath) : m_Name(name)
	{

	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetVec2(glm::vec2 vec) const
	{

	}
	
	void OpenGLShader::SetVec3(glm::vec3 vec) const
	{

	}
	
	void OpenGLShader::SetVec4(glm::vec4 vec) const
	{

	}
	
	void OpenGLShader::SetMat3(glm::mat3 mat) const
	{

	}
	
	void OpenGLShader::SetMat4(glm::mat4 mat) const
	{

	}
	 
	//Please make this better this was copied over from the openGL test renderer
	uint32_t OpenGLShader::CompileShader(uint32_t shaderType, const std::string& shaderSrc)
	{
		if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER)
		{
			DB_CORE_ERROR("Unknown Shader type {0}", shaderType);
		}

		uint32_t id = glCreateShader(shaderType);
		const char* src = shaderSrc.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}
}