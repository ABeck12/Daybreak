#include "dbpch.h"

#include "Daybreak/Scene/Scene.h"

#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Renderer/Renderer2D.h"
#include "Daybreak/Core/Time.h"
#include "Daybreak/Scene/ScriptableEntity.h"
#include "Daybreak/Renderer/RenderCommand.h"

#include <box2d/box2d.h>

// TEMPORARY
#include <glad/glad.h>
#include "Daybreak/Core/Input.h"

namespace Daybreak
{
	Scene::Scene(const std::string& name)
		: m_SceneName(name)
	{
	}

	Scene::~Scene() {}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();

		UUID uuid = UUID();
		entity.AddComponent<IDComponent>(uuid);
		m_EntityMap[uuid] = entity;
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		entity.AddComponent<RelationshipComponent>();

		return entity;
	}

	Entity Scene::CreateEntity(Entity& parent, const std::string& name)
	{
		Entity child = CreateEntity(name);
		auto& childRc = child.GetComponent<RelationshipComponent>();
		childRc.ParentID = parent.GetUUID();

		auto& parentRc = parent.GetComponent<RelationshipComponent>();
		parentRc.ChildrenIDs.emplace_back(child.GetUUID());
		parentRc.AmountOfChildren++;

		return child;
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<IDComponent>(uuid);
		m_EntityMap[uuid] = entity;
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		entity.AddComponent<RelationshipComponent>();

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		auto& entityRc = entity.GetComponent<RelationshipComponent>();
		if (entityRc.ParentID != 0)
		{
			Entity parent = GetEntityByUUID(entityRc.ParentID);
			auto& parentRc = parent.GetComponent<RelationshipComponent>();
			std::vector<UUID>::iterator position = std::find(parentRc.ChildrenIDs.begin(), parentRc.ChildrenIDs.end(), entity.GetUUID());
			if (position != parentRc.ChildrenIDs.end())
			{
				parentRc.ChildrenIDs.erase(position);
				parentRc.AmountOfChildren--;
			}
		}

		if (entity.HasComponent<Rigidbody2DComponent>() || entity.HasComponent<CircleCollider2DComponent>() || entity.HasComponent<BoxCollider2DComponent>())
		{
			m_PhysicsSim2D->RemoveEntity(entity);
		}

		if (entity.HasComponent<NativeScriptComponent>())
		{
			auto nsc = entity.GetComponent<NativeScriptComponent>();
			nsc.Instance->OnDestroy();
		}

		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
													  {
			// TODO: Move to Scene::OnScenePlay
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript(nsc.TypeName);
				nsc.Instance->m_Entity = Entity{ entity, this };
				nsc.Instance->OnCreate();
			} });

		OnPhysicsStart();
	}

	void Scene::OnRuntimeUpdate(DeltaTime dt)
	{
		{
			auto view = m_Registry.view<AnimatorComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& anim = entity.GetComponent<AnimatorComponent>();
				if (anim.IsPlaying)
				{
					anim.Controller->Update(dt);
				}
			}
		}

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
													  {
				// This runs here to catch any nsc that are instianted during scene play
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript(nsc.TypeName);
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(dt); });

		OnPhysicsUpdate(dt);
		RenderScene();
	}

	static void RenderShadows()
	{
	}

	void Scene::OnRuntimeEnd()
	{
		OnPhysicsStop();

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
													  {
				// TODO: Move to Scene::OnSceneStop
				// This NEEDS to be fixed. Not every nsc will have an active instance?						  
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript(nsc.TypeName);
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnDestroy();
				} });
	}

	Entity Scene::GetActiveCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity { entity, this };
		}
		DB_CORE_ERROR("Scene has no camera component marked as primary!");
		return {};
	}

	bool findIntersection(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec2& intersection)
	{
		// Calculate slope and y-intercept for the first line
		float m1 = (p2.y - p1.y) / (p2.x - p1.x);
		float c1 = p1.y - m1 * p1.x;

		// Calculate slope and y-intercept for the second line
		float m2 = (p4.y - p3.y) / (p4.x - p3.x);
		float c2 = p3.y - m2 * p3.x;

		// Check if lines are parallel
		if (m1 == m2)
		{
			return false;
		}

		// Calculate intersection point
		intersection.x = (c2 - c1) / (m1 - m2);
		intersection.y = m1 * intersection.x + c1;
		DB_LOG("{} {} {} {}", c1, c2, m1, m2);

		return true;
	}

	void Scene::RenderScene()
	{
		auto cameraEntity = GetActiveCameraEntity();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().Position);
		glm::mat4 rotation = glm::toMat4(glm::quat(cameraEntity.GetComponent<TransformComponent>().Rotation));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().Scale);

		Renderer2D::BeginScene(cameraEntity.GetComponent<CameraComponent>().Camera, scale * rotation * translation);
		// Renderer2D::DrawQuad({ 1, 1 }, { 1, 1 }, { 1, 1, 1, 1 });
		// Renderer2D::DrawQuad({ -2, -3 }, { 1, 2 }, { 1, 1, 1, 1 });
		// Renderer2D::DrawQuad({ 6, -1 }, { 1, 1 }, { 1, 1, 1, 1 });
		// Renderer2D::DrawQuad({ 5, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		// Renderer2D::DrawQuad({ -3, 3 }, { 1, 1 }, { 1, 1, 1, 1 });
		// Renderer2D::DrawQuad({ -5, 0 }, { 3, 3 }, { 1, 1, 1, 1 });

		glm::vec2 mousePos = Input::GetMousePosition();
		glm::mat4 cameramat = (scale * rotation * translation);
		glm::mat4 vp = cameraEntity.GetComponent<CameraComponent>().Camera.GetProjection();
		glm::vec4 ndcMouse = glm::vec4(2 * (mousePos.x / 1280) - 1, -2 * (mousePos.y / 720) + 1, 0, 0);
		glm::vec3 camerapos = cameraEntity.GetComponent<TransformComponent>().Position;
		glm::vec4 mouseWorldSpace(-camerapos.z * (glm::inverse(vp) * ndcMouse));
		mouseWorldSpace.x -= camerapos.x;
		mouseWorldSpace.y -= camerapos.y;

		// Renderer2D::DrawQuad({ 1, 1 }, { 1, 1 }, { 1, 1, 1, 1 });
		std::vector<std::pair<glm::vec2, glm::vec2>> lineSegments;
		lineSegments.emplace_back(std::pair<glm::vec2, glm::vec2>({ 0.5, 0.5 }, { 0.5, 1.5 })); // West
		lineSegments.emplace_back(std::pair<glm::vec2, glm::vec2>({ 0.5, 0.5 }, { 1.5, 0.5 })); // South
		lineSegments.emplace_back(std::pair<glm::vec2, glm::vec2>({ 0.5, 1.5 }, { 1.5, 1.5 })); // North
		lineSegments.emplace_back(std::pair<glm::vec2, glm::vec2>({ 1.5, 0.5 }, { 1.5, 1.5 })); // North

		for (auto pair : lineSegments)
		{
			Renderer2D::DrawLine(pair.first, pair.second, glm::vec4(1.0, 0.5, 1.0, 1));
			// Renderer2D::DrawLine({ 0, 0 }, pair.first, glm::vec4(1));
			// Renderer2D::DrawLine({ 0, 0 }, pair.second, glm::vec4(1));
		}

		std::vector<glm::vec2> vertices;
		vertices.emplace_back(glm::vec2(0.5, 0.5));
		vertices.emplace_back(glm::vec2(0.5, 1.5));
		vertices.emplace_back(glm::vec2(1.5, 1.5));
		vertices.emplace_back(glm::vec2(1.5, 0.5));

		// mouseWorldSpace *= 0.0f;
		for (auto vertex : vertices)
		{
			for (auto pair : lineSegments)
			{
				glm::vec2 intersection = vertex;
				if (findIntersection(mouseWorldSpace, vertex, pair.first, pair.second, intersection))
				{
				}
				Renderer2D::DrawLine(mouseWorldSpace, intersection, { 1, 1, 1, 1 });
			}
		}


		// Renderer2D::DrawLine({ 0, 0 }, 100.0f * mouseWorldSpace, glm::vec4(1));


		// RenderShadows();
		Renderer2D::EndScene();
		return;

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		// glBlendFunc(GL_ONE, GL_ONE);
		RenderCommand::SetClearColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0));
		glm::mat4 scale2 = glm::scale(glm::mat4(1.0), glm::vec3(4));
		// Renderer2D::DrawPointLight(glm::mat4(1.0f), 0.5, 1.0, 1.0, { 1.0, 1.0, 1.0 });
		Renderer2D::DrawPointLight(glm::vec3(0.0), 3.0, 1.0, { 1.0, 1.0, 1.0 });
		// Renderer2D::DrawPointLight({ -1, 0, 0 }, 1.0, 1.0, { 1.0, 0.0, 1.0 });
		// Renderer2D::EndScene();
		// Renderer2D::DrawQuad({ 1, 1 }, { 1, 1 }, { 0, 0, 0, 1 });
		// Renderer2D::DrawPointLight({ 5, -1 }, 0.5f, 1.0f, 10.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
		Renderer2D::EndScene();

		return;


		{
			auto view = m_Registry.view<TransformComponent, AnimatorComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& anim = entity.GetComponent<AnimatorComponent>();

				Renderer2D::DrawSprite(transform.GetTransform(), anim, (int)e);
			}
		}

		{
			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteRendererComponent>();

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)e);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::OnPhysicsUpdate(DeltaTime dt)
	{
		// Set data for Box2D
		{
			auto boxview = m_Registry.view<BoxCollider2DComponent>();

			for (auto e : boxview)
			{
				Entity entity = { e, this };
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2Body* body = (b2Body*)bc2d.RuntimeBody;
				body->SetEnabled(bc2d.Enabled);
			}
		}

		{
			auto boxview = m_Registry.view<CircleCollider2DComponent>();

			for (auto e : boxview)
			{
				Entity entity = { e, this };
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2Body* body = (b2Body*)cc2d.RuntimeBody;
				body->SetEnabled(cc2d.Enabled);
			}
		}

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
			m_PhysicsSim2D->FixedStepSimulation();
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
		m_PhysicsSim2D = new PhysicsSim2D();
		m_PhysicsSim2D->InitSimulation(this);

		{
			auto view = m_Registry.view<BoxCollider2DComponent>();

			for (auto e : view)
			{
				Entity entity = { e, this };
				if (entity.HasComponent<Rigidbody2DComponent>())
				{
					m_PhysicsSim2D->AddColliderWithRigidbody(entity);
				}
				else
				{
					m_PhysicsSim2D->AddCollider(entity);
				}
			}
		}

		{
			auto view = m_Registry.view<CircleCollider2DComponent>();

			for (auto e : view)
			{
				Entity entity = { e, this };
				if (entity.HasComponent<Rigidbody2DComponent>())
				{
					m_PhysicsSim2D->AddColliderWithRigidbody(entity);
				}
				else
				{
					m_PhysicsSim2D->AddCollider(entity);
				}
			}
		}
		PhysicsSim2D::SetActiveSim(this->m_PhysicsSim2D);
	}

	void Scene::OnPhysicsStop()
	{
		PhysicsSim2D::SetActiveSim(nullptr);
		m_PhysicsSim2D->ShutdownSimulation();
		delete m_PhysicsSim2D;
	}

	Entity Scene::GetEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity { entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	// void Scene::LogEntities()
	// {
	// 	DB_LOG("========");
	// 	auto view = m_Registry.view<TagComponent>();
	// 	for (auto entity : view)
	// 	{
	// 		const TagComponent& tc = view.get<TagComponent>(entity);
	// 		DB_LOG(tc.Tag);
	// 	}
	// 	DB_LOG("========");
	// }
}