#pragma once

#include "Daybreak/Core/DeltaTime.h"
#include "Daybreak/Physics/DebugDraw.h"

#include <glm/glm.hpp>

class b2World;

namespace Daybreak
{
	class Entity;
	class Scene;
	class ContactListener;
	class ContactFilter;
	struct Rigidbody2DComponent;
	struct BoxCollider2DComponent;
	struct CircleCollider2DComponent;
	struct PolygonCollider2DComponent;

	class PhysicsSim2D
	{
	public:
		void InitSimulation(Scene* scene);
		void FixedStepSimulation();
		void ShutdownSimulation();

		void InitBody(const Entity& entity);

		void AddBoxFixture(const Entity& entity,
						   BoxCollider2DComponent& bc2d,
						   const Rigidbody2DComponent& rb2d,
						   const glm::vec3& worldScale,
						   const glm::vec3& worldPosition,
						   const glm::vec3& rbWorldEntityPosition);

		void AddCircleFixture(const Entity& entity,
							  CircleCollider2DComponent& cc2d,
							  const Rigidbody2DComponent& rb2d,
							  const glm::vec3& worldScale,
							  const glm::vec3& worldPosition,
							  const glm::vec3& rbWorldEntityPosition);

		void AddPolygonFixture(const Entity& entity,
							   PolygonCollider2DComponent& pc2d,
							   const Rigidbody2DComponent& rb2d,
							   const glm::vec3& worldScale,
							   const glm::vec3& worldPosition,
							   const glm::vec3& rbWorldEntityPosition);

		void AddBoxFixtureNoBody(const Entity& entity);
		void AddCircleFixtureNoBody(const Entity& entity);
		void AddPolygonFixtureNoBody(const Entity& entity);

		void RemoveEntity(Entity& entity);

		bool RayCast(const glm::vec2& startPos, const glm::vec2& endPos);
		bool RayCast(const glm::vec2& startPos, const glm::vec2& directionNorm, float depth);

		void AddForce(const Rigidbody2DComponent& rb2d, const glm::vec2& force, const glm::vec2& point);
		void AddForce(const Rigidbody2DComponent& rb2d, const glm::vec2& force);
		void AddTorque(const Rigidbody2DComponent& rb2d, float torque);

		void AddLinearImpulse(const Rigidbody2DComponent& rb2d, const glm::vec2& impulse, const glm::vec2& point);
		void AddLinearImpulse(const Rigidbody2DComponent& rb2d, const glm::vec2& impulse);
		void AddAngularImpulse(const Rigidbody2DComponent& rb2d, float impulse);

		void DebugDraw();

		// TODO: Rework into a project class
		static PhysicsSim2D* GetActiveSim()
		{
			return s_ActiveSim;
		}

	private:
		Scene* m_Scene;
		DebugRenderer* m_DebugRenderer;
		b2World* m_PhysicsWorld;
		ContactListener* m_ContactLitener;
		ContactFilter* m_ContactFilter;

		float m_LastUpdateTime = 0.0f;

	private:
		static PhysicsSim2D* s_ActiveSim;
	};

}
