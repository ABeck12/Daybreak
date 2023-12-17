#pragma once

#include "dbpch.h"

namespace Daybreak
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		Ref<Shader> Create(const std::string& filepath);

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

	};
}
