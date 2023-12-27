#include "dbpch.h"

#include "Daybreak/Renderer/Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Daybreak
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}

	Ref<IndexBuffer> IndexBuffer::Create(const void* indices, uint32_t count)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, count);
	}
}