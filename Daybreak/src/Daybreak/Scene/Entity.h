#pragma once

#include <entt.hpp>

namespace Daybreak
{
	class Entity
	{
	public:
		Entity() = default;

		//template<typename T>
		//T& GetComponent()
		//{
		//	DB_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
		//	return m_Scene->m_Registry.get<T>(m_EntityHandle);
		//}
	private:
		entt::entity m_EntityHandle{ entt::null };
	};
}
