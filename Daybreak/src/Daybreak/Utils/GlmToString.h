#pragma once

#include "dbpch.h"

#include <glm/gtx/string_cast.hpp>


inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
	return os << glm::to_string(vec);
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
	return os << glm::to_string(vec);
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
{
	return os << glm::to_string(vec);
}

inline std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
	return os << glm::to_string(mat);
}

inline std::ostream& operator<<(std::ostream& os, const glm::mat3& mat)
{
	return os << glm::to_string(mat);
}

inline std::ostream& operator<<(std::ostream& os, const glm::mat2& mat)
{
	return os << glm::to_string(mat);
}
