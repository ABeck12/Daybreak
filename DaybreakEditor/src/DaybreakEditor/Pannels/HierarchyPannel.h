#pragma once

#include <Daybreak.h>

namespace Daybreak
{
	class HierarchyPannel
	{
	public:
		HierarchyPannel() = default;
		HierarchyPannel(const Ref<Scene> scene);

		void Render();
		// void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }
		bool HasSelectedEntity() { return m_SelectionContext; }
		glm::vec2 GetSelectedEntityPosition();
		void SetScene(const Ref<Scene> scene);

	private:
		void DrawEntityRow(Entity entity);
		void DrawInspectorComponents(Entity entity);

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

	private:
		Entity m_SelectionContext;
		std::string m_ActiveEntityName;
		Ref<Scene> m_ActiveScene;
	};
}