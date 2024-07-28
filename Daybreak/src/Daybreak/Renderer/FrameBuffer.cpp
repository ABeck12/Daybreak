#include "dbpch.h"

#include "Daybreak/Renderer/FrameBuffer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Daybreak
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecifications& spec)
	{
		return CreateRef<OpenGLFrameBuffer>(spec);
	}
}
