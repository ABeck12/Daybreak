#pragma once

#include "dbpch.h"

#include "Daybreak/Renderer/Shader.h"

namespace Daybreak
{
	class OpenGLShader final : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& name, const std::filesystem::path& filepath);

		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt1(const std::string& name, const uint32_t value) const override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) const override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& vec) const override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& vec) const override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& vec) const override;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat) const override;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) const override;

		virtual const std::string& GetName() const override { return m_Name; }

	private:
		uint32_t CompileShader(uint32_t shaderType, const std::string& shaderSrc);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}
