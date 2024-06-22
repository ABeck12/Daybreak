#pragma once

#include <glm/glm.hpp>

namespace Daybreak
{
	// enum class CameraType
	//{
	//	Orthographic, Perspective
	// };

	class Camera
	{
	public:
		Camera() = default;
		Camera(glm::mat4 projection)
			: m_Projection(projection) {}
		virtual ~Camera() = default;

		const inline glm::mat4& GetProjection() const { return m_Projection; }
		inline void SetProjection(const glm::mat4& projection) { m_Projection = projection; }

	private:
		glm::mat4 m_Projection;
	};
}
