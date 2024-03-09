#include "dbpch.h"

#include "Daybreak/Scene/ScriptableEntityRegistry.h"

namespace Daybreak
{
	std::unordered_map<std::string, std::function<ScriptableEntity*()>> ScriptableEntityRegistry::s_Registry;
}