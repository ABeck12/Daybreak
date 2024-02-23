#include "dbpch.h"

#include "Daybreak/Physics/PhysicsSim2D.h"

#include "Daybreak/Scene/Scene.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Physics/Physics2DUtils.h"
#include "Daybreak/Scene/ScriptableEntity.h"
#include "Daybreak/Core/UUID.h"

#include "box2d/box2d.h"

namespace Daybreak
{
	class ContactListener : public b2ContactListener
	{
		void BeginContact(b2Contact* contact)
		{
			Scene* scene = (Scene*)contact->GetFixtureA()->GetUserData().pointer;
			Entity entityA = scene->GetEntityByUUID((UUID)contact->GetFixtureA()->GetUserData().uuid);
			Entity entityB = scene->GetEntityByUUID((UUID)contact->GetFixtureB()->GetUserData().uuid);

			if (entityA.HasComponent<NativeScriptComponent>())
			{
				auto& nsc = entityA.GetComponent<NativeScriptComponent>();
				nsc.Instance->OnCollisionEnter(entityB);
			}
			if (entityB.HasComponent<NativeScriptComponent>())
			{
				auto& nsc = entityB.GetComponent<NativeScriptComponent>();
				nsc.Instance->OnCollisionEnter(entityA);
			}
		}

		void EndContact(b2Contact* contact)
		{
			Scene* scene = (Scene*)contact->GetFixtureA()->GetUserData().pointer;
			Entity entityA = scene->GetEntityByUUID((UUID)contact->GetFixtureA()->GetUserData().uuid);
			Entity entityB = scene->GetEntityByUUID((UUID)contact->GetFixtureB()->GetUserData().uuid);

			if (entityA.HasComponent<NativeScriptComponent>())
			{
				auto& nsc = entityA.GetComponent<NativeScriptComponent>();
				nsc.Instance->OnCollisionExit(entityB);
			}
			if (entityB.HasComponent<NativeScriptComponent>())
			{
				auto& nsc = entityB.GetComponent<NativeScriptComponent>();
				nsc.Instance->OnCollisionExit(entityA);
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
			return entityA.GetComponent<BoxCollider2DComponent>().CollisionLayer == entityB.GetComponent<BoxCollider2DComponent>().CollisionLayer;
		}
	};

	class RaycastCallback : public b2RayCastCallback
	{
	public:
		bool m_Hit;

		RaycastCallback() : m_Hit(false) {}

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
		delete m_Scene;
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

	void PhysicsSim2D::AddEntity(Entity& entity)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		auto uuid = entity.GetUUID();

		b2BodyDef bodyDef;
		bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
		bodyDef.position.Set(transform.Position.x, transform.Position.y);
		bodyDef.angle = transform.Rotation.z;
		bodyDef.bullet = rb2d.ContinuousDetection;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.FixedRotation);
		body->SetGravityScale(rb2d.GravityScale);
		body->SetAngularDamping(rb2d.AngularDrag);
		body->SetLinearDamping(rb2d.LinearDrag);

		rb2d.RuntimeBody = body;

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

			b2PolygonShape boxShape;
			boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);
	
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = rb2d.Density;
			fixtureDef.friction = rb2d.Friction;
			fixtureDef.restitution = rb2d.Restitution;
			fixtureDef.restitutionThreshold = rb2d.RestitutionThreshold;
			fixtureDef.isSensor = bc2d.IsTrigger;

			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(m_Scene);
			fixtureDef.userData.uuid = entity.GetUUID();

			b2Fixture* fixture = body->CreateFixture(&fixtureDef);
			bc2d.RuntimeFixture = fixture;
			bc2d.RuntimeBody = body;

			b2MassData massData;
			massData.mass = rb2d.Mass;
			massData.center = b2Vec2(transform.Position.x + bc2d.Offset.x, transform.Position.y + bc2d.Offset.y);
			float I = rb2d.Mass * ((bc2d.Size.x * bc2d.Size.x) / 12 + (bc2d.Size.y * bc2d.Size.y) / 12);
			massData.I = I;

			body->SetMassData(&massData);
		}
	}

	bool PhysicsSim2D::RayCast(const glm::vec2& startPos, const glm::vec2& endPos)
	{
		RaycastCallback callback;
		m_PhysicsWorld->RayCast(&callback, { startPos.x, startPos.y }, { endPos.x, endPos.y });

		return callback.m_Hit;
	}

	bool PhysicsSim2D::RayCast(const glm::vec2& startPos, const glm::vec2& directionNorm, const float& depth)
	{
		glm::vec2 endPos;
		float angle = atan(directionNorm.y / directionNorm.x);
		endPos.x = startPos.x + depth * cos(angle);
		endPos.y = startPos.y + depth * sin(angle);

		return RayCast(startPos, endPos);
	}
}