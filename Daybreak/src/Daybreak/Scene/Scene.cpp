#include "dbpch.h"

#include "Daybreak/Scene/Scene.h"

#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Renderer/Renderer2D.h"
#include "Daybreak/Physics/Physics2DUtils.h"
#include "Daybreak/Core/Time.h"

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
		// entity.AddComponent<RelationshipComponent>();

		return entity;
	}

	// Entity Scene::CreateEntity(Entity& parent, const std::string& name)
	// {
	// 	Entity child = CreateEntity(name);
	// 	auto& childRc = child.GetComponent<RelationshipComponent>();
	// 	childRc.ParentEntity = &parent;

	// 	auto& parentRc = parent.GetComponent<RelationshipComponent>();
	// 	parentRc.ChildrenEntities.emplace_back(&child);

	// 	return child;
	// }

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

		float startTime = Time::GetTime();
		while (m_LastUpdateTime < startTime)
		{
			m_PhysicsSim2D.FixedStepSimulation();
			m_LastUpdateTime += 0.016f; // For now this is the fixed delta time
		}

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
		m_PhysicsSim2D.InitSimulation();

		auto view = m_Registry.view<Rigidbody2DComponent>();

		for (auto e : view)
		{
			Entity entity = { e, this };
			m_PhysicsSim2D.AddEntity(entity);
		}
	}

	void Scene::OnPhysicsStop()
	{
		m_PhysicsSim2D.ShutdownSimulation();
	}
}