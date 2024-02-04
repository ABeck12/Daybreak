#pragma once

#include "Daybreak/Core/DeltaTime.h"

// #include "box2d/box2d.h"
class b2World;

namespace Daybreak
{
	class Entity;

	class PhysicsSim2D
	{
	public:
		void InitSimulation();
		void FixedStepSimulation();
		void ShutdownSimulation();

		void AddEntity(Entity& entity);

		bool OverlapCollider();
		// glm::vec2 RayCast(const glm::vec2& origin, const glm::vec2& direction, const float& depth);

	private:
		// bool ContactFilter()

	private:
		float m_LastUpdateTime = 0.0f;
		b2World* m_PhysicsWorld;
	};
}

