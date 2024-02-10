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
	void PhysicsSim2D::InitSimulation(Scene* scene)
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		m_ContactLitener = new ContactListener();
		m_ContactLitener->SetScene(scene);
		m_PhysicsWorld->SetContactListener(m_ContactLitener);
	}

	void PhysicsSim2D::ShutdownSimulation()
	{
		delete m_PhysicsWorld;
		delete m_ContactLitener;
		m_ContactLitener = nullptr;
		m_PhysicsWorld = nullptr;
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

			fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(m_ContactLitener->GetScene());
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

	void ContactListener::BeginContact(b2Contact* contact)
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
	
	void ContactListener::EndContact(b2Contact* contact)
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
}