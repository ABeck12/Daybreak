#include "dbpch.h"

#include "Daybreak/Renderer/RenderAPI.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Daybreak
{
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	Scope<RenderAPI> RenderAPI::Create()
	{
		return CreateScope<OpenGLRenderAPI>();
	}
}