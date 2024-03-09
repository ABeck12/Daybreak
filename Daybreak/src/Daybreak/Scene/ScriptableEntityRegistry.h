#pragma once

#include "dbpch.h"


namespace Daybreak
{
	class ScriptableEntity;

	class ScriptableEntityRegistry
	{
	public:
#define DB_REGISTER_SCRIPTABLE_ENTITY(Type) Daybreak::ScriptableEntityRegistry::RegisterType<Type>()

		template<typename T>
		static void RegisterType()
		{
			s_Registry[std::string(typeid(T).name()).erase(0, 6)] = []()
			{
				return static_cast<ScriptableEntity*>(new T());
			};
		}

		static ScriptableEntity* GetRegisteredType(const std::string& typeName)
		{
			auto it = s_Registry.find(typeName);
			if (it != s_Registry.end())
			{
				return it->second();
			}
			DB_CORE_ASSERT(false, "ScriptableEntity is not registered!");
			return nullptr;
		}

		static const std::unordered_map<std::string, std::function<ScriptableEntity*()>>& GetRegistry() { return s_Registry; }

	private:
		static std::unordered_map<std::string, std::function<ScriptableEntity*()>> s_Registry;
	};
}