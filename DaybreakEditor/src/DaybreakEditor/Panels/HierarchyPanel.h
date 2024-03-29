#pragma once

#include <Daybreak.h>

namespace Daybreak
{
	class HierarchyPannel
	{
	public:
		HierarchyPannel() = default;
		HierarchyPannel(const Ref<Scene>& scene);

		void Render();
		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

	private:
		void DrawEntityRow(Entity entity);
		void DrawInspectorComponents(Entity entity);

	private:
		Entity m_SelectionContext;
		Ref<Scene> m_ActiveScene;
	};
}