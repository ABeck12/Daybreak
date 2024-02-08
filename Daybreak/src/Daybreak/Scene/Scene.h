#pragma once

#include "dbpch.h"

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Scene/Components.h"
#include "Daybreak/Physics/PhysicsSim2D.h"

#include <entt.hpp>

namespace Daybreak
{
	// forward decleration
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnRuntimeStart();
		void OnRuntimeUpdate(DeltaTime dt);
		void OnRuntimeEnd();

		Entity CreateEntity(const std::string& name = std::string());
		// Entity CreateEntity(Entity& parent, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Entity GetActiveCameraEntity();

		Entity GetEntityByUUID(UUID uuid);
		Entity FindEntityByName(std::string_view name);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

	private:
		inline void RenderScene();

		inline void OnPhysicsStart();
		inline void OnPhysicsUpdate(DeltaTime dt);
		inline void OnPhysicsStop();

	private:
		entt::registry m_Registry;
		PhysicsSim2D m_PhysicsSim2D;
		float m_LastUpdateTime = 0.0f;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
	};
}
