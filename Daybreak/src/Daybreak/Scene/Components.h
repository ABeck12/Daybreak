#pragma once

#include "dbpch.h"

namespace Daybreak
{
	struct TransformComponent
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
	};

	struct TagComponent
	{
		std::string Tag;
	};
}