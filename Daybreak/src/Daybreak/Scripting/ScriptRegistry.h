#pragma once

#include "dbpch.h"


namespace Daybreak
{
	class Script;

	class ScriptRegistry
	{
	public:
#define DB_REGISTER_SCRIPT(Type) Daybreak::ScriptRegistry::RegisterType<Type>()

		template<typename T>
		static void RegisterType()
		{
			s_Registry[DB_STRUCT_NAME(T)] = []()
			{
				return static_cast<Script*>(new T());
			};
		}

		static Script* GetRegisteredType(const std::string& typeName)
		{
			auto it = s_Registry.find(typeName);
			if (it != s_Registry.end())
			{
				return it->second();
			}
			DB_CORE_ASSERT(false, "Script {} is not registered!", typeName);
			return nullptr;
		}

		static const std::unordered_map<std::string, std::function<Script*()>>& GetRegistry() { return s_Registry; }

	private:
		static std::unordered_map<std::string, std::function<Script*()>> s_Registry;
	};
}