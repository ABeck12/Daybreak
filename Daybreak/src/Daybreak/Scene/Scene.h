#pragma once

#include "dbpch.h"

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Scene/Components.h"

#include <entt.hpp>

// forward decleration
class b2World;

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
		void DestroyEntity(const Entity& entity);

		Entity GetActiveCameraEntity();

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
		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
	};
}
