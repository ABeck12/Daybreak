#pragma once

#include "Daybreak/Scene/Scene.h"

namespace Daybreak
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void Deserialize(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
    };
}