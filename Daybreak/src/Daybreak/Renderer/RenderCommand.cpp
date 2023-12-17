#include "dbpch.h"

#include "Daybreak/Renderer/RenderCommand.h"

namespace Daybreak
{

	Scope<RenderAPI> RenderCommand::s_RenderAPI = RenderAPI::Create();

}