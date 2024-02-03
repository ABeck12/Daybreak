#include "dbpch.h"

#include "Daybreak/Physics/Physics2D.h"
#include "Daybreak/Physics/Physics2DUtils.h"

#include "box2d/box2d.h"

namespace Daybreak
{
	void Physics2D::InitSimulation()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.82f });
	}

	void Physics2D::ShutdownSimulation()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Physics2D::StepSimulation(DeltaTime dt)
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);
	}

	void Physics2D::AddEntity(Entity& entity)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

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

			body->CreateFixture(&fixtureDef);

			b2MassData massData;
			massData.mass = rb2d.Mass;
			massData.center = b2Vec2(transform.Position.x + bc2d.Offset.x, transform.Position.y + bc2d.Offset.y);
			float I = rb2d.Mass * ((bc2d.Size.x * bc2d.Size.x) / 12 + (bc2d.Size.y * bc2d.Size.y) / 12);
			massData.I = I;

			body->SetMassData(&massData);
		}
	}
}