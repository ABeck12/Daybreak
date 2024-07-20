#include "dbpch.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Daybreak
{
	OpenGLShader::OpenGLShader(const std::string& name, const std::filesystem::path& filepath)
		: m_Name(name)
	{
		std::ifstream stream(filepath);


		ShaderType type = ShaderType::NONE;

		std::string line;
		std::stringstream ss[3];
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("VERTEX") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("FRAGMENT") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
				else if (line.find("COMPUTE") != std::string::npos)
				{
					type = ShaderType::COMPUTE;
				}
			}
			else
			{
				ss[(int)type] << line << "\n";
			}
		}
		std::string vertexSrc = ss[0].str();
		std::string fragmentSrc = ss[1].str();
		std::string computeSrc = ss[2].str();

		m_RendererID = glCreateProgram();

		if (computeSrc.size() > 0)
		{
			uint32_t cs = CompileShader(GL_COMPUTE_SHADER, computeSrc);
			glAttachShader(m_RendererID, cs);

			glLinkProgram(m_RendererID);
			glValidateProgram(m_RendererID);

			glDeleteProgram(cs);
		}
		if (vertexSrc.size() > 0 && fragmentSrc.size() > 0)
		{
			uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
			uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

			glAttachShader(m_RendererID, vs);
			glAttachShader(m_RendererID, fs);
			glLinkProgram(m_RendererID);
			glValidateProgram(m_RendererID);

			glDeleteShader(vs);
			glDeleteShader(fs);
		}
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
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

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& computeSrc)
		: m_Name(name)
	{
		m_RendererID = glCreateProgram();

		uint32_t cs = CompileShader(GL_COMPUTE_SHADER, computeSrc);
		glAttachShader(m_RendererID, cs);

		glLinkProgram(m_RendererID);
		glValidateProgram(m_RendererID);

		glDeleteProgram(cs);
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

	void OpenGLShader::SetInt1(const std::string& name, const uint32_t value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, const int* values, uint32_t count) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetFloat1(const std::string& name, const float value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& vec) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, vec.x, vec.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vec) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, vec.x, vec.y, vec.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vec) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
	}

	// Please make this better this was copied over from the openGL test renderer
	uint32_t OpenGLShader::CompileShader(uint32_t shaderType, const std::string& shaderSrc)
	{
		if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER && shaderType != GL_COMPUTE_SHADER)
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

#ifdef DB_ENABLE_LOGGING
			std::string shaderTypeName;
			switch (shaderType)
			{
				case GL_VERTEX_SHADER:
					shaderTypeName = "vertex";
					break;
				case GL_FRAGMENT_SHADER:
					shaderTypeName = "fragment";
					break;
				case GL_COMPUTE_SHADER:
					shaderTypeName = "compute";
					break;
			}
			DB_CORE_ERROR("Failed to compile {0} shader {1} {2}", shaderTypeName, m_Name, message);
#endif

			glDeleteShader(id);
			return 0;
		}
		return id;
	}

	void OpenGLShader::DispatchCompute(const uint32_t numGroupsX, const uint32_t numGroupsY, const uint32_t numGroupsZ) const
	{
		DB_CORE_ASSERT(GetShaderType() == ShaderType::COMPUTE, "Can not dispatch non-compute shader {0}", m_Name);
		Bind();
		glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}

	const OpenGLShader::ShaderType OpenGLShader::GetShaderType() const
	{
		int out;
		glGetShaderiv(m_RendererID, GL_SHADER_TYPE, &out);

		switch (out)
		{
			case GL_VERTEX_SHADER:
				return ShaderType::VERTEX;
			case GL_FRAGMENT_SHADER:
				return ShaderType::FRAGMENT;
			case GL_COMPUTE_SHADER:
				return ShaderType::COMPUTE;
			default:
				return ShaderType::NONE;
		}
	}
}