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
	struct convert<glm::mat4>
	{
		static Node encode(const glm::mat4& mat)
		{
			Node node;
			node.push_back(mat[0]);
			node.push_back(mat[1]);
			node.push_back(mat[2]);
			node.push_back(mat[3]);
			node.SetStyle(EmitterStyle::Flow);
			// node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, glm::mat4& mat)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			mat[0] = node[0].as<glm::vec4>();
			mat[1] = node[1].as<glm::vec4>();
			mat[2] = node[2].as<glm::vec4>();
			mat[3] = node[3].as<glm::vec4>();

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
		out << m[0] << m[1] << m[2] << m[3];
		out << YAML::EndSeq;
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
			default:
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
			out << YAML::Key << "AnimatorComponent";
			out << YAML::BeginMap;

			out << YAML::EndMap;

			DB_CORE_WARN("AnimatorComponent serialization not implemented");
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
			out << YAML::Key << "Size" << YAML::Value << cc2d.Radius;
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
		if (entity.HasComponent<NativeScriptComponent>())
		{
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap;

			auto& nsc = entity.GetComponent<NativeScriptComponent>();
			// const std::type_info& typeInfo = typeid(*nsc.Instance);


			out << YAML::Key << "TypeName" << YAML::Value << nsc.TypeName;
			out << YAML::EndMap;

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
		out.SetIndent(4);

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

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

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		// DB_CORE_WARN("Deserialize function not implemented");
		// return false;
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			DB_CORE_ERROR("Failed to load .dbscn file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		DB_CORE_LOG("Deserializing scene: \"{0}\"", sceneName);

		auto entities = data["Entities"];

		if (entities)
		{
			// for (auto entity : entities)
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

					DB_CORE_WARN("AnimatorComponent deserialization not implemented");
				}
				auto srComponent = entity["SpriteRendererComponent"];
				if (srComponent)
				{
					auto& sr = deserializedEntity.AddComponent<SpriteRendererComponent>();

					auto filepath = srComponent["Sprite"]["Filepath"].as<std::string>();
					TextureSpecifications spec;
					spec.Height = srComponent["Sprite"]["Specifications"]["Height"].as<uint32_t>();
					spec.Width = srComponent["Sprite"]["Specifications"]["Width"].as<uint32_t>();

					if (srComponent["Sprite"]["Specifications"]["Format"].as<std::string>() == "RGB")
						spec.Format = Daybreak::ImageFormat::RGB;
					else if (srComponent["Sprite"]["Specifications"]["Format"].as<std::string>() == "RGBA")
						spec.Format = Daybreak::ImageFormat::RGBA;

					if (srComponent["Sprite"]["Specifications"]["Filter"].as<std::string>() == "Point")
						spec.Filter = Daybreak::TextureFilterType::Point;
					else if (srComponent["Sprite"]["Specifications"]["Filter"].as<std::string>() == "Bilinear")
						spec.Filter = Daybreak::TextureFilterType::Bilinear;

					sr.Sprite = Texture2D::Create(spec, filepath);
					sr.TintColor = srComponent["TintColor"].as<glm::vec4>();
					sr.TilingFactor = srComponent["TilingFactor"].as<float>();
					sr.PixelsPerUnit = srComponent["PixelsPerUnit"].as<uint32_t>();
				}

				auto nscComponent = entity["NativeScriptComponent"];
				if (nscComponent)
				{
					auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();

					nsc.RuntimeBind(nscComponent["TypeName"].as<std::string>());
				}
			}
			return true;
		}

		return true;
	}
}