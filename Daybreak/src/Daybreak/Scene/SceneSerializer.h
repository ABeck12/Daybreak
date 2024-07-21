#pragma once

#include <filesystem>

#include "Daybreak/Scene/Scene.h"

namespace Daybreak
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::filesystem::path& filepath) const;
		bool Deserialize(const std::filesystem::path& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}