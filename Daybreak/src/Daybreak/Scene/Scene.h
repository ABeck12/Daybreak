#pragma once

#include <string>

#include "Daybreak/Core/Base.h"
#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Physics/PhysicsSim2D.h"
#include "Daybreak/Core/UUID.h"

#include <entt.hpp>

namespace Daybreak
{
	// forward decleration
	class Entity;
	class SceneRenderer;

	class Scene
	{
	public:
		Scene(const std::string& name = "Untitled");
		~Scene();

		void OnRuntimeStart();
		void OnRuntimeUpdate(DeltaTime dt);
		void OnRuntimeStop();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(Entity& parent, const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());

		static Ref<Scene> Copy(const Ref<Scene>& input);

		void DestroyEntity(Entity entity);

		Entity GetActiveCameraEntity();

		Entity GetEntityByUUID(UUID uuid);
		Entity GetEntityByName(std::string_view name);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		const std::string& GetName() const { return m_SceneName; }

		void OnRenderScene(const Entity& cameraEntity);
		void OnUpdateComponents(DeltaTime dt);

		void SetStartTime(float time) { m_LastUpdateTime = time; }

		void OnPhysicsStart();
		void OnPhysicsUpdate(DeltaTime dt);
		void OnPhysicsStop();

		void ToggleDebugDraw() { m_DebugDraw = !m_DebugDraw; }

		Scope<SceneRenderer> m_SceneRenderer;

	private:
		// TODO: move to entity class?==============
		inline glm::mat4 GetWorldTransform(Entity& entity);
		inline glm::vec3 GetWorldPosition(Entity& entity);
		inline glm::vec3 GetWorldScale(Entity& entity);

		template<typename T>
		Entity& GetParentEntityWith(Entity& entity);

		template<typename T>
		bool HasParentEntityWith(Entity& entity);
		// ========================================

		void DebugDraw();

	private:
		std::string m_SceneName;
		entt::registry m_Registry;
		PhysicsSim2D* m_PhysicsSim2D;
		float m_LastUpdateTime = 0.0f;
		bool m_DebugDraw = true;
		bool m_SceneRunning = false;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneSerializer;

		// For Editor
		friend class HierarchyPannel;
		friend class ViewportPannel;
	};
}
