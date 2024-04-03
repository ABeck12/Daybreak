#pragma once

#include "Daybreak/Core/DeltaTime.h"

#include <glm/glm.hpp>

class b2World;

namespace Daybreak
{
	class Entity;
	class Scene;
	class ContactListener;
	class ContactFilter;
	struct Rigidbody2DComponent;

	class PhysicsSim2D
	{
	public:
		void InitSimulation(Scene* scene);
		void FixedStepSimulation();
		void ShutdownSimulation();

		void AddBoxCollider(Entity& entity);
		void AddCircleCollider(Entity& entity);

		void RemoveEntity(Entity& entity);

		bool RayCast(const glm::vec2& startPos, const glm::vec2& endPos);
		bool RayCast(const glm::vec2& startPos, const glm::vec2& directionNorm, float depth);

		void AddForce(const Rigidbody2DComponent& rb2d, const glm::vec2& force, const glm::vec2& point);
		void AddForce(const Rigidbody2DComponent& rb2d, const glm::vec2& force);
		void AddTorque(const Rigidbody2DComponent& rb2d, float torque);

		void AddLinearImpulse(const Rigidbody2DComponent& rb2d, const glm::vec2& impulse, const glm::vec2& point);
		void AddLinearImpulse(const Rigidbody2DComponent& rb2d, const glm::vec2& impulse);
		void AddAngularImpulse(const Rigidbody2DComponent& rb2d, float impulse);

		// TODO: Rework into a project class
		static PhysicsSim2D* GetActiveSim() { return s_ActiveSim; }

	private:
		Scene* m_Scene;
		b2World* m_PhysicsWorld;
		ContactListener* m_ContactLitener;
		ContactFilter* m_ContactFilter;

		float m_LastUpdateTime = 0.0f;

	private:
		// Rework into a project class
		static void SetActiveSim(PhysicsSim2D* sim) { s_ActiveSim = sim; }
		static PhysicsSim2D* s_ActiveSim;

		friend Scene;
	};

}
