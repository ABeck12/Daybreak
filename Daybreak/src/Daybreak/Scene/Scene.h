#pragma once

#include "dbpch.h"

#include <entt.hpp>

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Scene/Components.h"

namespace Daybreak
{
	// forward decleration
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnSceneUpdate(DeltaTime dt);

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(const Entity& entity);

		Entity GetActiveCameraEntity();

	private:
		void DebugRender();
		void RenderSpriteComponents();

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}
