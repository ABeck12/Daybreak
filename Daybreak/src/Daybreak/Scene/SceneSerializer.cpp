#include "dbpch.h"

#include "Daybreak/Scene/SceneSerializer.h"
#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"
#include "Daybreak/Utils/YamlConversion.h"
#include "Daybreak/Assets/AssetManager/AssetManager.h"

namespace Daybreak
{
	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		DB_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity does not have ID Component");

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();


		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<RelationshipComponent>())
		{
			out << YAML::Key << "RelationshipComponent";
			out << YAML::BeginMap;

			auto& rc = entity.GetComponent<RelationshipComponent>();
			out << YAML::Key << "ParentID" << YAML::Value << rc.ParentID;
			out << YAML::Key << "AmountOfChildren" << YAML::Value << rc.AmountOfChildren;
			out << YAML::Key << "ChildrenIDs" << YAML::Value << rc.ChildrenIDs;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& sr = entity.GetComponent<SpriteRendererComponent>();

			if (sr.Sprite)
			{
				auto am = AssetManager::Get();
				std::filesystem::path assetPath = am->GetTexture2DFilepath(sr.Sprite);
				am->SerializeTexture2D(sr.Sprite, assetPath);
				out << YAML::Key << "Sprite" << YAML::Value << assetPath;
			}
			else
			{
				out << YAML::Key << "Sprite" << YAML::Value << YAML::Null;
			}

			out << YAML::Key << "TintColor" << YAML::Value << sr.TintColor;
			out << YAML::Key << "TilingFactor" << YAML::Value << sr.TilingFactor;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<AnimatorComponent>())
		{
			auto& anim = entity.GetComponent<AnimatorComponent>();

			out << YAML::Key << "AnimatorComponent";
			out << YAML::BeginMap;

			if (anim.Controller)
			{
				auto am = AssetManager::Get();
				std::filesystem::path assetPath = am->GetAnimationControllerFilepath(anim.Controller);
				am->SerializeAnimationController(anim.Controller, assetPath);
				out << YAML::Key << "Controller" << YAML::Value << assetPath;
			}
			else
			{
				out << YAML::Key << "Controller" << YAML::Value << YAML::Null;
			}
			out << YAML::Key << "IsPlaying" << YAML::Value << anim.IsPlaying;
			out << YAML::Key << "TintColor" << YAML::Value << anim.TintColor;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			switch (rb2d.Type)
			{
				case (Rigidbody2DComponent::BodyType::Dynamic):
					out << YAML::Key << "Type" << YAML::Value << "Dynamic";
					break;
				case (Rigidbody2DComponent::BodyType::Static):
					out << YAML::Key << "Type" << YAML::Value << "Static";
					break;
				case (Rigidbody2DComponent::BodyType::Kinematic):
					out << YAML::Key << "Type" << YAML::Value << "Kinematic";
					break;
			}
			out << YAML::Key << "ContinuousDetection" << YAML::Value << rb2d.ContinuousDetection;
			out << YAML::Key << "AllowSleep" << YAML::Value << rb2d.AllowSleep;
			out << YAML::Key << "Velocity" << YAML::Value << rb2d.Velocity;
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;
			out << YAML::Key << "Mass" << YAML::Value << rb2d.Mass;
			out << YAML::Key << "GravityScale" << YAML::Value << rb2d.GravityScale;
			out << YAML::Key << "AngularDrag" << YAML::Value << rb2d.AngularDrag;
			out << YAML::Key << "LinearDrag" << YAML::Value << rb2d.LinearDrag;
			out << YAML::Key << "Density" << YAML::Value << rb2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << rb2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << rb2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << rb2d.RestitutionThreshold;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Size" << YAML::Value << bc2d.Size;
			out << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			out << YAML::Key << "CollisionLayer" << YAML::Value << bc2d.CollisionLayer;
			out << YAML::Key << "IsTrigger" << YAML::Value << bc2d.IsTrigger;
			out << YAML::Key << "Enabled" << YAML::Value << bc2d.Enabled;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Radius" << YAML::Value << cc2d.Radius;
			out << YAML::Key << "Offset" << YAML::Value << cc2d.Offset;
			out << YAML::Key << "CollisionLayer" << YAML::Value << cc2d.CollisionLayer;
			out << YAML::Key << "IsTrigger" << YAML::Value << cc2d.IsTrigger;
			out << YAML::Key << "Enabled" << YAML::Value << cc2d.Enabled;


			out << YAML::EndMap;
		}
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cc = entity.GetComponent<CameraComponent>();

			out << YAML::Key << "CameraProjection" << YAML::Value << cc.Camera.GetProjection();
			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			switch (cc.Type)
			{
				case (CameraComponent::ProjectionType::Orthographic):
					out << YAML::Key << "Type" << YAML::Value << "Orthographic";
					break;
				case (CameraComponent::ProjectionType::Perspective):
					out << YAML::Key << "Type" << YAML::Value << "Perspective";
					break;
			}

			out << YAML::EndMap;
		}
		if (entity.HasComponent<ScriptComponent>())
		{
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;

			auto& sc = entity.GetComponent<ScriptComponent>();

			out << YAML::Key << "TypeName" << YAML::Value << sc.TypeName;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}


	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::filesystem::path& filepath) const
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_SceneName;

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
								 {
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity); });

		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			DB_CORE_ERROR("Failed to load .scene file \"{0}\"\n     {1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		m_Scene->m_SceneName = sceneName;
		DB_CORE_LOG("Deserializing scene: \"{0}\"", sceneName);

		auto entities = data["Entities"];

		if (entities)
		{
			for (size_t i = entities.size(); i > 0; --i)
			{
				auto entity = entities[i - 1];
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
				}

				DB_CORE_LOG("  Deserialized entity: \"{1}\", ID = {0}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}
				auto relationshipComponent = entity["RelationshipComponent"];
				if (relationshipComponent)
				{
					auto& rc = deserializedEntity.GetComponent<RelationshipComponent>();
					rc.ParentID = relationshipComponent["ParentID"].as<uint64_t>();
					rc.AmountOfChildren = relationshipComponent["AmountOfChildren"].as<uint32_t>();
					rc.ChildrenIDs = relationshipComponent["ChildrenIDs"].as<std::vector<UUID>>();
				}
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					cc.Camera.SetProjection(cameraComponent["CameraProjection"].as<glm::mat4>());
					cc.Primary = cameraComponent["Primary"].as<bool>();
					std::string type = cameraComponent["Type"].as<std::string>();
					if (type == "Orthograpthic")
						cc.Type = CameraComponent::ProjectionType::Orthographic;
					else if (type == "Perspective")
						cc.Type = CameraComponent::ProjectionType::Perspective;
				}
				auto bc2dComponent = entity["BoxCollider2DComponent"];
				if (bc2dComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Size = bc2dComponent["Size"].as<glm::vec2>();
					bc2d.Offset = bc2dComponent["Offset"].as<glm::vec2>();
					bc2d.CollisionLayer = bc2dComponent["CollisionLayer"].as<uint16_t>();
					bc2d.IsTrigger = bc2dComponent["IsTrigger"].as<bool>();
					bc2d.Enabled = bc2dComponent["Enabled"].as<bool>();
				}
				auto cc2dComponent = entity["CircleCollider2DComponent"];
				if (cc2dComponent)
				{
					auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc2d.Radius = cc2dComponent["Radius"].as<float>();
					cc2d.Offset = cc2dComponent["Offset"].as<glm::vec2>();
					cc2d.CollisionLayer = cc2dComponent["CollisionLayer"].as<uint16_t>();
					cc2d.IsTrigger = cc2dComponent["IsTrigger"].as<bool>();
					cc2d.Enabled = cc2dComponent["Enabled"].as<bool>();
				}
				auto rb2dComponent = entity["Rigidbody2DComponent"];
				if (rb2dComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					std::string type = rb2dComponent["Type"].as<std::string>();
					if (type == "Dynamic")
						rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
					else if (type == "Static")
						rb2d.Type = Rigidbody2DComponent::BodyType::Static;
					else if (type == "kinematic")
						rb2d.Type = Rigidbody2DComponent::BodyType::Kinematic;
					rb2d.ContinuousDetection = rb2dComponent["ContinuousDetection"].as<bool>();
					rb2d.AllowSleep = rb2dComponent["AllowSleep"].as<bool>();
					rb2d.Velocity = rb2dComponent["Velocity"].as<glm::vec2>();
					rb2d.FixedRotation = rb2dComponent["FixedRotation"].as<bool>();
					rb2d.Mass = rb2dComponent["Mass"].as<float>();
					rb2d.GravityScale = rb2dComponent["GravityScale"].as<float>();
					rb2d.AngularDrag = rb2dComponent["AngularDrag"].as<float>();
					rb2d.LinearDrag = rb2dComponent["LinearDrag"].as<float>();
					rb2d.Density = rb2dComponent["Density"].as<float>();
					rb2d.Friction = rb2dComponent["Friction"].as<float>();
					rb2d.Restitution = rb2dComponent["Restitution"].as<float>();
					rb2d.RestitutionThreshold = rb2dComponent["RestitutionThreshold"].as<float>();
				}
				auto animComponent = entity["AnimatorComponent"];
				if (animComponent)
				{
					auto& anim = deserializedEntity.AddComponent<AnimatorComponent>();

					if (!animComponent["Controller"].IsNull())
					{
						auto am = AssetManager::Get();
						std::filesystem::path assetPath = animComponent["Controller"].as<std::filesystem::path>();
						anim.Controller = am->LoadAnimationController(assetPath);
					}
					else
					{
						DB_CORE_WARN("Controller for entity \"{}\" is null!", deserializedEntity.GetName());
					}

					anim.IsPlaying = animComponent["IsPlaying"].as<bool>();
					anim.TintColor = animComponent["TintColor"].as<glm::vec4>();
				}
				auto srComponent = entity["SpriteRendererComponent"];
				if (srComponent)
				{
					auto& sr = deserializedEntity.AddComponent<SpriteRendererComponent>();

					if (!srComponent["Sprite"].IsNull())
					{
						auto am = AssetManager::Get();
						std::filesystem::path assetPath = srComponent["Sprite"].as<std::filesystem::path>();
						sr.Sprite = am->LoadTexture2D(assetPath);
					}
					else
					{
						DB_CORE_WARN("  Sprite for entity \"{}\" is null!", deserializedEntity.GetName());
					}

					sr.TintColor = srComponent["TintColor"].as<glm::vec4>();
					sr.TilingFactor = srComponent["TilingFactor"].as<float>();
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();

					sc.RuntimeBind(scriptComponent["TypeName"].as<std::string>());
				}
			}
			return true;
		}

		return true;
	}
}