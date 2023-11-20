#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Daybreak/Math/Math.h"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//void SetUniform() const; //make this templated in real engine
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
	void SetUniformMat4f(const std::string& name, Daybreak::Mat4& matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);

private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
};








//ShaderProgramSource ParseShader(const std::string& filepath);
//int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
//unsigned int CompileShader(unsigned int type, const std::string& source);