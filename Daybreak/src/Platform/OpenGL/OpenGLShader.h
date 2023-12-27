#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Shader.h"

namespace Daybreak
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& name, const std::string& filepath);

		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetVec2(glm::vec2 vec) const override;
		virtual void SetVec3(glm::vec3 vec) const override;
		virtual void SetVec4(glm::vec4 vec) const override;
		virtual void SetMat3(glm::mat3 mat) const override;
		virtual void SetMat4(glm::mat4 mat) const override;

		virtual const std::string& GetName() const override { return m_Name; }

	private:
		uint32_t CompileShader(uint32_t shaderType, const std::string& shaderSrc);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}
