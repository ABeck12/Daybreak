#pragma once

#include <string>

#include "Daybreak/Core/Base.h"
#include "Daybreak/Scene/Scene.h"
#include "Daybreak/Core/UUID.h"

#include <entt.hpp>

namespace Daybreak
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) const
		{
			DB_CORE_ASSERT(!HasComponent<T>(), "Entity already has component {}!", typeid(T).name());
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		T& GetComponent() const
		{
			DB_CORE_ASSERT(HasComponent<T>(), "Entity does not have component {}!", typeid(T).name());
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent() const
		{
			DB_CORE_ASSERT(HasComponent<T>(), "Entity does not have component {}!", typeid(T).name());
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		const std::string& GetName() const;
		const UUID GetUUID() const;
		const bool IsActive() const;
		void SetActive(bool active);
		const bool HasParent() const;
		const UUID GetParentUUID() const;

	private:
		Scene* m_Scene = nullptr; // TODO: This could be turned into a weak ref system
		entt::entity m_EntityHandle { entt::null };
	};
}