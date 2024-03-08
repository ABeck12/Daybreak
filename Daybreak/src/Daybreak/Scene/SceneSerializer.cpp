#include "dbpch.h"

#include "Daybreak/Scene/SceneSerializer.h"
#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"

#define YAML_CPP_DLL 
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <fstream>


namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Daybreak::UUID>
	{
		static Node encode(const Daybreak::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Daybreak::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};

}



namespace Daybreak
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat4& m)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		out	<< m[0][0] << m[0][1] << m[0][1] << m[0][1];
		out	<< m[1][0] << m[1][1] << m[1][2] << m[1][3];
		out	<< m[2][0] << m[2][1] << m[2][2] << m[2][3];
		out	<< m[3][0] << m[3][1] << m[3][2] << m[3][3];
		out	<< YAML::EndSeq;
		return out;
	}

	// Temporary
	YAML::Emitter& operator<<(YAML::Emitter& out, const Ref<Texture2D>& texture)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Filepath" << YAML::Value << texture->GetFilepath();
		out << YAML::Key << "Specifications";
		out << YAML::BeginMap;
		out << YAML::Key << "Width" << YAML::Value << texture->GetTexutreSpecifications().Width;
		out << YAML::Key << "Height" << YAML::Value << texture->GetTexutreSpecifications().Height;
		switch (texture->GetTexutreSpecifications().Format)
		{
			case ImageFormat::RGB:
				out << YAML::Key << "Format" << YAML::Value << "RGB";
				break;
			case ImageFormat::RGBA:
				out << YAML::Key << "Format" << YAML::Value << "RGBA";
				break;
			case ImageFormat::None:
				out << YAML::Key << "Format" << YAML::Value << "None";
				DB_CORE_ERROR("Can't serialize image type \"None\"");
				break;
		}
		switch (texture->GetTexutreSpecifications().Filter)
		{
			case TextureFilterType::Point:
				out << YAML::Key << "Filter" << YAML::Value << "Point";
				break;
			case TextureFilterType::Bilinear:
				out << YAML::Key << "Filter" << YAML::Value << "Bilinear";
				break;					
		}
		out << YAML::EndMap;
		out << YAML::EndMap;

		return out;
	}

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

			out << YAML::Key << "Sprite" << YAML::Value << sr.Sprite;

			out << YAML::Key << "TintColor" << YAML::Value << sr.TintColor;
			out << YAML::Key << "TilingFactor" << YAML::Value << sr.TilingFactor;
			out << YAML::Key << "PixelsPerUnit" << YAML::Value << sr.PixelsPerUnit;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<AnimatorComponent>())
		{
			DB_CORE_WARN("AnimatorComponent serialization not implemented");
		}
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
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
			out << YAML::Key << "Velocity" << YAML::Value << rb2d.Velocity;
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

			out << YAML::EndMap;
		}
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& bc2d = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Size" << YAML::Value << bc2d.Radius;
			out << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			out << YAML::Key << "CollisionLayer" << YAML::Value << bc2d.CollisionLayer;
			out << YAML::Key << "IsTrigger" << YAML::Value << bc2d.IsTrigger;

			out << YAML::EndMap;
		}
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cc = entity.GetComponent<CameraComponent>();

			// out << YAML::Key << "CameraProjection" << YAML::Value << cc.Camera.GetProjection();
			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;

			out << YAML::EndMap;
			DB_CORE_WARN("CameraComponent serialization not fully implemented");
		}
		if (entity.HasComponent<NativeScriptComponent>())
		{
			DB_CORE_WARN("NativeScriptComponent serialization not implemented");
		}

		out << YAML::EndMap;
	}


	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

    bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		DB_CORE_WARN("Deserialize function not implemented");
		return false;
		// YAML::Node data;
		// try
		// {
		// 	data = YAML::LoadFile(filepath);
		// }
		// catch (YAML::ParserException e)
		// {
		// 	DB_CORE_ERROR("Failed to load .dbscn file '{0}'\n     {1}", filepath, e.what());
		// 	return false;
		// }

		// if (!data["Scene"])
		// 	return false;

		// std::string sceneName = data["Scene"].as<std::string>();
		// DB_CORE_LOG("Deserializing scene '{0}'", sceneName);

		// auto entities = data["Entities"];
		// if (entities)
		// {
		// 	for (auto entity : entities)
		// 	{
		// 		uint64_t uuid = entity["Entity"].as<uint64_t>();

		// 		std::string name;
		// 		auto tagComponent = entity["TagComponent"];
		// 		if (tagComponent)
		// 			name = tagComponent["Tag"].as<std::string>();

		// 		DB_CORE_LOG("Deserialized entity with ID = {0}, name = {1}", uuid, name);

		// 		Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

		// 		auto transformComponent = entity["TransformComponent"];
		// 		if (transformComponent)
		// 		{
		// 			// Entities always have transforms
		// 			auto& tc = deserializedEntity.GetComponent<TransformComponent>();
		// 			tc.Position = transformComponent["Position"].as<glm::vec3>();
		// 			tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
		// 			tc.Scale = transformComponent["Scale"].as<glm::vec3>();
		// 		}
		// 	}
		// }

		// return true;
	}
}