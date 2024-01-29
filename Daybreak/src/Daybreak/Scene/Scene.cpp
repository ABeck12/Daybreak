#include "dbpch.h"

#include "Daybreak/Scene/Scene.h"

#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Renderer/Renderer2D.h"
#include "Daybreak/Physics/Physics2DUtils.h"

#include <box2d/box2d.h>

namespace Daybreak
{
	Scene::Scene() {}

	Scene::~Scene() {}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		OnPhysicsStart();
	}

	void Scene::OnRuntimeUpdate(DeltaTime dt)
	{
		OnPhysicsUpdate(dt);
		RenderScene();
	}

	void Scene::OnRuntimeEnd()
	{
		OnPhysicsStop();
	}

	Entity Scene::GetActiveCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		DB_CORE_ERROR("Scene has no camera component marked as primary!");
		return {};
	}

	void Scene::RenderScene()
	{
		auto cameraEntity = GetActiveCameraEntity();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().Position);
		glm::mat4 rotation = glm::toMat4(glm::quat(cameraEntity.GetComponent<TransformComponent>().Rotation));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().Scale);

		Renderer2D::BeginScene(cameraEntity.GetComponent<CameraComponent>().Camera, scale * rotation * translation);
		
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}
		Renderer2D::EndScene();
	}

	void Scene::OnPhysicsUpdate(DeltaTime dt)
	{
		// Set data for Box2D
		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			body->SetLinearVelocity({ rb2d.Velocity.x, rb2d.Velocity.y });
			body->SetTransform({ transform.Position.x, transform.Position.y }, transform.Rotation.z);
		}

		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);

		// Retrieve data from Box2D
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			const auto& position = body->GetPosition();
			transform.Position.x = position.x;
			transform.Position.y = position.y;
			transform.Rotation.z = body->GetAngle();

			const auto& velocity = body->GetLinearVelocity();
			rb2d.Velocity.x = velocity.x;
			rb2d.Velocity.y = velocity.y;
		}
	}

	void Scene::OnPhysicsStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.82f });

		auto view = m_Registry.view<Rigidbody2DComponent>();

		for (auto e : view)
		{
			Entity entity = { e, this };

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

	void Scene::OnPhysicsStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}
}