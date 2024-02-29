#include "dbpch.h"

#include "Daybreak/Scene/SceneSerializer.h"
#include "Daybreak/Scene/Components.h"
#include "Daybreak/Scene/Entity.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <fstream>

/*
// namespace YAML
// {
// 	template<>
// 	struct convert<glm::vec2>
// 	{
// 		static Node encode(const glm::vec2& rhs)
// 		{
// 			Node node;
// 			node.push_back(rhs.x);
// 			node.push_back(rhs.y);
// 			node.SetStyle(EmitterStyle::Flow);
// 			return node;
// 		}

// 		static bool decode(const Node& node, glm::vec2& rhs)
// 		{
// 			if (!node.IsSequence() || node.size() != 2)
// 				return false;

// 			rhs.x = node[0].as<float>();
// 			rhs.y = node[1].as<float>();
// 			return true;
// 		}
// 	};

// 	template<>
// 	struct convert<glm::vec3>
// 	{
// 		static Node encode(const glm::vec3& rhs)
// 		{
// 			Node node;
// 			node.push_back(rhs.x);
// 			node.push_back(rhs.y);
// 			node.push_back(rhs.z);
// 			node.SetStyle(EmitterStyle::Flow);
// 			return node;
// 		}

// 		static bool decode(const Node& node, glm::vec3& rhs)
// 		{
// 			if (!node.IsSequence() || node.size() != 3)
// 				return false;

// 			rhs.x = node[0].as<float>();
// 			rhs.y = node[1].as<float>();
// 			rhs.z = node[2].as<float>();
// 			return true;
// 		}
// 	};

// 	template<>
// 	struct convert<glm::vec4>
// 	{
// 		static Node encode(const glm::vec4& rhs)
// 		{
// 			Node node;
// 			node.push_back(rhs.x);
// 			node.push_back(rhs.y);
// 			node.push_back(rhs.z);
// 			node.push_back(rhs.w);
// 			node.SetStyle(EmitterStyle::Flow);
// 			return node;
// 		}

// 		static bool decode(const Node& node, glm::vec4& rhs)
// 		{
// 			if (!node.IsSequence() || node.size() != 4)
// 				return false;

// 			rhs.x = node[0].as<float>();
// 			rhs.y = node[1].as<float>();
// 			rhs.z = node[2].as<float>();
// 			rhs.w = node[3].as<float>();
// 			return true;
// 		}
// 	};

// 	template<>
// 	struct convert<Daybreak::UUID>
// 	{
// 		static Node encode(const Daybreak::UUID& uuid)
// 		{
// 			Node node;
// 			node.push_back((uint64_t)uuid);
// 			return node;
// 		}

// 		static bool decode(const Node& node, Daybreak::UUID& uuid)
// 		{
// 			uuid = node.as<uint64_t>();
// 			return true;
// 		}
// 	};

// }
*/


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
			out << YAML::Key << "Filepath" << YAML::Value << sr.Sprite->GetFilepath();
			out << YAML::Key << "TintColor" << YAML::Value << sr.TintColor;
			out << YAML::Key << "TilingFactor" << YAML::Value << sr.TilingFactor;
			// out << YAML::Key << "Width" << YAML::Value << sr.TilingFactor;
			// out << YAML::Key << "Height" << YAML::Value << sr.TilingFactor;
			// out << YAML::Key << "FIlterType" << YAML::Value << sr.TilingFactor;

			out << YAML::EndMap;

			DB_CORE_WARN("SpriteRendererComponent serialization not fully implemented");
		}
		if (entity.HasComponent<AnimatorComponent>())
		{
			DB_CORE_WARN("AnimatorComponent serialization implemented");
		}
		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			DB_CORE_WARN("Rigidbody2DComponent serialization implemented");
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
			DB_CORE_WARN("CameraComponent serialization implemented");
		}
		if (entity.HasComponent<NativeScriptComponent>())
		{
			DB_CORE_WARN("NativeScriptComponent serialization implemented");
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

    void SceneSerializer::Deserialize(const std::string& filepath)
	{
	}
}