#pragma once

#include "dbpch.h"

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Physics/PhysicsSim2D.h"
#include "Daybreak/Core/UUID.h"

#include <entt.hpp>

namespace Daybreak
{
	// forward decleration
	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& name = "Untitled");
		~Scene();

		void OnRuntimeStart();
		void OnRuntimeUpdate(DeltaTime dt);
		void OnRuntimeEnd();

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

		const std::string& GetName() { return m_SceneName; }

		// void EditorRenderScene(Entity& editorCameraEntity);
		void RenderScene(const Entity& cameraEntity);

		void SetStartTime(float time) { m_LastUpdateTime = time; }

	private:
		inline void OnPhysicsStart();
		inline void OnPhysicsUpdate(DeltaTime dt);
		inline void OnPhysicsStop();

		inline glm::mat4 GetWorldTransform(Entity& entity);

		template<typename T>
		Entity& GetParentEntityWith(Entity& entity);

		template<typename T>
		bool HasParentEntityWith(Entity& entity);

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
	};
}
