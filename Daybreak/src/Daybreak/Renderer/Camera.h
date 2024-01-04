#pragma once

#include <glm/glm.hpp>

namespace Daybreak
{
	class Camera
	{
	public:
		Camera(glm::mat4 projection) : m_Projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	private:
		glm::mat4 m_Projection;
	};
}
