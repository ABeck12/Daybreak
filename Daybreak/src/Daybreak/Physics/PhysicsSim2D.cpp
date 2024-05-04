#include "dbpch.h"

#include "Daybreak/Physics/PhysicsSim2D.h"

#include "Daybreak/Scene/Scene.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Scripting/Script.h"
#include "Daybreak/Scene/Components.h"
#include "Daybreak/Core/UUID.h"

#include "box2d/box2d.h"

namespace Daybreak
{
	PhysicsSim2D* PhysicsSim2D::s_ActiveSim = nullptr;

	namespace Utils
	{
		inline b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
		{
			switch (bodyType)
			{
				case Rigidbody2DComponent::BodyType::Static: return b2_staticBody;
				case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
				case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
			}

			DB_CORE_ASSERT(false, "Unknown body type");
			return b2_staticBody;
		}

		inline Rigidbody2DComponent::BodyType Rigidbody2DTypeFromBox2DBody(b2BodyType bodyType)
		{
			switch (bodyType)
			{
				case b2_staticBody: return Rigidbody2DComponent::BodyType::Static;
				case b2_dynamicBody: return Rigidbody2DComponent::BodyType::Dynamic;
				case b2_kinematicBody: return Rigidbody2DComponent::BodyType::Kinematic;
			}

			DB_CORE_ASSERT(false, "Unknown body type");
			return Rigidbody2DComponent::BodyType::Static;
		}
	}

	class ContactListener : public b2ContactListener
	{
		void BeginContact(b2Contact* contact)
		{
			Scene* scene = (Scene*)contact->GetFixtureA()->GetUserData().pointer;
			Entity entityA = scene->GetEntityByUUID((UUID)contact->GetFixtureA()->GetUserData().uuid);
			Entity entityB = scene->GetEntityByUUID((UUID)contact->GetFixtureB()->GetUserData().uuid);

			if (entityA.HasComponent<ScriptComponent>())
			{
				auto& sc = entityA.GetComponent<ScriptComponent>();
				sc.Instance->OnCollisionEnter(entityB);
			}
			if (entityB.HasComponent<ScriptComponent>())
			{
				auto& sc = entityB.GetComponent<ScriptComponent>();
				sc.Instance->OnCollisionEnter(entityA);
			}
		}

		void EndContact(b2Contact* contact)
		{
			Scene* scene = (Scene*)contact->GetFixtureA()->GetUserData().pointer;
			Entity entityA = scene->GetEntityByUUID((UUID)contact->GetFixtureA()->GetUserData().uuid);
			Entity entityB = scene->GetEntityByUUID((UUID)contact->GetFixtureB()->GetUserData().uuid);

			if (entityA.HasComponent<ScriptComponent>())
			{
				auto& sc = entityA.GetComponent<ScriptComponent>();
				sc.Instance->OnCollisionExit(entityB);
			}
			if (entityB.HasComponent<ScriptComponent>())
			{
				auto& sc = entityB.GetComponent<ScriptComponent>();
				sc.Instance->OnCollisionExit(entityA);
			}
		}
	};

	class ContactFilter : public b2ContactFilter
	{
		bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override
		{
			Scene* scene = (Scene*)fixtureA->GetUserData().pointer;
			Entity entityA = scene->GetEntityByUUID((UUID)fixtureA->GetUserData().uuid);
			Entity entityB = scene->GetEntityByUUID((UUID)fixtureB->GetUserData().uuid);

			if (entityA.HasComponent<BoxCollider2DComponent>() && entityB.HasComponent<BoxCollider2DComponent>())
			{
				return entityA.GetComponent<BoxCollider2DComponent>().CollisionLayer == entityB.GetComponent<BoxCollider2DComponent>().CollisionLayer;
			}
			else if (entityA.HasComponent<BoxCollider2DComponent>() && entityB.HasComponent<CircleCollider2DComponent>())
			{
				return entityA.GetComponent<BoxCollider2DComponent>().CollisionLayer == entityB.GetComponent<CircleCollider2DComponent>().CollisionLayer;
			}
			else if (entityA.HasComponent<CircleCollider2DComponent>() && entityB.HasComponent<BoxCollider2DComponent>())
			{
				return entityA.GetComponent<CircleCollider2DComponent>().CollisionLayer == entityB.GetComponent<BoxCollider2DComponent>().CollisionLayer;
			}
			else if (entityA.HasComponent<CircleCollider2DComponent>() && entityB.HasComponent<CircleCollider2DComponent>())
			{
				return entityA.GetComponent<CircleCollider2DComponent>().CollisionLayer == entityB.GetComponent<CircleCollider2DComponent>().CollisionLayer;
			}
			return false;
		}
	};

