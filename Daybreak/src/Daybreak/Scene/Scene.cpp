#include "dbpch.h"

#include "Daybreak/Scene/Scene.h"

#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Renderer/Renderer2D.h"
#include "Daybreak/Core/Time.h"
#include "Daybreak/Scripting/Script.h"
#include "Daybreak/Renderer/RenderCommand.h"
#include "Daybreak/Scene/SceneSerializer.h"

#include <box2d/box2d.h>
#include <variant>

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
		entity.AddComponent<ActiveComponent>();

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
		entity.AddComponent<ActiveComponent>();

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

		if (entity.HasComponent<ScriptComponent>())
		{
			auto sc = entity.GetComponent<ScriptComponent>();
			sc.Instance->OnDestroy();
		}

		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}

	template<typename T>
	Entity& Scene::GetParentEntityWith(Entity& entity)
	{
		if (entity.HasComponent<T>())
		{
			return entity;
		}
		RelationshipComponent& rc = entity.GetComponent<RelationshipComponent>();
		if (rc.ParentID)
		{
			Entity parent = GetEntityByUUID(rc.ParentID);
			return GetParentEntityWith<T>(parent);
		}
		DB_CORE_ASSERT(false, "Entity line with {} does not have component {}", entity.GetName(), typeid(T).name());
	}

	template<typename T>
	bool Scene::HasParentEntityWith(Entity& entity)
	{
		if (entity.HasComponent<T>())
		{
			return true;
		}
		RelationshipComponent& rc = entity.GetComponent<RelationshipComponent>();
		if (rc.ParentID)
		{
			Entity parent = GetEntityByUUID(rc.ParentID);
			return HasParentEntityWith<T>(parent);
		}
		return false;
	}

	void Scene::OnRuntimeStart()
	{
		m_Registry.view<ScriptComponent>().each([=](auto entity, ScriptComponent& sc)
												{
			// TODO: Move to Scene::OnScenePlay
			if (!sc.Instance)
			{
				sc.Instance = sc.InstantiateScript(sc.TypeName);
				sc.Instance->m_Entity = Entity{ entity, this };
				sc.Instance->OnCreate();
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
				if (!entity.IsActive())
				{
					continue;
				}

				auto& anim = entity.GetComponent<AnimatorComponent>();
				if (anim.IsPlaying && (anim.Controller != NULL))
				{
					anim.Controller->Update(dt);
				}
			}
		}

		m_Registry.view<ScriptComponent, ActiveComponent>().each(
			[=](entt::entity entity, ScriptComponent& sc, ActiveComponent& ac)
			{
				if (!ac.Active)
				{
					return;
				}
				// This runs here to catch any sc that are instianted during scene play
				if (!sc.Instance)
				{
					sc.Instance = sc.InstantiateScript(sc.TypeName);
					sc.Instance->m_Entity = Entity { entity, this };
					sc.Instance->OnCreate();
				}

				sc.Instance->OnUpdate(dt);
			});

		OnPhysicsUpdate(dt);
		RenderScene(GetActiveCameraEntity());
	}

	void Scene::OnRuntimeEnd()
	{
		OnPhysicsStop();

		m_Registry.view<ScriptComponent>().each([=](auto entity, ScriptComponent& sc)
												{
				// TODO: Move to Scene::OnSceneStop
				// This NEEDS to be fixed. Not every sc will have an active instance?						  
				if (!sc.Instance)
				{
					sc.Instance = sc.InstantiateScript(sc.TypeName);
					sc.Instance->m_Entity = Entity{ entity, this };
					sc.Instance->OnDestroy();
				} });
	}

	// FIXME: Temporary
	Ref<Scene> Scene::Copy(const Ref<Scene>& input)
	{
		Ref<Scene> output = CreateRef<Scene>();

		SceneSerializer inputSerializer(input);
		SceneSerializer outputSerializer(output);

		inputSerializer.Serialize("../Sandbox/assets/scenes/TempLoadedScene.scene");
		outputSerializer.Deserialize("../Sandbox/assets/scenes/TempLoadedScene.scene");


		return output;
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

	glm::mat4 Scene::GetWorldTransform(Entity& entity)
	{
		RelationshipComponent& rc = entity.GetComponent<RelationshipComponent>();
		TransformComponent& tr = entity.GetComponent<TransformComponent>();
		if (rc.ParentID)
		{
			Entity parent = GetEntityByUUID(rc.ParentID);
			return GetWorldTransform(parent) * tr.GetTransform();
		}
		else
		{
			return tr.GetTransform();
		}
	}

	void Scene::RenderScene(const Entity& cameraEntity)
	{
		enum class SceneRenderObjectType
		{
			Sprite,
			Animator,
			Text
		};

		struct SceneRenderObject
		{
			entt::entity Entity;
			SceneRenderObjectType RenderType;
			uint32_t RenderLayer;
			std::variant<AnimatorComponent, SpriteRendererComponent, TextRendererComponent> Component;
			glm::mat4 Transform;

			bool operator<(const SceneRenderObject& obj) const
			{
				return Transform[3][3] < obj.Transform[3][3] && RenderType > obj.RenderType;
			}
		};

		auto animatorView = m_Registry.view<TransformComponent, AnimatorComponent>();
		auto spriteView = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		auto textView = m_Registry.view<TransformComponent, TextRendererComponent>();

		const size_t numberObjects = animatorView.size() + spriteView.size() + textView.size();
		std::vector<SceneRenderObject> renderObjects(numberObjects);
		uint32_t renderObjectsIndex = 0;

		for (auto e : animatorView)
		{
			Entity entity = { e, this };
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			AnimatorComponent& anim = entity.GetComponent<AnimatorComponent>();
			renderObjects[renderObjectsIndex] = {
				e,
				SceneRenderObjectType::Animator,
				anim.RenderLayer,
				anim,
				GetWorldTransform(entity),
			};
			renderObjectsIndex++;
		}
		for (auto e : spriteView)
		{
			Entity entity = { e, this };
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			SpriteRendererComponent& sr = entity.GetComponent<SpriteRendererComponent>();
			renderObjects[renderObjectsIndex] = {
				e,
				SceneRenderObjectType::Sprite,
				sr.RenderLayer,
				sr,
				GetWorldTransform(entity),
			};
			renderObjectsIndex++;
		}
		for (auto e : textView)
		{
			Entity entity = { e, this };
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			TextRendererComponent& text = entity.GetComponent<TextRendererComponent>();
			renderObjects[renderObjectsIndex] = {
				e,
				SceneRenderObjectType::Text,
				text.RenderLayer,
				text,
				GetWorldTransform(entity),
			};
			renderObjectsIndex++;
		}

		std::sort(renderObjects.begin(), renderObjects.end());

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -1, 1) * cameraEntity.GetComponent<TransformComponent>().Position);
		glm::mat4 rotation = glm::toMat4(glm::quat(cameraEntity.GetComponent<TransformComponent>().Rotation));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().Scale);

		Renderer2D::BeginScene(cameraEntity.GetComponent<CameraComponent>().Camera, scale * rotation * translation);
		for (int i = 0; i < numberObjects; i++)
		{
			Entity entity = { renderObjects[i].Entity, this };

			if (!entity.IsActive())
			{
				continue;
			}

			auto& transform = entity.GetComponent<TransformComponent>();
			switch (renderObjects[i].RenderType)
			{
				case SceneRenderObjectType::Sprite:
				{
					Renderer2D::DrawSprite(renderObjects[i].Transform, std::get<SpriteRendererComponent>(renderObjects[i].Component), (int)(renderObjects[i].Entity));
					break;
				}
				case SceneRenderObjectType::Animator:
				{
					Renderer2D::DrawSprite(renderObjects[i].Transform, std::get<AnimatorComponent>(renderObjects[i].Component), (int)(renderObjects[i].Entity));
					break;
				}
				case SceneRenderObjectType::Text:
				{
					TextRendererComponent text = std::get<TextRendererComponent>(renderObjects[i].Component);
					Renderer2D::DrawString(text.Text, text.Font, renderObjects[i].Transform, text.Color, text.Kerning, text.LineSpacing, (int)(renderObjects[i].Entity));
					break;
				}
			}
		}
		Renderer2D::EndScene();
	}

	void Scene::OnPhysicsUpdate(DeltaTime dt)
	{
		// Set data for Box2D
		{
			auto view = m_Registry.view<BoxCollider2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				b2Body* body = (b2Body*)bc2d.RuntimeBody;
				body->SetEnabled(bc2d.Enabled && entity.IsActive());
			}
		}
		{
			auto view = m_Registry.view<CircleCollider2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
				b2Body* body = (b2Body*)cc2d.RuntimeBody;
				body->SetEnabled(cc2d.Enabled && entity.IsActive());
			}
		}
		{
			auto view = m_Registry.view<PolygonCollider2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& pc2d = entity.GetComponent<PolygonCollider2DComponent>();
				b2Body* body = (b2Body*)pc2d.RuntimeBody;
				body->SetEnabled(pc2d.Enabled && entity.IsActive());
			}
		}

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			Rigidbody2DComponent& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			if (body)
			{
				std::string name = entity.GetName();
				TransformComponent& transform = entity.GetComponent<TransformComponent>();

				body->SetLinearVelocity({ rb2d.Velocity.x, rb2d.Velocity.y });
				body->SetTransform({ transform.Position.x, transform.Position.y }, transform.Rotation.z);
				body->SetEnabled(entity.IsActive());
			}
		}

		double startTime = Time::GetTime();
		while (m_LastUpdateTime < startTime)
		{
			m_PhysicsSim2D->FixedStepSimulation();
			m_LastUpdateTime += 0.016f; // For now this is the fixed delta time
		}

		// Retrieve data from Box2D
		for (auto e : view)
		{
			Entity entity = { e, this };
			Rigidbody2DComponent& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			if (body)
			{
				TransformComponent& transform = entity.GetComponent<TransformComponent>();

				const auto& position = body->GetPosition();
				transform.Position.x = position.x;
				transform.Position.y = position.y;
				transform.Rotation.z = body->GetAngle();

				const auto& velocity = body->GetLinearVelocity();
				rb2d.Velocity.x = velocity.x;
				rb2d.Velocity.y = velocity.y;
			}
		}
	}

	void Scene::OnPhysicsStart()
	{
		m_PhysicsSim2D = new PhysicsSim2D();
		m_PhysicsSim2D->InitSimulation(this);

		auto rb2dView = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : rb2dView)
		{
			Entity entity = { e, this };
			m_PhysicsSim2D->InitBody(entity);
		}

		auto bc2dView = m_Registry.view<BoxCollider2DComponent>();
		for (auto e : bc2dView)
		{
			Entity entity = { e, this };
			BoxCollider2DComponent& bc2d = entity.GetComponent<BoxCollider2DComponent>();

			if (HasParentEntityWith<Rigidbody2DComponent>(entity))
			{
				Entity parent = GetParentEntityWith<Rigidbody2DComponent>(entity);

				glm::vec2 offset = glm::vec2(GetWorldTransform(entity)[3] - GetWorldTransform(parent)[3]);

				Rigidbody2DComponent& rb2d = parent.GetComponent<Rigidbody2DComponent>();
				m_PhysicsSim2D->AddBoxFixture(entity, bc2d, rb2d, offset);
			}
			else
			{
				m_PhysicsSim2D->AddBoxFixtureNoBody(entity);
			}
		}
		auto cc2dView = m_Registry.view<CircleCollider2DComponent>();
		for (auto e : cc2dView)
		{
			Entity entity = { e, this };
			CircleCollider2DComponent& cc2d = entity.GetComponent<CircleCollider2DComponent>();

			if (HasParentEntityWith<Rigidbody2DComponent>(entity))
			{
				Entity parent = GetParentEntityWith<Rigidbody2DComponent>(entity);

				glm::vec2 offset = glm::vec2(GetWorldTransform(entity)[3] - GetWorldTransform(parent)[3]);

				Rigidbody2DComponent& rb2d = parent.GetComponent<Rigidbody2DComponent>();
				m_PhysicsSim2D->AddCircleFixture(entity, cc2d, rb2d, offset);
			}
			else
			{
				m_PhysicsSim2D->AddCircleFixtureNoBody(entity);
			}
		}
		auto pc2dView = m_Registry.view<PolygonCollider2DComponent>();
		for (auto e : pc2dView)
		{
			Entity entity = { e, this };
			PolygonCollider2DComponent& pc2d = entity.GetComponent<PolygonCollider2DComponent>();

			if (HasParentEntityWith<Rigidbody2DComponent>(entity))
			{
				Entity parent = GetParentEntityWith<Rigidbody2DComponent>(entity);

				glm::vec2 offset = glm::vec2(GetWorldTransform(entity)[3] - GetWorldTransform(parent)[3]);

				Rigidbody2DComponent& rb2d = parent.GetComponent<Rigidbody2DComponent>();
				m_PhysicsSim2D->AddPolygonFixture(entity, pc2d, rb2d, offset);
			}
			else
			{
				m_PhysicsSim2D->AddPolygonFixtureNoBody(entity);
			}
		}
	}

	void Scene::OnPhysicsStop()
	{
		m_PhysicsSim2D->ShutdownSimulation();
		delete m_PhysicsSim2D;
		m_PhysicsSim2D = nullptr;
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
