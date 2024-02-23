#pragma once

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Scene/Components.h"

class b2World;

namespace Daybreak
{
	class Entity;
	class Scene;
	class ContactListener;
	class ContactFilter;

	class PhysicsSim2D
	{
	public:
		void InitSimulation(Scene* scene);
		void FixedStepSimulation();
		void ShutdownSimulation();

		void AddEntity(Entity& entity);

		bool RayCast(const glm::vec2& startPos, const glm::vec2& endPos);
		bool RayCast(const glm::vec2& startPos, const glm::vec2& directionNorm, const float& depth);

	private:
		Scene* m_Scene;
		b2World* m_PhysicsWorld;
		ContactListener* m_ContactLitener;
		ContactFilter* m_ContactFilter;

		float m_LastUpdateTime = 0.0f;
	};
}
