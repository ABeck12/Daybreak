#include "dbpch.h"

#include "Daybreak/Renderer/VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Daybreak
{
	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<OpenGLVertexArray>();
	}
}