	class RaycastCallback : public b2RayCastCallback
	{
	public:
		bool m_Hit;

		RaycastCallback()
			: m_Hit(false) {}

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
		{
			m_Hit = true;
			return fraction;
		}
	};

	void PhysicsSim2D::InitSimulation(Scene* scene)
	{
		m_Scene = scene;
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		m_ContactLitener = new ContactListener();
		m_ContactFilter = new ContactFilter();
		m_PhysicsWorld->SetContactListener(m_ContactLitener);
		m_PhysicsWorld->SetContactFilter(m_ContactFilter);
	}

	void PhysicsSim2D::ShutdownSimulation()
	{
		delete m_PhysicsWorld;
		delete m_ContactLitener;
		delete m_ContactFilter;
		m_ContactFilter = nullptr;
		m_ContactLitener = nullptr;
		m_PhysicsWorld = nullptr;
		m_Scene = nullptr;
	}

	void PhysicsSim2D::FixedStepSimulation()
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_PhysicsWorld->Step(0.016f, velocityIterations, positionIterations); // For now this is the fixed delta time
	}

	void PhysicsSim2D::AddBoxCollider(Entity& entity)
	{
		auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();
		auto uuid = entity.GetUUID();

		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;

		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(transform.Position.x, transform.Position.y);
		bodyDef.angle = transform.Rotation.z;
		bodyDef.bullet = true;
		bodyDef.allowSleep = false;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);

		fixtureDef.shape = &boxShape;
		fixtureDef.isSensor = bc2d.IsTrigger;
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(m_Scene);
		fixtureDef.userData.uuid = entity.GetUUID();

		b2Fixture* fixture = body->CreateFixture(&fixtureDef);
		bc2d.RuntimeFixture = fixture;
		bc2d.RuntimeBody = body;

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			body->SetType(Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type));
			body->SetBullet(rb2d.ContinuousDetection);
			body->SetSleepingAllowed(rb2d.AllowSleep);
			body->SetFixedRotation(rb2d.FixedRotation);
			body->SetGravityScale(rb2d.GravityScale);
			body->SetAngularDamping(rb2d.AngularDrag);
			body->SetLinearDamping(rb2d.LinearDrag);
			rb2d.RuntimeBody = body;

			fixture->SetDensity(rb2d.Density);
			fixture->SetFriction(rb2d.Friction);
			fixture->SetRestitution(rb2d.Restitution);
			fixture->SetRestitutionThreshold(rb2d.RestitutionThreshold);

			b2MassData massData;
			massData.mass = rb2d.Mass;
			massData.center = b2Vec2(bc2d.Offset.x * transform.Scale.x, bc2d.Offset.y * transform.Scale.y);
			massData.I = rb2d.Mass * ((bc2d.Size.x * bc2d.Size.x) / 12 + (bc2d.Size.y * bc2d.Size.y) / 12);
			body->SetMassData(&massData);
		}
	}

	void PhysicsSim2D::AddCircleCollider(Entity& entity)
	{
		auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();
		auto uuid = entity.GetUUID();

		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;

		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(transform.Position.x, transform.Position.y);
		bodyDef.angle = transform.Rotation.z;
		bodyDef.bullet = true;
		bodyDef.allowSleep = false;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);

		b2CircleShape circleShape;
		circleShape.m_p.Set(cc2d.Offset.x * transform.Scale.x, cc2d.Offset.y * transform.Scale.y);
		circleShape.m_radius = cc2d.Radius;

		fixtureDef.shape = &circleShape;
		fixtureDef.isSensor = cc2d.IsTrigger;
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(m_Scene);
		fixtureDef.userData.uuid = entity.GetUUID();

		b2Fixture* fixture = body->CreateFixture(&fixtureDef);
		cc2d.RuntimeFixture = fixture;
		cc2d.RuntimeBody = body;

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			body->SetType(Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type));
			body->SetBullet(rb2d.ContinuousDetection);
			body->SetSleepingAllowed(rb2d.AllowSleep);
			body->SetFixedRotation(rb2d.FixedRotation);
			body->SetGravityScale(rb2d.GravityScale);
			body->SetAngularDamping(rb2d.AngularDrag);
			body->SetLinearDamping(rb2d.LinearDrag);
			rb2d.RuntimeBody = body;

			fixture->SetDensity(rb2d.Density);
			fixture->SetFriction(rb2d.Friction);
			fixture->SetRestitution(rb2d.Restitution);
			fixture->SetRestitutionThreshold(rb2d.RestitutionThreshold);

			b2MassData massData;
			massData.mass = rb2d.Mass;
			massData.center = b2Vec2(cc2d.Offset.x * transform.Scale.x, cc2d.Offset.y * transform.Scale.y);
			massData.I = 0.5f * rb2d.Mass * cc2d.Radius * cc2d.Radius;
			body->SetMassData(&massData);
		}
	}

	void PhysicsSim2D::RemoveEntity(Entity& entity)
	{
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto bc2d = entity.GetComponent<BoxCollider2DComponent>();
			m_PhysicsWorld->DestroyBody((b2Body*)bc2d.RuntimeBody);
			bc2d.RuntimeBody = nullptr;
			bc2d.RuntimeFixture = nullptr;
		}
		else if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto cc2d = entity.GetComponent<CircleCollider2DComponent>();
			m_PhysicsWorld->DestroyBody((b2Body*)cc2d.RuntimeBody);
			cc2d.RuntimeBody = nullptr;
			cc2d.RuntimeFixture = nullptr;
		}
		else if (entity.HasComponent<Rigidbody2DComponent>())
		{
			auto rb2d = entity.GetComponent<Rigidbody2DComponent>();
			if (rb2d.RuntimeBody)
			{
				m_PhysicsWorld->DestroyBody((b2Body*)rb2d.RuntimeBody);
				rb2d.RuntimeBody = nullptr;
			}
		}
	}

	bool PhysicsSim2D::RayCast(const glm::vec2& startPos, const glm::vec2& endPos)
	{
		RaycastCallback callback;
		m_PhysicsWorld->RayCast(&callback, { startPos.x, startPos.y }, { endPos.x, endPos.y });

		return callback.m_Hit;
	}

	bool PhysicsSim2D::RayCast(const glm::vec2& startPos, const glm::vec2& directionNorm, float depth)
	{
		glm::vec2 endPos;
		float angle = atan(directionNorm.y / directionNorm.x);
		endPos.x = startPos.x + depth * cos(angle);
		endPos.y = startPos.y + depth * sin(angle);

		return RayCast(startPos, endPos);
	}

	void PhysicsSim2D::AddForce(const Rigidbody2DComponent& rb2d, const glm::vec2& force, const glm::vec2& point)
	{
		((b2Body*)rb2d.RuntimeBody)->ApplyForce({ force.x, force.y }, { point.x, point.y }, true);
	}

	void PhysicsSim2D::AddForce(const Rigidbody2DComponent& rb2d, const glm::vec2& force)
	{
		((b2Body*)rb2d.RuntimeBody)->ApplyForceToCenter({ force.x, force.y }, true);
	}

	void PhysicsSim2D::AddTorque(const Rigidbody2DComponent& rb2d, float torque)
	{
		((b2Body*)rb2d.RuntimeBody)->ApplyTorque(torque, true);
	}

	void PhysicsSim2D::AddLinearImpulse(const Rigidbody2DComponent& rb2d, const glm::vec2& impulse, const glm::vec2& point)
	{
		((b2Body*)rb2d.RuntimeBody)->ApplyLinearImpulse({ impulse.x, impulse.y }, { point.x, point.y }, true);
	}

	void PhysicsSim2D::AddLinearImpulse(const Rigidbody2DComponent& rb2d, const glm::vec2& impulse)
	{
		((b2Body*)rb2d.RuntimeBody)->ApplyLinearImpulseToCenter({ impulse.x, impulse.y }, true);
	}

	void PhysicsSim2D::AddAngularImpulse(const Rigidbody2DComponent& rb2d, float impulse)
	{
		((b2Body*)rb2d.RuntimeBody)->ApplyAngularImpulse(impulse, true);
	}
}