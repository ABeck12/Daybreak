#pragma once

#include <stdint.h>
#include <string>
#include <filesystem>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Daybreak/Core/Base.h"

namespace Daybreak
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void DispatchCompute(const uint32_t numGroupsX, const uint32_t numGroupsY, const uint32_t numGroupsZ) const = 0;

		virtual void SetInt1(const std::string& name, const uint32_t value) const = 0;
		virtual void SetIntArray(const std::string& name, const int* values, uint32_t count) const = 0;
		virtual void SetFloat1(const std::string& name, const float value) const = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& vec) const = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& vec) const = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& vec) const = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) const = 0;

		static Ref<Shader> Create(const std::string& name, const std::filesystem::path& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		// static Ref<Shader> Create(const std::string& name, const std::string& computeSrc);
	};

	class ShaderLibrary
	{
	public:
		static void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader>& Load(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>&> m_Shaders;
	};
}