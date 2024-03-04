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

		void AddColliderWithRigidbody(Entity& entity);
		void AddCollider(Entity& entity);

		bool RayCast(const glm::vec2& startPos, const glm::vec2& endPos);
		bool RayCast(const glm::vec2& startPos, const glm::vec2& directionNorm, const float& depth);

		void AddForce(const Rigidbody2DComponent& rb2d, const glm::vec2& force, const glm::vec2& point);
		void AddForce(const Rigidbody2DComponent& rb2d, const glm::vec2& force);
		void AddTorque(const Rigidbody2DComponent& rb2d, const float& torque);

		void AddLinearImpulse(const Rigidbody2DComponent& rb2d, const glm::vec2& impulse, const glm::vec2& point);
		void AddLinearImpulse(const Rigidbody2DComponent& rb2d, const glm::vec2& impulse);
		void AddAngularImpulse(const Rigidbody2DComponent& rb2d, const float& impulse);

	private:
		Scene* m_Scene;
		b2World* m_PhysicsWorld;
		ContactListener* m_ContactLitener;
		ContactFilter* m_ContactFilter;

		float m_LastUpdateTime = 0.0f;
	};
}

