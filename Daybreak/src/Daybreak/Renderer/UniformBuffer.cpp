#include "dbpch.h"

#include "Daybreak/Renderer/UniformBuffer.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Daybreak
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		return CreateRef<OpenGLUniformBuffer>(size, binding);
	}
}