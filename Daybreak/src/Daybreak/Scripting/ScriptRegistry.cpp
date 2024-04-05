#include "dbpch.h"

#include "Daybreak/Scripting/ScriptRegistry.h"

namespace Daybreak
{
	std::unordered_map<std::string, std::function<Script*()>> ScriptRegistry::s_Registry;
}